#include "interpreter.h"
#include <iostream>

Interpreter::Interpreter() {
    global_env = std::make_shared<Environment>();
    current_env = global_env;
}

void Interpreter::interpret(ProgramNode& program) {
    try {
        program.accept(*this);
    } catch (const RuntimeError& e) {
        std::cerr << "[bro] Runtime Error [line " << e.line << ", col " << e.col 
                  << "]: " << e.what() << "\n";
    }
}

void Interpreter::dump_globals() const {
    global_env->dump_globals();
}

RuntimeValue Interpreter::evaluate(ExprNode& expr) {
    expr.accept(*this);
    return last_value;
}

void Interpreter::execute(StmtNode& stmt) {
    stmt.accept(*this);
}

void Interpreter::execute_block(const std::vector<std::unique_ptr<StmtNode>>& stmts, std::shared_ptr<Environment> env) {
    auto previous = current_env;
    try {
        current_env = env;
        for (const auto& stmt : stmts) {
            execute(*stmt);
        }
        current_env = previous;
    } catch (...) {
        current_env = previous;
        throw;
    }
}

RuntimeValue Interpreter::call_print(const std::vector<RuntimeValue>& args, int line, int col) {
    for (size_t i = 0; i < args.size(); ++i) {
        std::cout << args[i].to_string();
        if (i < args.size() - 1) std::cout << " ";
    }
    std::cout << "\n";
    return RuntimeValue(); // null
}

// ─── ASTVisitor Interface ───────────────────────────────────────────────────

void Interpreter::visit(ProgramNode& n) {
    for (const auto& stmt : n.stmts) {
        execute(*stmt);
    }
}

void Interpreter::visit(CommentNode& n) {
    // Do nothing
}

void Interpreter::visit(VarDeclNode& n) {
    RuntimeValue value;
    if (n.value) {
        value = evaluate(*n.value);
    }
    current_env->define(n.name.lexeme, value);
}

void Interpreter::visit(ConstDeclNode& n) {
    RuntimeValue value;
    if (n.value) {
        value = evaluate(*n.value);
    }
    current_env->define(n.name.lexeme, value);
}

void Interpreter::visit(AssignNode& n) {
    RuntimeValue value = evaluate(*n.value);
    const std::string& name = n.name.lexeme;

    RuntimeValue current_val;
    if (!current_env->get(name, current_val)) {
        throw RuntimeError("Variable '" + name + "' not found.", n.name.line, n.name.column);
    }

    if (n.op.kind != TokenKind::ASSIGN) {
        if (!current_val.is_int() && !current_val.is_float()) {
            throw RuntimeError("Compound assignment requires numeric types.", n.op.line, n.op.column);
        }
        if (!value.is_int() && !value.is_float()) {
            throw RuntimeError("Compound assignment requires numeric types.", n.op.line, n.op.column);
        }
        
        bool is_float = current_val.is_float() || value.is_float();
        double d_cur = current_val.is_float() ? current_val.as_float() : (double)current_val.as_int();
        double d_val = value.is_float() ? value.as_float() : (double)value.as_int();

        if (n.op.kind == TokenKind::PLUS_ASSIGN) d_cur += d_val;
        else if (n.op.kind == TokenKind::MINUS_ASSIGN) d_cur -= d_val;
        else if (n.op.kind == TokenKind::MULT_ASSIGN) d_cur *= d_val;
        else if (n.op.kind == TokenKind::DIV_ASSIGN) {
            if (d_val == 0) throw RuntimeError("Division by zero.", n.op.line, n.op.column);
            d_cur /= d_val;
        }

        if (is_float) value = RuntimeValue(d_cur);
        else value = RuntimeValue((long long)d_cur);
    }

    current_env->assign(name, value);
}

void Interpreter::visit(UnaryStmtNode& n) {
    const std::string& name = n.name.lexeme;
    RuntimeValue current_val;
    if (!current_env->get(name, current_val)) {
        throw RuntimeError("Variable '" + name + "' not found.", n.name.line, n.name.column);
    }

    if (!current_val.is_int() && !current_val.is_float()) {
        throw RuntimeError("Increment/Decrement requires numeric type.", n.name.line, n.name.column);
    }

    if (current_val.is_float()) {
        double d = current_val.as_float();
        if (n.op.kind == TokenKind::INC) d += 1.0;
        else d -= 1.0;
        current_env->assign(name, RuntimeValue(d));
    } else {
        long long i = current_val.as_int();
        if (n.op.kind == TokenKind::INC) i += 1;
        else i -= 1;
        current_env->assign(name, RuntimeValue(i));
    }
}

