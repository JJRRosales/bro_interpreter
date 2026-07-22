#include "ast.h"
#include <sstream>

// ─── Helper ──────────────────────────────────────────────────────────────────
static std::string prefix(int indent) { return std::string(indent * 2, ' '); }

// ─── Expression nodes ────────────────────────────────────────────────────────

std::string LiteralNode::to_string(int indent) const {
    return prefix(indent) + "[Literal:" + token.kind_to_string() + "] \"" + token.lexeme + "\"";
}

std::string IdentifierNode::to_string(int indent) const {
    return prefix(indent) + "[Identifier] " + token.lexeme;
}

std::string BinaryExprNode::to_string(int indent) const {
    return prefix(indent) + "[BinaryExpr:" + op.kind_to_string() + "]\n"
        + left->to_string(indent + 1) + "\n"
        + right->to_string(indent + 1);
}

std::string UnaryExprNode::to_string(int indent) const {
    return prefix(indent) + "[UnaryExpr:" + op.kind_to_string() + "]\n"
        + operand->to_string(indent + 1);
}

std::string NotExprNode::to_string(int indent) const {
    return prefix(indent) + "[Not]\n"
        + operand->to_string(indent + 1);
}

std::string FuncCallExprNode::to_string(int indent) const {
    std::string s = prefix(indent) + "[FuncCallExpr] " + name.lexeme + "(";
    s += std::to_string(args.size()) + " args)";
    for (const auto& a : args) {
        s += "\n" + a->to_string(indent + 1);
    }
    return s;
}

// ─── Statement nodes ─────────────────────────────────────────────────────────

std::string ProgramNode::to_string(int indent) const {
    std::string s = prefix(indent) + "[Program] (" + std::to_string(stmts.size()) + " stmts)";
    for (const auto& st : stmts) {
        s += "\n" + st->to_string(indent + 1);
    }
    return s;
}

std::string CommentNode::to_string(int indent) const {
    // Trim whitespace from lexeme for display
    std::string lex = token.lexeme;
    if (lex.size() > 40) lex = lex.substr(0, 37) + "...";
    return prefix(indent) + "[Comment] \"" + lex + "\"";
}

std::string VarDeclNode::to_string(int indent) const {
    return prefix(indent) + "[VarDecl] " + name.lexeme + "\n"
        + value->to_string(indent + 1);
}

std::string ConstDeclNode::to_string(int indent) const {
    return prefix(indent) + "[ConstDecl] " + name.lexeme + "\n"
        + value->to_string(indent + 1);
}

std::string AssignNode::to_string(int indent) const {
    return prefix(indent) + "[Assign:" + op.kind_to_string() + "] " + name.lexeme + "\n"
        + value->to_string(indent + 1);
}

std::string UnaryStmtNode::to_string(int indent) const {
    return prefix(indent) + "[UnaryStmt:" + op.kind_to_string() + "] " + name.lexeme;
}

std::string FuncCallStmtNode::to_string(int indent) const {
    std::string s = prefix(indent) + "[FuncCallStmt] " + name.lexeme
                  + "(" + std::to_string(args.size()) + " args)";
    for (const auto& a : args) {
        s += "\n" + a->to_string(indent + 1);
    }
    return s;
}

std::string IfNode::to_string(int indent) const {
    std::string s;
    bool first = true;
    for (const auto& branch : branches) {
        s += prefix(indent) + (first ? "[If]" : "[ElseIf]") + "\n";
        s += branch.condition->to_string(indent + 1) + "\n";
        s += prefix(indent + 1) + "[Body]\n";
        for (const auto& st : branch.body) {
            s += st->to_string(indent + 2) + "\n";
        }
        first = false;
    }
    if (has_else) {
        s += prefix(indent) + "[Else]\n";
        for (const auto& st : else_body) {
            s += st->to_string(indent + 1) + "\n";
        }
    }
    if (!s.empty() && s.back() == '\n') s.pop_back();
    return s;
}

std::string ForNode::to_string(int indent) const {
    std::string s = prefix(indent) + "[For]\n";
    s += prefix(indent + 1) + "[Init]\n";
    if (init) s += init->to_string(indent + 2) + "\n";
    else       s += prefix(indent + 2) + "(empty)\n";
    s += prefix(indent + 1) + "[Condition]\n";
    if (condition) s += condition->to_string(indent + 2) + "\n";
    s += prefix(indent + 1) + "[Update]\n";
    if (update) s += update->to_string(indent + 2) + "\n";
    else         s += prefix(indent + 2) + "(empty)\n";
    s += prefix(indent + 1) + "[Body]\n";
    for (const auto& st : body) {
        s += st->to_string(indent + 2) + "\n";
    }
    if (!s.empty() && s.back() == '\n') s.pop_back();
    return s;
}

std::string WhileNode::to_string(int indent) const {
    std::string s = prefix(indent) + "[While]\n";
    s += prefix(indent + 1) + "[Condition]\n";
    s += condition->to_string(indent + 2) + "\n";
    s += prefix(indent + 1) + "[Body]\n";
    for (const auto& st : body) {
        s += st->to_string(indent + 2) + "\n";
    }
    if (!s.empty() && s.back() == '\n') s.pop_back();
    return s;
}

std::string DoWhileNode::to_string(int indent) const {
    std::string s = prefix(indent) + "[DoWhile]\n";
    s += prefix(indent + 1) + "[Body]\n";
    for (const auto& st : body) {
        s += st->to_string(indent + 2) + "\n";
    }
    s += prefix(indent + 1) + "[Condition]\n";
    s += condition->to_string(indent + 2);
    return s;
}

std::string FuncDeclNode::to_string(int indent) const {
    std::string s = prefix(indent) + "[FuncDecl] " + name.lexeme + "(";
    for (size_t i = 0; i < params.size(); ++i) {
        if (i > 0) s += ", ";
        s += params[i].lexeme;
    }
    s += ")\n";
    s += prefix(indent + 1) + "[Body]\n";
    for (const auto& st : body) {
        s += st->to_string(indent + 2) + "\n";
    }
    if (!s.empty() && s.back() == '\n') s.pop_back();
    return s;
}

std::string ReturnNode::to_string(int indent) const {
    if (!value) {
        return prefix(indent) + "[Return] (void)";
    }
    return prefix(indent) + "[Return]\n" + value->to_string(indent + 1);
}
