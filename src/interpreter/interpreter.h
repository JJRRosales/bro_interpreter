#pragma once

#include "../ast/ast.h"
#include "environment.h"
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// Exception to handle runtime errors gracefully
struct RuntimeError : public std::runtime_error {
    int line;
    int col;
    RuntimeError(const std::string& msg, int line, int col) 
        : std::runtime_error(msg), line(line), col(col) {}
};

// Exception to handle the `sendDat` (return) control flow
struct ReturnException {
    RuntimeValue value;
    ReturnException(RuntimeValue v) : value(std::move(v)) {}
};

class Interpreter : public ASTVisitor {
public:
    Interpreter();
    void interpret(ProgramNode& program);

    // Prints global variables
    void dump_globals() const;

private:
    std::shared_ptr<Environment> global_env;
    std::shared_ptr<Environment> current_env;
    
    // Functions mapped by name
    std::unordered_map<std::string, FuncDeclNode*> functions;
    
    // Holds the result of the last evaluated expression
    RuntimeValue last_value;

    // Helpers
    RuntimeValue evaluate(ExprNode& expr);
    void execute(StmtNode& stmt);
    void execute_block(const std::vector<std::unique_ptr<StmtNode>>& stmts, std::shared_ptr<Environment> env);

    // Native functions
    RuntimeValue call_print(const std::vector<RuntimeValue>& args, int line, int col);

    // ASTVisitor Interface
    void visit(ProgramNode& n) override;
    void visit(CommentNode& n) override;
    void visit(VarDeclNode& n) override;
    void visit(ConstDeclNode& n) override;
    void visit(AssignNode& n) override;
    void visit(UnaryStmtNode& n) override;
    void visit(FuncCallStmtNode& n) override;
    void visit(IfNode& n) override;
    void visit(ForNode& n) override;
    void visit(WhileNode& n) override;
    void visit(DoWhileNode& n) override;
    void visit(FuncDeclNode& n) override;
    void visit(ReturnNode& n) override;
    void visit(LiteralNode& n) override;
    void visit(IdentifierNode& n) override;
    void visit(BinaryExprNode& n) override;
    void visit(UnaryExprNode& n) override;
    void visit(NotExprNode& n) override;
    void visit(FuncCallExprNode& n) override;
};