void Interpreter::visit(FuncCallStmtNode& n) {
    const std::string& name = n.name.lexeme;
    
    std::vector<RuntimeValue> args;
    for (const auto& arg : n.args) {
        args.push_back(evaluate(*arg));
    }

    if (name == "print") {
        call_print(args, n.name.line, n.name.column);
        return;
    }

    auto it = functions.find(name);
    if (it == functions.end()) {
        throw RuntimeError("Undefined function '" + name + "'.", n.name.line, n.name.column);
    }

    FuncDeclNode* func = it->second;
    if (args.size() != func->params.size()) {
        throw RuntimeError("Incorrect number of arguments for '" + name + "'.", n.name.line, n.name.column);
    }

    auto func_env = std::make_shared<Environment>(global_env);
    for (size_t i = 0; i < args.size(); ++i) {
        func_env->define(func->params[i].lexeme, args[i]);
    }

    try {
        execute_block(func->body, func_env);
    } catch (const ReturnException& ret) {
        // Function returning a value (not expected here, but catch anyway)
    }
}

void Interpreter::visit(IfNode& n) {
    for (const auto& branch : n.branches) {
        bool condition = true;
        if (branch.condition) {
            condition = evaluate(*branch.condition).is_truthy();
        }
        if (condition) {
            execute_block(branch.body, std::make_shared<Environment>(current_env));
            return;
        }
    }
    if (n.has_else) {
        execute_block(n.else_body, std::make_shared<Environment>(current_env));
    }
}

void Interpreter::visit(ForNode& n) {
    auto env = std::make_shared<Environment>(current_env);
    auto previous = current_env;
    try {
        current_env = env;
        if (n.init) execute(*n.init);
        while (true) {
            if (n.condition) {
                if (!evaluate(*n.condition).is_truthy()) break;
            }
            execute_block(n.body, std::make_shared<Environment>(current_env));
            if (n.update) execute(*n.update);
        }
        current_env = previous;
    } catch (...) {
        current_env = previous;
        throw;
    }
}

void Interpreter::visit(WhileNode& n) {
    while (true) {
        if (n.condition && !evaluate(*n.condition).is_truthy()) {
            break;
        }
        execute_block(n.body, std::make_shared<Environment>(current_env));
    }
}

void Interpreter::visit(DoWhileNode& n) {
    do {
        execute_block(n.body, std::make_shared<Environment>(current_env));
    } while (n.condition && evaluate(*n.condition).is_truthy());
}

void Interpreter::visit(FuncDeclNode& n) {
    functions[n.name.lexeme] = &n;
}

void Interpreter::visit(ReturnNode& n) {
    RuntimeValue val;
    if (n.value) val = evaluate(*n.value);
    throw ReturnException(val);
}

void Interpreter::visit(LiteralNode& n) {
    switch (n.token.kind) {
        case TokenKind::V_INT:       last_value = RuntimeValue(std::stoll(n.token.lexeme)); break;
        case TokenKind::V_FLOAT:     last_value = RuntimeValue(std::stod(n.token.lexeme)); break;
        case TokenKind::V_STRING:    last_value = RuntimeValue(n.token.lexeme); break;
        case TokenKind::KW_LEGIT:    last_value = RuntimeValue(true); break;
        case TokenKind::KW_FAKE:     last_value = RuntimeValue(false); break;
        case TokenKind::KW_GHOSTED:  last_value = RuntimeValue(); break;
        // Basic fallback for other literal types like Hex, Binary, etc. 
        // For a full implementation, these would be converted to int.
        default:                     last_value = RuntimeValue(); break;
    }
}

void Interpreter::visit(IdentifierNode& n) {
    if (!current_env->get(n.token.lexeme, last_value)) {
        throw RuntimeError("Variable '" + n.token.lexeme + "' not found.", n.token.line, n.token.column);
    }
}

