#include "parser.h"
#include <stdexcept>

// ─────────────────────────────────────────────────────────────────────────────
//  Constructor
// ─────────────────────────────────────────────────────────────────────────────
Parser::Parser(const std::vector<Token>& tokens)
    : m_tokens(tokens) {}

std::unique_ptr<ProgramNode> Parser::parse() {
    return parse_program();
}

// ─────────────────────────────────────────────────────────────────────────────
//  Token helpers
// ─────────────────────────────────────────────────────────────────────────────
const Token& Parser::peek() const {
    if (m_pos < m_tokens.size()) return m_tokens[m_pos];
    return m_tokens.back(); // EOF sentinel
}

Token Parser::advance() {
    if (m_pos < m_tokens.size()) return m_tokens[m_pos++];
    return m_tokens.back();
}

bool Parser::check(TokenKind k) const {
    return peek().kind == k;
}

bool Parser::match(TokenKind k) {
    if (check(k)) { advance(); return true; }
    return false;
}

Token Parser::expect(TokenKind k, const std::string& msg) {
    if (check(k)) return advance();
    const auto& t = peek();
    throw std::runtime_error(msg + " at line " + std::to_string(t.line) +
                             ", col "  + std::to_string(t.column) +
                             " (got "  + t.kind_to_string() +
                             " \""     + t.lexeme + "\")");
}

bool Parser::is_at_end() const {
    return m_pos >= m_tokens.size() || peek().kind == TokenKind::Eof;
}

// ─────────────────────────────────────────────────────────────────────────────
//  Predicate sets
// ─────────────────────────────────────────────────────────────────────────────
bool Parser::is_stmt_start() const {
    switch (peek().kind) {
        case TokenKind::COMMENT:
        case TokenKind::KW_LOCKEDIN:
        case TokenKind::ID:
        case TokenKind::KW_CHECKDIS:
        case TokenKind::KW_COOKING:
        case TokenKind::KW_LETS:
        case TokenKind::KW_COOK:
        case TokenKind::KW_SENDDAT:
        case TokenKind::KW_LETHIMCOOK:
            return true;
        default:
            return false;
    }
}

bool Parser::is_arith_op() const {
    switch (peek().kind) {
        case TokenKind::PLUS:
        case TokenKind::MINUS:
        case TokenKind::MULT:
        case TokenKind::DIV:
        case TokenKind::MOD:
            return true;
        default:
            return false;
    }
}

bool Parser::is_rel_op() const {
    switch (peek().kind) {
        case TokenKind::GT:
        case TokenKind::GTE:
        case TokenKind::LT:
        case TokenKind::LTE:
        case TokenKind::KW_SAME:
        case TokenKind::KW_AINTSAME:
            return true;
        default:
            return false;
    }
}

bool Parser::is_log_op() const {
    return check(TokenKind::KW_ANDALSO) || check(TokenKind::KW_EITHERWAY);
}

bool Parser::is_compound_assign() const {
    switch (peek().kind) {
        case TokenKind::PLUS_ASSIGN:
        case TokenKind::MINUS_ASSIGN:
        case TokenKind::MULT_ASSIGN:
        case TokenKind::DIV_ASSIGN:
            return true;
        default:
            return false;
    }
}

