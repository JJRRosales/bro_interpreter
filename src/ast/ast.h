#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../lexer/token.h"

// ─── Forward declarations ────────────────────────────────────────────────────
struct ProgramNode;
struct CommentNode;
struct VarDeclNode;
struct ConstDeclNode;
struct AssignNode;
struct UnaryStmtNode;
struct FuncCallStmtNode;
struct IfNode;
struct ForNode;
struct WhileNode;
struct DoWhileNode;
struct FuncDeclNode;
struct ReturnNode;
struct LiteralNode;
struct IdentifierNode;
struct BinaryExprNode;
struct UnaryExprNode;
struct NotExprNode;
struct FuncCallExprNode;

// ─── Visitor interface ───────────────────────────────────────────────────────
struct ASTVisitor {
    virtual ~ASTVisitor() = default;
    virtual void visit(ProgramNode&)      = 0;
    virtual void visit(CommentNode&)      = 0;
    virtual void visit(VarDeclNode&)      = 0;
    virtual void visit(ConstDeclNode&)    = 0;
    virtual void visit(AssignNode&)       = 0;
    virtual void visit(UnaryStmtNode&)    = 0;
    virtual void visit(FuncCallStmtNode&) = 0;
    virtual void visit(IfNode&)           = 0;
    virtual void visit(ForNode&)          = 0;
    virtual void visit(WhileNode&)        = 0;
    virtual void visit(DoWhileNode&)      = 0;
    virtual void visit(FuncDeclNode&)     = 0;
    virtual void visit(ReturnNode&)       = 0;
    virtual void visit(LiteralNode&)      = 0;
    virtual void visit(IdentifierNode&)   = 0;
    virtual void visit(BinaryExprNode&)   = 0;
    virtual void visit(UnaryExprNode&)    = 0;
    virtual void visit(NotExprNode&)      = 0;
    virtual void visit(FuncCallExprNode&) = 0;
};

// ─── Base nodes ──────────────────────────────────────────────────────────────
struct ASTNode {
    int line = 0;
    int col  = 0;
    virtual ~ASTNode() = default;
    virtual void        accept(ASTVisitor& v) = 0;
    virtual std::string to_string(int indent = 0) const = 0;
protected:
    static std::string ind(int n) { return std::string(n * 2, ' '); }
};

struct StmtNode : ASTNode {};
struct ExprNode : ASTNode {};

// ─────────────────────────────────────────────────────────────────────────────
//  Expression nodes
// ─────────────────────────────────────────────────────────────────────────────

// A literal value: int, float, string, bool, hex, binary, sci, imaginary, ghosted
struct LiteralNode : ExprNode {
    Token token;
    explicit LiteralNode(Token t) : token(std::move(t)) { line = token.line; col = token.column; }
    void accept(ASTVisitor& v) override { v.visit(*this); }
    std::string to_string(int indent = 0) const override;
};

// A bare identifier used as an expression: x, result, ...
struct IdentifierNode : ExprNode {
    Token token;
    explicit IdentifierNode(Token t) : token(std::move(t)) { line = token.line; col = token.column; }
    void accept(ASTVisitor& v) override { v.visit(*this); }
    std::string to_string(int indent = 0) const override;
};

// Binary expression: left op right  (arithmetic, relational, logical)
struct BinaryExprNode : ExprNode {
    Token                    op;
    std::unique_ptr<ExprNode> left;
    std::unique_ptr<ExprNode> right;
    BinaryExprNode(Token op, std::unique_ptr<ExprNode> l, std::unique_ptr<ExprNode> r)
        : op(std::move(op)), left(std::move(l)), right(std::move(r))
    { line = this->op.line; col = this->op.column; }
    void accept(ASTVisitor& v) override { v.visit(*this); }
    std::string to_string(int indent = 0) const override;
};

// Unary minus: -operand
struct UnaryExprNode : ExprNode {
    Token                    op;
    std::unique_ptr<ExprNode> operand;
    UnaryExprNode(Token op, std::unique_ptr<ExprNode> operand)
        : op(std::move(op)), operand(std::move(operand))
    { line = this->op.line; col = this->op.column; }
    void accept(ASTVisitor& v) override { v.visit(*this); }
    std::string to_string(int indent = 0) const override;
};

// Logical NOT: nah(expr)
struct NotExprNode : ExprNode {
    std::unique_ptr<ExprNode> operand;
    explicit NotExprNode(int ln, std::unique_ptr<ExprNode> op)
        : operand(std::move(op)) { line = ln; }
    void accept(ASTVisitor& v) override { v.visit(*this); }
    std::string to_string(int indent = 0) const override;
};

// Function call used as an expression: add(a, b)
struct FuncCallExprNode : ExprNode {
    Token                                  name;
    std::vector<std::unique_ptr<ExprNode>> args;
    FuncCallExprNode(Token name, std::vector<std::unique_ptr<ExprNode>> args)
        : name(std::move(name)), args(std::move(args))
    { line = this->name.line; col = this->name.column; }
    void accept(ASTVisitor& v) override { v.visit(*this); }
    std::string to_string(int indent = 0) const override;
};

// ─────────────────────────────────────────────────────────────────────────────
//  Statement nodes
// ─────────────────────────────────────────────────────────────────────────────

// Root of the entire program
struct ProgramNode : ASTNode {
    std::vector<std::unique_ptr<StmtNode>> stmts;
    void accept(ASTVisitor& v) override { v.visit(*this); }
    std::string to_string(int indent = 0) const override;
};

