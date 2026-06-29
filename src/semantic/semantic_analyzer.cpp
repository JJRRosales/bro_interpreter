#include "semantic_analyzer.h"
#include <iostream>

// ─────────────────────────────────────────────────────────────────────────────
//  Public entry point
// ─────────────────────────────────────────────────────────────────────────────
bool SemanticAnalyzer::analyze(ProgramNode& program) {
    program.accept(*this);
    return !had_errors();
}

// ─────────────────────────────────────────────────────────────────────────────
//  Helpers
// ─────────────────────────────────────────────────────────────────────────────
void SemanticAnalyzer::error(const std::string& msg, int line, int col) {
    m_errors.push_back({ msg, line, col });
}

ValueType SemanticAnalyzer::infer_literal_type(const Token& tok) {
    switch (tok.kind) {
        case TokenKind::V_INT:       return ValueType::Int;
        case TokenKind::V_FLOAT:     return ValueType::Float;
        case TokenKind::V_STRING:    return ValueType::String;
        case TokenKind::KW_LEGIT:
        case TokenKind::KW_FAKE:     return ValueType::Bool;
        case TokenKind::V_HEX:       return ValueType::Hex;
        case TokenKind::V_BINARY:    return ValueType::Binary;
        case TokenKind::V_SCI:       return ValueType::Sci;
        case TokenKind::V_IMAGINARY: return ValueType::Imaginary;
        case TokenKind::KW_GHOSTED:  return ValueType::Null;
        default:                     return ValueType::Unknown;
    }
}

ValueType SemanticAnalyzer::infer_expr_type(ExprNode& expr) {
    if (auto* lit = dynamic_cast<LiteralNode*>(&expr)) {
        return infer_literal_type(lit->token);
    }
    if (auto* id = dynamic_cast<IdentifierNode*>(&expr)) {
        Symbol* sym = m_table.lookup(id->token.lexeme);
        if (sym) return sym->type;
    }
    // For compound expressions (binary, unary, function calls) we don't
    // do full type propagation here — that belongs to a type-checker phase.
    return ValueType::Unknown;
}