bool Parser::is_val_start() const {
    switch (peek().kind) {
        case TokenKind::V_INT:
        case TokenKind::V_FLOAT:
        case TokenKind::V_STRING:
        case TokenKind::V_HEX:
        case TokenKind::V_SCI:
        case TokenKind::V_BINARY:
        case TokenKind::V_IMAGINARY:
        case TokenKind::KW_LEGIT:
        case TokenKind::KW_FAKE:
        case TokenKind::KW_GHOSTED:
            return true;
        default:
            return false;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  Top-level
// ─────────────────────────────────────────────────────────────────────────────
std::unique_ptr<ProgramNode> Parser::parse_program() {
    auto program = std::make_unique<ProgramNode>();
    program->stmts = parse_stmt_list();
    if (!is_at_end()) {
        const auto& t = peek();
        throw std::runtime_error(
            "Unexpected token at line " + std::to_string(t.line) +
            ": " + t.kind_to_string() + " \"" + t.lexeme + "\"");
    }
    return program;
}

// <STMT_LIST> → <STMT> <STMT_LIST> | EPSILON
std::vector<std::unique_ptr<StmtNode>> Parser::parse_stmt_list() {
    std::vector<std::unique_ptr<StmtNode>> stmts;
    while (!is_at_end() && !check(TokenKind::RBRACE) && is_stmt_start()) {
        stmts.push_back(parse_stmt());
    }
    return stmts;
}

// <STMT> → one of the statement forms
std::unique_ptr<StmtNode> Parser::parse_stmt() {
    switch (peek().kind) {
        case TokenKind::COMMENT:
            return std::make_unique<CommentNode>(advance());

        case TokenKind::KW_LOCKEDIN: {
            advance(); // consume lockedIn
            Token name = expect(TokenKind::ID, "Expected identifier after 'lockedIn'");
            expect(TokenKind::ASSIGN, "Expected '=' after identifier in 'lockedIn' declaration");
            auto val = parse_expr();
            return std::make_unique<ConstDeclNode>(std::move(name), std::move(val));
        }

        case TokenKind::ID:
            return parse_id_stmt();

        case TokenKind::KW_CHECKDIS:
            return parse_if();

        case TokenKind::KW_COOKING:
            return parse_for();

        case TokenKind::KW_COOK:
            return parse_while();

        case TokenKind::KW_LETS:
            return parse_dowhile();

        case TokenKind::KW_LETHIMCOOK:
            return parse_func_decl();

        case TokenKind::KW_SENDDAT:
            return parse_return();

        default: {
            const auto& t = peek();
            throw std::runtime_error(
                "Unexpected token in statement at line " + std::to_string(t.line) +
                ": " + t.kind_to_string() + " \"" + t.lexeme + "\"");
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  Statement parsers
// ─────────────────────────────────────────────────────────────────────────────

// <ID_STMT_FACTOR> dispatch after consuming the leading ID
std::unique_ptr<StmtNode> Parser::parse_id_stmt() {
    Token name = expect(TokenKind::ID, "Expected identifier");

    // id = expr  →  VarDecl (first-use or reassignment)
    if (check(TokenKind::ASSIGN)) {
        advance();
        auto val = parse_expr();
        return std::make_unique<VarDeclNode>(std::move(name), std::move(val));
    }

    // id += expr  |  id -= expr  |  id *= expr  |  id /= expr
    if (is_compound_assign()) {
        Token op = advance();
        auto val = parse_expr();
        return std::make_unique<AssignNode>(std::move(name), std::move(op), std::move(val));
    }

    // id++  |  id--
    if (check(TokenKind::INC) || check(TokenKind::DEC)) {
        Token op = advance();
        return std::make_unique<UnaryStmtNode>(std::move(name), std::move(op));
    }

    // id(args)  →  function call as statement
    if (check(TokenKind::LPAREN)) {
        advance();
        auto args = parse_arg_list();
        expect(TokenKind::RPAREN, "Expected ')' after argument list");
        return std::make_unique<FuncCallStmtNode>(std::move(name), std::move(args));
    }

    throw std::runtime_error(
        "Expected '=', compound-assign, '++', '--', or '(' after identifier '" +
        name.lexeme + "' at line " + std::to_string(name.line));
}

// <IF> → checkDis (bool) { stmts } <ELSEIF>
std::unique_ptr<IfNode> Parser::parse_if() {
    expect(TokenKind::KW_CHECKDIS, "Expected 'checkDis'");
    auto node = std::make_unique<IfNode>();

    // Primary branch
    expect(TokenKind::LPAREN, "Expected '(' after 'checkDis'");
    auto cond = parse_bool_expr();
    expect(TokenKind::RPAREN, "Expected ')'");
    expect(TokenKind::LBRACE, "Expected '{'");
    auto body = parse_stmt_list();
    expect(TokenKind::RBRACE, "Expected '}'");
    node->branches.push_back({ std::move(cond), std::move(body) });

    // whatAbout branches
    while (check(TokenKind::KW_WHATABOUT)) {
        advance();
        expect(TokenKind::LPAREN, "Expected '(' after 'whatAbout'");
        auto elif_cond = parse_bool_expr();
        expect(TokenKind::RPAREN, "Expected ')'");
        expect(TokenKind::LBRACE, "Expected '{'");
        auto elif_body = parse_stmt_list();
        expect(TokenKind::RBRACE, "Expected '}'");
        node->branches.push_back({ std::move(elif_cond), std::move(elif_body) });
    }

    // otherwise
    if (check(TokenKind::KW_OTHERWISE)) {
        advance();
        expect(TokenKind::LBRACE, "Expected '{'");
        node->else_body = parse_stmt_list();
        expect(TokenKind::RBRACE, "Expected '}'");
        node->has_else = true;
    }

    return node;
}

// <FOR> → cooking (init; cond; update) { stmts }
std::unique_ptr<ForNode> Parser::parse_for() {
    expect(TokenKind::KW_COOKING, "Expected 'cooking'");
    auto node = std::make_unique<ForNode>();

    expect(TokenKind::LPAREN, "Expected '('");
    node->init = parse_for_init();
    expect(TokenKind::SEMICOLON, "Expected ';' after for-init");
    node->condition = parse_bool_expr();
    expect(TokenKind::SEMICOLON, "Expected ';' after for-condition");
    node->update = parse_for_update();
    expect(TokenKind::RPAREN, "Expected ')'");
    expect(TokenKind::LBRACE, "Expected '{'");
    node->body = parse_stmt_list();
    expect(TokenKind::RBRACE, "Expected '}'");

    return node;
}

// <WHILE> → cook (bool) { stmts }
std::unique_ptr<WhileNode> Parser::parse_while() {
    expect(TokenKind::KW_COOK, "Expected 'cook'");
    auto node = std::make_unique<WhileNode>();
    expect(TokenKind::LPAREN, "Expected '('");
    node->condition = parse_bool_expr();
    expect(TokenKind::RPAREN, "Expected ')'");
    expect(TokenKind::LBRACE, "Expected '{'");
    node->body = parse_stmt_list();
    expect(TokenKind::RBRACE, "Expected '}'");
    return node;
}

// <DOWHILE> → lets { stmts } cook (bool)
std::unique_ptr<DoWhileNode> Parser::parse_dowhile() {
    expect(TokenKind::KW_LETS, "Expected 'lets'");
    auto node = std::make_unique<DoWhileNode>();
    expect(TokenKind::LBRACE, "Expected '{'");
    node->body = parse_stmt_list();
    expect(TokenKind::RBRACE, "Expected '}'");
    expect(TokenKind::KW_COOK, "Expected 'cook' after 'lets { }'");
    expect(TokenKind::LPAREN, "Expected '('");
    node->condition = parse_bool_expr();
    expect(TokenKind::RPAREN, "Expected ')'");
    return node;
}

// <FUNC_DECL> → letHimCook id (params) { stmts }
std::unique_ptr<FuncDeclNode> Parser::parse_func_decl() {
    expect(TokenKind::KW_LETHIMCOOK, "Expected 'letHimCook'");
    Token name = expect(TokenKind::ID, "Expected function name after 'letHimCook'");
    expect(TokenKind::LPAREN, "Expected '('");
    auto params = parse_param_list();
    expect(TokenKind::RPAREN, "Expected ')'");
    expect(TokenKind::LBRACE, "Expected '{'");
    auto body = parse_stmt_list();
    expect(TokenKind::RBRACE, "Expected '}'");
    return std::make_unique<FuncDeclNode>(std::move(name), std::move(params), std::move(body));
}

// <RETURN> → sendDat [expr] ;
std::unique_ptr<ReturnNode> Parser::parse_return() {
    expect(TokenKind::KW_SENDDAT, "Expected 'sendDat'");
    std::unique_ptr<ExprNode> val;
    // EPSILON if next is SEMICOLON or EOF
    if (!check(TokenKind::SEMICOLON) && !is_at_end()) {
        val = parse_expr();
    }
    expect(TokenKind::SEMICOLON, "Expected ';' after 'sendDat'");
    return std::make_unique<ReturnNode>(std::move(val));
}

// ─────────────────────────────────────────────────────────────────────────────
//  For-loop sub-parsers
// ─────────────────────────────────────────────────────────────────────────────

// <FOR_INIT> → <DECL> | EPSILON
std::unique_ptr<StmtNode> Parser::parse_for_init() {
    if (check(TokenKind::SEMICOLON) || is_at_end()) {
        return nullptr; // EPSILON
    }
    if (check(TokenKind::KW_LOCKEDIN)) {
        advance();
        Token name = expect(TokenKind::ID, "Expected identifier");
        expect(TokenKind::ASSIGN, "Expected '='");
        auto val = parse_expr();
        return std::make_unique<ConstDeclNode>(std::move(name), std::move(val));
    }
    // ID ASSIGN EXPR
    Token name = expect(TokenKind::ID, "Expected identifier in for-init");
    expect(TokenKind::ASSIGN, "Expected '=' in for-init");
    auto val = parse_expr();
    return std::make_unique<VarDeclNode>(std::move(name), std::move(val));
}

// <FOR_UPDATE> → ID <FOR_UPDATE_FACTOR> | EPSILON
std::unique_ptr<StmtNode> Parser::parse_for_update() {
    if (check(TokenKind::RPAREN) || is_at_end()) {
        return nullptr; // EPSILON
    }
    Token name = expect(TokenKind::ID, "Expected identifier in for-update");

    if (check(TokenKind::INC) || check(TokenKind::DEC)) {
        Token op = advance();
        return std::make_unique<UnaryStmtNode>(std::move(name), std::move(op));
    }
    if (is_compound_assign()) {
        Token op = advance();
        auto val = parse_expr();
        return std::make_unique<AssignNode>(std::move(name), std::move(op), std::move(val));
    }
    // ASSIGN EXPR
    expect(TokenKind::ASSIGN, "Expected '=', '++', '--', or compound-assign in for-update");
    auto val = parse_expr();
    return std::make_unique<VarDeclNode>(std::move(name), std::move(val));
}

// ─────────────────────────────────────────────────────────────────────────────
//  Param / arg lists
// ─────────────────────────────────────────────────────────────────────────────

// <PARAM_LIST> → <PARAM> <PARAM_TAIL> | EPSILON
std::vector<Token> Parser::parse_param_list() {
    std::vector<Token> params;
    if (check(TokenKind::RPAREN) || is_at_end()) return params;
    params.push_back(expect(TokenKind::ID, "Expected parameter name"));
    while (match(TokenKind::COMMA)) {
        params.push_back(expect(TokenKind::ID, "Expected parameter name after ','"));
    }
    return params;
}

// <ARG_LIST> → <ARG> <ARG_TAIL> | EPSILON
std::vector<std::unique_ptr<ExprNode>> Parser::parse_arg_list() {
    std::vector<std::unique_ptr<ExprNode>> args;
    if (check(TokenKind::RPAREN) || is_at_end()) return args;
    args.push_back(parse_expr());
    while (match(TokenKind::COMMA)) {
        args.push_back(parse_expr());
    }
    return args;
}

// ─────────────────────────────────────────────────────────────────────────────
//  Expression parsers
//
//  Grammar (simplified):
//  <EXPR>       → LPAREN <EXPR> RPAREN <EXPR_TAIL>
//               | KW_NAH LPAREN <BOOL_EXPR> RPAREN <BOOL_TAIL>
//               | <UNARY_EXPR> <EXPR_TAIL>
//  <EXPR_TAIL>  → <REL_OP> <ARITH_EXPR> <BOOL_TAIL>
//               | <ARITH_OP> <ARITH_EXPR>
//               | <BOOL_TAIL>                          (epsilon)
//  <BOOL_EXPR>  → KW_NAH LPAREN <BOOL_EXPR> RPAREN <BOOL_TAIL>
//               | <VARC> <BOOL_EXPR_FACTOR>
//  <BOOL_EXPR_FACTOR> → <REL_OP> <ARITH_EXPR> <BOOL_TAIL>
//               | <BOOL_TAIL>
//  <BOOL_TAIL>  → <LOG_OP> <BOOL_EXPR> | EPSILON
//  <ARITH_EXPR> → LPAREN <ARITH_EXPR> RPAREN <ARITH_TAIL>
//               | <UNARY_EXPR> <ARITH_TAIL>
//  <ARITH_TAIL> → <ARITH_OP> <ARITH_EXPR> | EPSILON
//  <UNARY_EXPR> → MINUS <UNARY_EXPR> | <VARC>
//  <VARC>       → ID [LPAREN <ARG_LIST> RPAREN] | <VAL>
// ─────────────────────────────────────────────────────────────────────────────

std::unique_ptr<ExprNode> Parser::parse_expr() {
    // LPAREN <EXPR> RPAREN <EXPR_TAIL>
    if (check(TokenKind::LPAREN)) {
        advance();
        auto inner = parse_expr();
        expect(TokenKind::RPAREN, "Expected ')'");
        return parse_expr_tail(std::move(inner));
    }
    // KW_NAH LPAREN <BOOL_EXPR> RPAREN <BOOL_TAIL>
    if (check(TokenKind::KW_NAH)) {
        int ln = peek().line;
        advance();
        expect(TokenKind::LPAREN, "Expected '(' after 'nah'");
        auto inner = parse_bool_expr();
        expect(TokenKind::RPAREN, "Expected ')'");
        auto not_node = std::make_unique<NotExprNode>(ln, std::move(inner));
        return parse_bool_tail(std::move(not_node));
    }
    // <UNARY_EXPR> <EXPR_TAIL>
    auto left = parse_unary_expr();
    return parse_expr_tail(std::move(left));
}

std::unique_ptr<ExprNode> Parser::parse_expr_tail(std::unique_ptr<ExprNode> left) {
    // <REL_OPERATOR> <ARITH_EXPR> <BOOL_TAIL>
    if (is_rel_op()) {
        Token op = advance();
        auto right = parse_arith_expr();
        auto binary = std::make_unique<BinaryExprNode>(std::move(op), std::move(left), std::move(right));
        return parse_bool_tail(std::move(binary));
    }
    // <ARITH_OPERATOR> <ARITH_EXPR>
    if (is_arith_op()) {
        Token op = advance();
        auto right = parse_arith_expr();
        return std::make_unique<BinaryExprNode>(std::move(op), std::move(left), std::move(right));
    }
    // <BOOL_TAIL>  (may be epsilon)
    return parse_bool_tail(std::move(left));
}

std::unique_ptr<ExprNode> Parser::parse_bool_expr() {
    // KW_NAH LPAREN <BOOL_EXPR> RPAREN <BOOL_TAIL>
    if (check(TokenKind::KW_NAH)) {
        int ln = peek().line;
        advance();
        expect(TokenKind::LPAREN, "Expected '(' after 'nah'");
        auto inner = parse_bool_expr();
        expect(TokenKind::RPAREN, "Expected ')'");
        auto not_node = std::make_unique<NotExprNode>(ln, std::move(inner));
        return parse_bool_tail(std::move(not_node));
    }
    // <VARC> <BOOL_EXPR_FACTOR>
    auto left = parse_varc();
    return parse_bool_expr_factor(std::move(left));
}

std::unique_ptr<ExprNode> Parser::parse_bool_expr_factor(std::unique_ptr<ExprNode> left) {
    // <REL_OPERATOR> <ARITH_EXPR> <BOOL_TAIL>
    if (is_rel_op()) {
        Token op = advance();
        auto right = parse_arith_expr();
        auto binary = std::make_unique<BinaryExprNode>(std::move(op), std::move(left), std::move(right));
        return parse_bool_tail(std::move(binary));
    }
    // <BOOL_TAIL>  (may be epsilon)
    return parse_bool_tail(std::move(left));
}

std::unique_ptr<ExprNode> Parser::parse_bool_tail(std::unique_ptr<ExprNode> left) {
    if (is_log_op()) {
        Token op = advance();
        auto right = parse_bool_expr();
        return std::make_unique<BinaryExprNode>(std::move(op), std::move(left), std::move(right));
    }
    return left; // EPSILON
}

std::unique_ptr<ExprNode> Parser::parse_arith_expr() {
    if (check(TokenKind::LPAREN)) {
        advance();
        auto inner = parse_arith_expr();
        expect(TokenKind::RPAREN, "Expected ')'");
        return parse_arith_tail(std::move(inner));
    }
    auto left = parse_unary_expr();
    return parse_arith_tail(std::move(left));
}

std::unique_ptr<ExprNode> Parser::parse_arith_tail(std::unique_ptr<ExprNode> left) {
    if (is_arith_op()) {
        Token op = advance();
        auto right = parse_arith_expr();
        return std::make_unique<BinaryExprNode>(std::move(op), std::move(left), std::move(right));
    }
    return left; // EPSILON
}

std::unique_ptr<ExprNode> Parser::parse_unary_expr() {
    if (check(TokenKind::MINUS)) {
        Token op = advance();
        auto operand = parse_unary_expr();
        return std::make_unique<UnaryExprNode>(std::move(op), std::move(operand));
    }
    return parse_varc();
}

// <VARC> → ID [LPAREN <ARG_LIST> RPAREN]  |  <VAL>
std::unique_ptr<ExprNode> Parser::parse_varc() {
    if (check(TokenKind::ID)) {
        Token id = advance();
        if (check(TokenKind::LPAREN)) {
            advance();
            auto args = parse_arg_list();
            expect(TokenKind::RPAREN, "Expected ')' after argument list");
            return std::make_unique<FuncCallExprNode>(std::move(id), std::move(args));
        }
        return std::make_unique<IdentifierNode>(std::move(id));
    }
    return parse_val();
}

// <VAL> → V_INT | V_FLOAT | V_STRING | V_HEX | V_SCI | V_BINARY | V_IMAGINARY
//        | KW_LEGIT | KW_FAKE | KW_GHOSTED
std::unique_ptr<ExprNode> Parser::parse_val() {
    switch (peek().kind) {
        case TokenKind::V_INT:
        case TokenKind::V_FLOAT:
        case TokenKind::V_STRING:
        case TokenKind::V_HEX:
        case TokenKind::V_SCI:
        case TokenKind::V_BINARY:
        case TokenKind::V_IMAGINARY:
        case TokenKind::KW_LEGIT:
        case TokenKind::KW_FAKE:
        case TokenKind::KW_GHOSTED:
            return std::make_unique<LiteralNode>(advance());
        default: {
            const auto& t = peek();
            throw std::runtime_error(
                "Expected a value or identifier at line " + std::to_string(t.line) +
                ", col " + std::to_string(t.column) +
                " (got " + t.kind_to_string() + " \"" + t.lexeme + "\")");
        }
    }
}