// A comment token (single-line yap or multi-line yapping)
struct CommentNode : StmtNode {
    Token token;
    explicit CommentNode(Token t) : token(std::move(t)) { line = token.line; col = token.column; }
    void accept(ASTVisitor& v) override { v.visit(*this); }
    std::string to_string(int indent = 0) const override;
};

// Variable declaration / assignment: id = expr
// Used for both first-use declaration and subsequent assignment.
struct VarDeclNode : StmtNode {
    Token                    name;
    std::unique_ptr<ExprNode> value;
    VarDeclNode(Token n, std::unique_ptr<ExprNode> v)
        : name(std::move(n)), value(std::move(v))
    { line = name.line; col = name.column; }
    void accept(ASTVisitor& v) override { v.visit(*this); }
    std::string to_string(int indent = 0) const override;
};

// Constant declaration: lockedIn id = expr
struct ConstDeclNode : StmtNode {
    Token                    name;
    std::unique_ptr<ExprNode> value;
    ConstDeclNode(Token n, std::unique_ptr<ExprNode> v)
        : name(std::move(n)), value(std::move(v))
    { line = name.line; col = name.column; }
    void accept(ASTVisitor& v) override { v.visit(*this); }
    std::string to_string(int indent = 0) const override;
};

// Compound assignment: id += expr, id -= expr, id *= expr, id /= expr
struct AssignNode : StmtNode {
    Token                    name;
    Token                    op;   // PLUS_ASSIGN | MINUS_ASSIGN | MULT_ASSIGN | DIV_ASSIGN
    std::unique_ptr<ExprNode> value;
    AssignNode(Token n, Token op, std::unique_ptr<ExprNode> v)
        : name(std::move(n)), op(std::move(op)), value(std::move(v))
    { line = name.line; col = name.column; }
    void accept(ASTVisitor& v) override { v.visit(*this); }
    std::string to_string(int indent = 0) const override;
};

// Increment / decrement statement: id++, id--
struct UnaryStmtNode : StmtNode {
    Token name;
    Token op;   // INC | DEC
    UnaryStmtNode(Token n, Token op) : name(std::move(n)), op(std::move(op))
    { line = name.line; col = name.column; }
    void accept(ASTVisitor& v) override { v.visit(*this); }
    std::string to_string(int indent = 0) const override;
};

// Function call used as a statement: noReturn()
struct FuncCallStmtNode : StmtNode {
    Token                                  name;
    std::vector<std::unique_ptr<ExprNode>> args;
    FuncCallStmtNode(Token n, std::vector<std::unique_ptr<ExprNode>> args)
        : name(std::move(n)), args(std::move(args))
    { line = name.line; col = name.column; }
    void accept(ASTVisitor& v) override { v.visit(*this); }
    std::string to_string(int indent = 0) const override;
};

// One conditional branch (used by IfNode)
struct IfBranch {
    std::unique_ptr<ExprNode>              condition;
    std::vector<std::unique_ptr<StmtNode>> body;
};

// checkDis (...) { } whatAbout (...) { } ... otherwise { }
struct IfNode : StmtNode {
    std::vector<IfBranch>                  branches;   // [0]=checkDis, [1..]=whatAbout
    std::vector<std::unique_ptr<StmtNode>> else_body;
    bool                                   has_else = false;
    void accept(ASTVisitor& v) override { v.visit(*this); }
    std::string to_string(int indent = 0) const override;
};

// cooking (init; cond; update) { body }
struct ForNode : StmtNode {
    std::unique_ptr<StmtNode> init;       // VarDeclNode or ConstDeclNode, nullable
    std::unique_ptr<ExprNode> condition;
    std::unique_ptr<StmtNode> update;     // VarDeclNode, AssignNode, or UnaryStmtNode, nullable
    std::vector<std::unique_ptr<StmtNode>> body;
    void accept(ASTVisitor& v) override { v.visit(*this); }
    std::string to_string(int indent = 0) const override;
};

// cook (cond) { body }
struct WhileNode : StmtNode {
    std::unique_ptr<ExprNode>              condition;
    std::vector<std::unique_ptr<StmtNode>> body;
    void accept(ASTVisitor& v) override { v.visit(*this); }
    std::string to_string(int indent = 0) const override;
};

// lets { body } cook (cond)
struct DoWhileNode : StmtNode {
    std::vector<std::unique_ptr<StmtNode>> body;
    std::unique_ptr<ExprNode>              condition;
    void accept(ASTVisitor& v) override { v.visit(*this); }
    std::string to_string(int indent = 0) const override;
};

// letHimCook name(params) { body }
struct FuncDeclNode : StmtNode {
    Token              name;
    std::vector<Token> params;
    std::vector<std::unique_ptr<StmtNode>> body;
    FuncDeclNode(Token n, std::vector<Token> p, std::vector<std::unique_ptr<StmtNode>> b)
        : name(std::move(n)), params(std::move(p)), body(std::move(b))
    { line = name.line; col = name.column; }
    void accept(ASTVisitor& v) override { v.visit(*this); }
    std::string to_string(int indent = 0) const override;
};

// sendDat [expr];
struct ReturnNode : StmtNode {
    std::unique_ptr<ExprNode> value;  // nullptr if no return value
    explicit ReturnNode(std::unique_ptr<ExprNode> v) : value(std::move(v)) {}
    void accept(ASTVisitor& v) override { v.visit(*this); }
    std::string to_string(int indent = 0) const override;
};