void SemanticAnalyzer::visit_stmt_list(std::vector<std::unique_ptr<StmtNode>>& stmts) {
    for (auto& stmt : stmts) {
        stmt->accept(*this);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  Program
// ─────────────────────────────────────────────────────────────────────────────
void SemanticAnalyzer::visit(ProgramNode& n) {
    m_table.push_scope(); // open global scope — kept alive for dump_symbols()
    visit_stmt_list(n.stmts);
    // NOTE: global scope is intentionally NOT popped so dump_symbols() works.
}

// ─────────────────────────────────────────────────────────────────────────────
//  Declarations
// ─────────────────────────────────────────────────────────────────────────────

// id = expr  — declares on first use, reassigns on subsequent uses
void SemanticAnalyzer::visit(VarDeclNode& n) {
    // First analyse the RHS so inner identifiers are checked
    if (n.value) n.value->accept(*this);

    const std::string& name = n.name.lexeme;

    // If the identifier already exists anywhere in scope chain as a constant → error
    Symbol* existing = m_table.lookup(name);
    if (existing && existing->kind == SymbolKind::Constant) {
        error("AssignToConst: '" + name + "' is a constant and cannot be reassigned",
              n.name.line, n.name.column);
        return;
    }

    if (!existing) {
        // First use → declare in current scope
        Symbol sym;
        sym.name      = name;
        sym.kind      = SymbolKind::Variable;
        sym.type      = n.value ? infer_expr_type(*n.value) : ValueType::Unknown;
        sym.decl_line = n.name.line;
        sym.decl_col  = n.name.column;
        m_table.declare(sym);
    }
    // If existing and it's a var → silent reassignment (valid in Bro)
}

// lockedIn id = expr
void SemanticAnalyzer::visit(ConstDeclNode& n) {
    if (n.value) n.value->accept(*this);

    const std::string& name = n.name.lexeme;

    // Constants cannot be re-declared in the same scope
    if (m_table.lookup_current(name)) {
        error("RedeclaredVariable: '" + name + "' is already declared in this scope",
              n.name.line, n.name.column);
        return;
    }

    Symbol sym;
    sym.name      = name;
    sym.kind      = SymbolKind::Constant;
    sym.type      = n.value ? infer_expr_type(*n.value) : ValueType::Unknown;
    sym.decl_line = n.name.line;
    sym.decl_col  = n.name.column;
    m_table.declare(sym);
}

// id += expr  /  id -= expr  /  id *= expr  /  id /= expr
void SemanticAnalyzer::visit(AssignNode& n) {
    if (n.value) n.value->accept(*this);

    const std::string& name = n.name.lexeme;
    Symbol* sym = m_table.lookup(name);

    if (!sym) {
        error("UndeclaredVariable: '" + name + "' was not declared before use",
              n.name.line, n.name.column);
        return;
    }
    if (sym->kind == SymbolKind::Constant) {
        error("AssignToConst: '" + name + "' is a constant and cannot be reassigned",
              n.name.line, n.name.column);
    }
}

// id++  /  id--
void SemanticAnalyzer::visit(UnaryStmtNode& n) {
    const std::string& name = n.name.lexeme;
    Symbol* sym = m_table.lookup(name);

    if (!sym) {
        error("UndeclaredVariable: '" + name + "' was not declared before use",
              n.name.line, n.name.column);
        return;
    }
    if (sym->kind == SymbolKind::Constant) {
        error("AssignToConst: '" + name + "' is a constant and cannot be modified",
              n.name.line, n.name.column);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  Function declaration
// ─────────────────────────────────────────────────────────────────────────────
void SemanticAnalyzer::visit(FuncDeclNode& n) {
    const std::string& name = n.name.lexeme;

    // Declare the function in the current (outer) scope before entering the body,
    // so recursive calls can resolve the function name.
    if (m_table.lookup_current(name)) {
        error("RedeclaredVariable: function '" + name + "' is already declared in this scope",
              n.name.line, n.name.column);
    } else {
        Symbol sym;
        sym.name        = name;
        sym.kind        = SymbolKind::Function;
        sym.type        = ValueType::Unknown;
        sym.param_count = static_cast<int>(n.params.size());
        sym.decl_line   = n.name.line;
        sym.decl_col    = n.name.column;
        m_table.declare(sym);
    }

    // Open a new scope for the function body
    m_table.push_scope();
    bool prev = m_in_function;
    m_in_function = true;

    // Declare each parameter in the function's scope
    for (const auto& param : n.params) {
        Symbol psym;
        psym.name      = param.lexeme;
        psym.kind      = SymbolKind::Parameter;
        psym.type      = ValueType::Unknown;
        psym.decl_line = param.line;
        psym.decl_col  = param.column;
        if (!m_table.declare(psym)) {
            error("RedeclaredVariable: parameter '" + param.lexeme + "' is duplicated",
                  param.line, param.column);
        }
    }

    visit_stmt_list(n.body);

    m_in_function = prev;
    m_table.pop_scope();
}

// ─────────────────────────────────────────────────────────────────────────────
//  Control flow
// ─────────────────────────────────────────────────────────────────────────────
void SemanticAnalyzer::visit(IfNode& n) {
    for (auto& branch : n.branches) {
        if (branch.condition) branch.condition->accept(*this);
        m_table.push_scope();
        visit_stmt_list(branch.body);
        m_table.pop_scope();
    }
    if (n.has_else) {
        m_table.push_scope();
        visit_stmt_list(n.else_body);
        m_table.pop_scope();
    }
}

void SemanticAnalyzer::visit(ForNode& n) {
    m_table.push_scope(); // init variable lives in this scope
    if (n.init)      n.init->accept(*this);
    if (n.condition) n.condition->accept(*this);
    if (n.update)    n.update->accept(*this);
    visit_stmt_list(n.body);
    m_table.pop_scope();
}

void SemanticAnalyzer::visit(WhileNode& n) {
    if (n.condition) n.condition->accept(*this);
    m_table.push_scope();
    visit_stmt_list(n.body);
    m_table.pop_scope();
}

void SemanticAnalyzer::visit(DoWhileNode& n) {
    m_table.push_scope();
    visit_stmt_list(n.body);
    m_table.pop_scope();
    if (n.condition) n.condition->accept(*this);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Other statements
// ─────────────────────────────────────────────────────────────────────────────
void SemanticAnalyzer::visit(CommentNode&) {
    // Comments carry no semantic meaning
}

void SemanticAnalyzer::visit(ReturnNode& n) {
    if (n.value) n.value->accept(*this);
}

// noReturn()  /  add(1, 2) used as statement
void SemanticAnalyzer::visit(FuncCallStmtNode& n) {
    const std::string& name = n.name.lexeme;
    Symbol* sym = m_table.lookup(name);
    if (!sym) {
        error("UndeclaredFunction: '" + name + "' was not declared before call",
              n.name.line, n.name.column);
    } else if (sym->kind != SymbolKind::Function) {
        error("NotAFunction: '" + name + "' is not a function",
              n.name.line, n.name.column);
    }
    for (auto& arg : n.args) arg->accept(*this);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Expressions
// ─────────────────────────────────────────────────────────────────────────────
void SemanticAnalyzer::visit(LiteralNode&) {
    // Nothing to check — literals are always valid
}

void SemanticAnalyzer::visit(IdentifierNode& n) {
    const std::string& name = n.token.lexeme;
    if (!m_table.lookup(name)) {
        error("UndeclaredVariable: '" + name + "' was not declared before use",
              n.token.line, n.token.column);
    }
}

void SemanticAnalyzer::visit(BinaryExprNode& n) {
    // 1. Visit children first to catch deeply nested errors
    n.left->accept(*this);
    n.right->accept(*this);

    // 2. Infer the types of the left and right expressions
    ValueType left_type = infer_expr_type(*n.left);
    ValueType right_type = infer_expr_type(*n.right);

    // If either is unknown (perhaps due to a previous undeclared variable), 
    // bail out to prevent cascading dummy errors.
    if (left_type == ValueType::Unknown || right_type == ValueType::Unknown) {
        return; 
    }

    // 3. Perform Type Verification
    // Assuming your token operator can be checked via lexeme (e.g., "+") or kind (e.g., TokenKind::OP_PLUS)
    std::string op_str = n.op.lexeme; 

    if (op_str == "+") {
        // Prevent String + Non-String and Non-String + String
        if (left_type == ValueType::String && right_type != ValueType::String) {
            error("TypeError: Cannot concatenate String with a different type.", n.op.line, n.op.column);
        } else if (left_type != ValueType::String && right_type == ValueType::String) {
            error("TypeError: Cannot add a different type to a String.", n.op.line, n.op.column);
        }
    } 
    else if (op_str == "-" || op_str == "*" || op_str == "/") {
        // Prevent math operations on strings entirely
        if (left_type == ValueType::String || right_type == ValueType::String) {
            error("TypeError: Invalid arithmetic operation ('" + op_str + "') on String.", n.op.line, n.op.column);
        }
        
        // Add boolean checks (e.g., true * 5 is usually invalid)
        if (left_type == ValueType::Bool || right_type == ValueType::Bool) {
            error("TypeError: Invalid arithmetic operation on Boolean.", n.op.line, n.op.column);
        }
    }
    // You can add logic for relational operators (==, !=, <, >) here too
}

void SemanticAnalyzer::visit(UnaryExprNode& n) {
    n.operand->accept(*this);
}

void SemanticAnalyzer::visit(NotExprNode& n) {
    n.operand->accept(*this);
}

void SemanticAnalyzer::visit(FuncCallExprNode& n) {
    const std::string& name = n.name.lexeme;
    Symbol* sym = m_table.lookup(name);
    if (!sym) {
        error("UndeclaredFunction: '" + name + "' was not declared before call",
              n.name.line, n.name.column);
    } else if (sym->kind != SymbolKind::Function) {
        error("NotAFunction: '" + name + "' is not a function",
              n.name.line, n.name.column);
    }
    for (auto& arg : n.args) arg->accept(*this);
}
