#pragma once

#include <string>
#include <vector>
#include "../ast/ast.h"
#include "symbol_table.h"

// ─────────────────────────────────────────────────────────────────────────────
//  Semantic error descriptor
// ─────────────────────────────────────────────────────────────────────────────
struct SemanticError {
    std::string message;
    int         line;
    int         col;
};

// ─────────────────────────────────────────────────────────────────────────────
//  SemanticAnalyzer
//
//  Traverses the AST using the Visitor pattern. Builds the symbol table,
//  infers types, and collects semantic errors without stopping on the first
//  one — all errors are reported after the full traversal.
//
//  Errors detected:
//    - UndeclaredVariable   : identifier used before declaration
//    - RedeclaredVariable   : re-declaring a name in the *same* scope
//    - AssignToConst        : assigning to a 'lockedIn' constant
//    - UndeclaredFunction   : calling a function that was never declared
// ─────────────────────────────────────────────────────────────────────────────
class SemanticAnalyzer : public ASTVisitor {
public:
    // Run the full semantic analysis pass.
    // Returns true if no errors were found.
    bool analyze(ProgramNode& program);

    bool had_errors() const { return !m_errors.empty(); }
    const std::vector<SemanticError>& errors() const { return m_errors; }

    // Print the symbol table to stdout (call after analyze())
    void dump_symbols() const { m_table.dump(); }

private:
    SymbolTable               m_table;
    std::vector<SemanticError> m_errors;
    bool                      m_in_function = false; // true while inside a func body

    // ── Helpers ───────────────────────────────────────────────────────────
    void      error(const std::string& msg, int line, int col);
    ValueType infer_literal_type(const Token& tok);
    ValueType infer_expr_type(ExprNode& expr);
    void      visit_stmt_list(std::vector<std::unique_ptr<StmtNode>>& stmts);

    // ── ASTVisitor interface ──────────────────────────────────────────────
    void visit(ProgramNode& n)      override;
    void visit(CommentNode& n)      override;
    void visit(VarDeclNode& n)      override;
    void visit(ConstDeclNode& n)    override;
    void visit(AssignNode& n)       override;
    void visit(UnaryStmtNode& n)    override;
    void visit(FuncCallStmtNode& n) override;
    void visit(IfNode& n)           override;
    void visit(ForNode& n)          override;
    void visit(WhileNode& n)        override;
    void visit(DoWhileNode& n)      override;
    void visit(FuncDeclNode& n)     override;
    void visit(ReturnNode& n)       override;
    void visit(LiteralNode& n)      override;
    void visit(IdentifierNode& n)   override;
    void visit(BinaryExprNode& n)   override;
    void visit(UnaryExprNode& n)    override;
    void visit(NotExprNode& n)      override;
    void visit(FuncCallExprNode& n) override;
};