void Interpreter::visit(BinaryExprNode& n) {
    RuntimeValue left = evaluate(*n.left);
    RuntimeValue right = evaluate(*n.right);

    TokenKind op = n.op.kind;

    if (op == TokenKind::PLUS) {
        if (left.is_string() || right.is_string()) {
            last_value = RuntimeValue(left.to_string() + right.to_string());
            return;
        }
    }

    if (left.is_bool() || right.is_bool() || left.is_string() || right.is_string() || left.is_null() || right.is_null()) {
        // Relational and logical for mixed/other types
        if (op == TokenKind::EQ) last_value = RuntimeValue(left.to_string() == right.to_string());
        else if (op == TokenKind::NEQ) last_value = RuntimeValue(left.to_string() != right.to_string());
        else throw RuntimeError("Invalid operation for types.", n.op.line, n.op.column);
        return;
    }

    bool is_float = left.is_float() || right.is_float();
    double l_f = left.is_float() ? left.as_float() : (double)left.as_int();
    double r_f = right.is_float() ? right.as_float() : (double)right.as_int();

    switch (op) {
        case TokenKind::PLUS: last_value = is_float ? RuntimeValue(l_f + r_f) : RuntimeValue(left.as_int() + right.as_int()); break;
        case TokenKind::MINUS: last_value = is_float ? RuntimeValue(l_f - r_f) : RuntimeValue(left.as_int() - right.as_int()); break;
        case TokenKind::MULT: last_value = is_float ? RuntimeValue(l_f * r_f) : RuntimeValue(left.as_int() * right.as_int()); break;
        case TokenKind::DIV:
            if (r_f == 0) throw RuntimeError("Division by zero.", n.op.line, n.op.column);
            last_value = is_float ? RuntimeValue(l_f / r_f) : RuntimeValue(left.as_int() / right.as_int());
            break;
        case TokenKind::MOD:
            if (right.as_int() == 0) throw RuntimeError("Modulo by zero.", n.op.line, n.op.column);
            last_value = RuntimeValue(left.as_int() % right.as_int());
            break;
        case TokenKind::GT: last_value = RuntimeValue(l_f > r_f); break;
        case TokenKind::GTE: last_value = RuntimeValue(l_f >= r_f); break;
        case TokenKind::LT: last_value = RuntimeValue(l_f < r_f); break;
        case TokenKind::LTE: last_value = RuntimeValue(l_f <= r_f); break;
        case TokenKind::EQ: last_value = RuntimeValue(l_f == r_f); break;
        case TokenKind::NEQ: last_value = RuntimeValue(l_f != r_f); break;
        default: throw RuntimeError("Unknown binary operator.", n.op.line, n.op.column);
    }
}

void Interpreter::visit(UnaryExprNode& n) {
    RuntimeValue right = evaluate(*n.operand);
    if (n.op.kind == TokenKind::MINUS) {
        if (right.is_int()) last_value = RuntimeValue(-right.as_int());
        else if (right.is_float()) last_value = RuntimeValue(-right.as_float());
        else throw RuntimeError("Invalid operand for unary minus.", n.op.line, n.op.column);
    } else {
        last_value = right;
    }
}

void Interpreter::visit(NotExprNode& n) {
    RuntimeValue right = evaluate(*n.operand);
    last_value = RuntimeValue(!right.is_truthy());
}

void Interpreter::visit(FuncCallExprNode& n) {
    // Similar to FuncCallStmtNode
    const std::string& name = n.name.lexeme;
    
    std::vector<RuntimeValue> args;
    for (const auto& arg : n.args) {
        args.push_back(evaluate(*arg));
    }

    if (name == "print") {
        last_value = call_print(args, n.name.line, n.name.column);
        return;
    }
    
    auto it = functions.find(name);
    if (it == functions.end()) {
        throw RuntimeError("Undefined function '" + name + "'.", n.name.line, n.name.column);
    }

    FuncDeclNode* func = it->second;
    if (args.size() != func->params.size()) {
        throw RuntimeError("Incorrect number of arguments for '" + name + "'.", n.name.line, n.name.column);
    }

    auto func_env = std::make_shared<Environment>(global_env);
    for (size_t i = 0; i < args.size(); ++i) {
        func_env->define(func->params[i].lexeme, args[i]);
    }

    try {
        execute_block(func->body, func_env);
        last_value = RuntimeValue();
    } catch (const ReturnException& ret) {
        last_value = ret.value;
    }
}
