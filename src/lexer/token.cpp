#include "token.h"

#include <unordered_map>
#include <utility>

Token::Token(TokenKind k, std::string lex, int l, int col)
    : kind(k),
      lexeme(std::move(lex)),
      line(l),
      column(col)
{
    if (kind == TokenKind::ID)
        check_and_set_keyword();
}

Token::Token(int state_num, std::string lex, int l, int col)
    : kind(static_cast<TokenKind>(state_num)),
      lexeme(std::move(lex)),
      line(l),
      column(col)
{
    if (kind == TokenKind::ID)
        check_and_set_keyword();
}

void Token::check_and_set_keyword()
{
    static const std::unordered_map<std::string, TokenKind> KEYWORD_MAP =
    {
        {"lockedIn",   TokenKind::KW_LOCKEDIN},
        {"legit",      TokenKind::KW_LEGIT},
        {"fake",       TokenKind::KW_FAKE},
        {"same",       TokenKind::KW_SAME},
        {"aintSame",   TokenKind::KW_AINTSAME},
        {"andAlso",    TokenKind::KW_ANDALSO},
        {"eitherWay",  TokenKind::KW_EITHERWAY},
        {"nah",        TokenKind::KW_NAH},
        {"checkDis",   TokenKind::KW_CHECKDIS},
        {"whatAbout",  TokenKind::KW_WHATABOUT},
        {"otherwise",  TokenKind::KW_OTHERWISE},
        {"cooking",    TokenKind::KW_COOKING},
        {"cook",       TokenKind::KW_COOK},
        {"lets",       TokenKind::KW_LETS},
        {"letHimCook", TokenKind::KW_LETHIMCOOK},
        {"sendDat",    TokenKind::KW_SENDDAT}
    };

    auto it = KEYWORD_MAP.find(lexeme);

    if (it != KEYWORD_MAP.end())
        kind = it->second;
}

std::string Token::kind_to_string() const
    {
    static const std::unordered_map<TokenKind, std::string> names ={
        {TokenKind::Unknown, "UNKNOWN"},
        {TokenKind::Eof, "EOF"},
        {TokenKind::V_HEX, "V_HEX"},
        {TokenKind::V_BINARY, "V_BINARY"},
        {TokenKind::V_INT, "V_INT"},
        {TokenKind::V_FLOAT, "V_FLOAT"},
        {TokenKind::V_IMAGINARY, "V_IMAGINARY"},
        {TokenKind::V_SCI, "V_SCI"},
        {TokenKind::GT, "GT"},
        {TokenKind::GTE, "GTE"},
        {TokenKind::LT, "LT"},
        {TokenKind::LTE, "LTE"},
        {TokenKind::EQ, "EQ"},
        {TokenKind::NEQ, "NEQ"},
        {TokenKind::AND, "AND"},
        {TokenKind::OR, "OR"},
        {TokenKind::LPAREN, "LPAREN"},
        {TokenKind::RPAREN, "RPAREN"},
        {TokenKind::LBRACE, "LBRACE"},
        {TokenKind::RBRACE, "RBRACE"},
        {TokenKind::COLON, "COLON"},
        {TokenKind::SEMICOLON, "SEMICOLON"},
        {TokenKind::ID, "ID"},
        {TokenKind::ASSIGN, "ASSIGN"},
        {TokenKind::PLUS, "PLUS"},
        {TokenKind::MULT, "MULT"},
        {TokenKind::MINUS, "MINUS"},
        {TokenKind::DIV, "DIV"},
        {TokenKind::V_STRING, "V_STRING"},
        {TokenKind::COMMENT, "COMMENT"},
        {TokenKind::LEXICAL_ERROR, "LEXICAL_ERROR"},
        {TokenKind::SCI_ERROR, "SCI_ERROR"},
        {TokenKind::REL_OPERATOR_ERROR, "REL_OPERATOR_ERROR"},
        {TokenKind::LOG_OPERATOR_ERROR, "LOG_OPERATOR_ERROR"},
        {TokenKind::HEX_ERROR, "HEX_ERROR"},
        {TokenKind::INT_ERROR, "INT_ERROR"},
        {TokenKind::BINARY_ERROR, "BINARY_ERROR"},
        {TokenKind::FLOAT_ERROR, "FLOAT_ERROR"},
        {TokenKind::IMAGINARY_ERROR, "IMAGINARY_ERROR"},
        {TokenKind::ID_ERROR, "ID_ERROR"},
        {TokenKind::KW_LOCKEDIN, "KW_LOCKEDIN"},
        {TokenKind::KW_LEGIT, "KW_LEGIT"},
        {TokenKind::KW_FAKE, "KW_FAKE"},
        {TokenKind::KW_SAME, "KW_SAME"},
        {TokenKind::KW_AINTSAME, "KW_AINTSAME"},
        {TokenKind::KW_ANDALSO, "KW_ANDALSO"},
        {TokenKind::KW_EITHERWAY, "KW_EITHERWAY"},
        {TokenKind::KW_NAH, "KW_NAH"},
        {TokenKind::KW_CHECKDIS, "KW_CHECKDIS"},
        {TokenKind::KW_WHATABOUT, "KW_WHATABOUT"},
        {TokenKind::KW_OTHERWISE, "KW_OTHERWISE"},
        {TokenKind::KW_COOKING, "KW_COOKING"},
        {TokenKind::KW_COOK, "KW_COOK"},
        {TokenKind::KW_LETS, "KW_LETS"},
        {TokenKind::KW_LETHIMCOOK, "KW_LETHIMCOOK"},
        {TokenKind::KW_SENDDAT, "KW_SENDDAT"}
    };
    auto it = names.find(kind);

    return it != names.end()
        ? it->second
        : "UNKNOWN";
}

std::string Token::to_string() const
{
    return "[Line " + std::to_string(line) +
           ", Col " + std::to_string(column) +
           "] " + kind_to_string() +
           " (\"" + lexeme + "\")";
}