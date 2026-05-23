#pragma once

#include <string>

enum class TokenKind
    {
        // Special internal tokens
        Unknown,
        Eof,

        // State mappings
        V_HEX = 100,
        V_BINARY = 101,
        V_INT = 102,
        V_FLOAT = 103,
        V_IMAGINARY = 104,
        V_SCI = 105,

        GT = 107,
        GTE = 108,
        LT = 109,
        LTE = 110,
        EQ = 111,
        NEQ = 112,
        AND = 113,
        OR = 114,

        LPAREN = 115,
        RPAREN = 116,
        LBRACE = 117,
        RBRACE = 118,
        COLON = 119,
        SEMICOLON = 120,

        ID = 121,
        ASSIGN = 122,
        PLUS = 123,
        MULT = 124,
        MINUS = 125,
        DIV = 126,

        V_STRING = 127,
        COMMENT = 199,

        // Errors
        LEXICAL_ERROR = 200,
        SCI_ERROR = 201,
        REL_OPERATOR_ERROR = 202,
        LOG_OPERATOR_ERROR = 203,
        HEX_ERROR = 204,
        INT_ERROR = 205,
        BINARY_ERROR = 206,
        FLOAT_ERROR = 207,
        IMAGINARY_ERROR = 208,
        ID_ERROR = 209,

        // Reserved Keywords
        KW_LOCKEDIN,
        KW_LEGIT,
        KW_FAKE,
        KW_SAME,
        KW_AINTSAME,
        KW_ANDALSO,
        KW_EITHERWAY,
        KW_NAH,
        KW_CHECKDIS,
        KW_WHATABOUT,
        KW_OTHERWISE,
        KW_COOKING,
        KW_COOK,
        KW_LETS,
        KW_LETHIMCOOK,
        KW_SENDDAT
    };

struct Token
{

    TokenKind kind = TokenKind::Unknown;
    std::string lexeme;
    int line = 0;
    int column = 0;

    Token() = default;

    Token(TokenKind k, std::string lex, int l, int col);

    Token(int state_num, std::string lex, int l, int col);

    std::string kind_to_string() const;
    std::string to_string() const;

private:
    void check_and_set_keyword();
};