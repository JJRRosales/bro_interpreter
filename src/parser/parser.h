#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../lexer/token.h"
#include "../ast/ast.h"

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);

    // Build and return the AST for the entire program.
    // Throws std::runtime_error on syntax errors.
    std::unique_ptr<ProgramNode> parse();

private:
    void validate_syntax();
    std::vector<Token> m_tokens;
    size_t             m_pos = 0;

    // ── Token helpers ─────────────────────────────────────────────────────
    const Token& peek()  const;
    Token        advance();
    bool         check(TokenKind k) const;
    bool         match(TokenKind k);
    Token        expect(TokenKind k, const std::string& msg);
    bool         is_at_end() const;

    // ── Predicate sets ────────────────────────────────────────────────────
    bool is_stmt_start()     const;
    bool is_arith_op()       const;
    bool is_rel_op()         const;
    bool is_log_op()         const;
    bool is_compound_assign() const;
    bool is_val_start()      const;

    // ── Top-level ─────────────────────────────────────────────────────────
    std::unique_ptr<ProgramNode>           parse_program();
    std::vector<std::unique_ptr<StmtNode>> parse_stmt_list();
    std::unique_ptr<StmtNode>              parse_stmt();

    // ── Statement parsers ─────────────────────────────────────────────────
    std::unique_ptr<StmtNode>    parse_id_stmt();
    std::unique_ptr<IfNode>      parse_if();
    std::unique_ptr<ForNode>     parse_for();
    std::unique_ptr<WhileNode>   parse_while();
    std::unique_ptr<DoWhileNode> parse_dowhile();
    std::unique_ptr<FuncDeclNode> parse_func_decl();
    std::unique_ptr<ReturnNode>  parse_return();

    // For-loop sub-parsers
    std::unique_ptr<StmtNode>    parse_for_init();
    std::unique_ptr<StmtNode>    parse_for_update();

    // Param / arg lists
    std::vector<Token>                     parse_param_list();
    std::vector<std::unique_ptr<ExprNode>> parse_arg_list();

    // ── Expression parsers ────────────────────────────────────────────────
    std::unique_ptr<ExprNode> parse_expr();
    std::unique_ptr<ExprNode> parse_expr_tail(std::unique_ptr<ExprNode> left);
    std::unique_ptr<ExprNode> parse_bool_expr();
    std::unique_ptr<ExprNode> parse_bool_expr_factor(std::unique_ptr<ExprNode> left);
    std::unique_ptr<ExprNode> parse_bool_tail(std::unique_ptr<ExprNode> left);
    std::unique_ptr<ExprNode> parse_arith_expr();
    std::unique_ptr<ExprNode> parse_arith_tail(std::unique_ptr<ExprNode> left);
    std::unique_ptr<ExprNode> parse_unary_expr();
    std::unique_ptr<ExprNode> parse_varc();
    std::unique_ptr<ExprNode> parse_val();
};