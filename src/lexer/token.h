// ---------------------------------------------------------------------------
//  Token
// ---------------------------------------------------------------------------
#include <iostream>
#include <string>

struct Token {
    enum class Kind {
        // Special internal tokens
        Unknown,
        Eof,

        // State mappings (matching your exact numbering)
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

        // Reserved Keywords (Starting at a safe offset)
        KW_LOCKEDIN = 300,
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

    Kind        kind   = Kind::Unknown;
    std::string lexeme;
    int         line   = 0;
    int         column = 0;

    // Default constructor
    Token() = default;

    // Constructor 1: For passing explicit enum kinds 
    Token(Kind k, std::string lex, int l, int col)
        : kind(k), lexeme(std::move(lex)), line(l), column(col) {}

    // Constructor 2: For passing raw integer states dynamically
    Token(int state_num, std::string lex, int l, int col)
        : kind(static_cast<Kind>(state_num)), lexeme(std::move(lex)), line(l), column(col) {}

    // Helper to get the string representation of the token kind
    std::string kind_to_string() const {
        switch (kind) {
            case Kind::Unknown: return "UNKNOWN";
            case Kind::Eof: return "EOF";
            case Kind::V_HEX: return "V_HEX";
            case Kind::V_BINARY: return "V_BINARY";
            case Kind::V_INT: return "V_INT";
            case Kind::V_FLOAT: return "V_FLOAT";
            case Kind::V_IMAGINARY: return "V_IMAGINARY";
            case Kind::V_SCI: return "V_SCI";
            case Kind::GT: return "GT";
            case Kind::GTE: return "GTE";
            case Kind::LT: return "LT";
            case Kind::LTE: return "LTE";
            case Kind::EQ: return "EQ";
            case Kind::NEQ: return "NEQ";
            case Kind::AND: return "AND";
            case Kind::OR: return "OR";
            case Kind::LPAREN: return "LPAREN";
            case Kind::RPAREN: return "RPAREN";
            case Kind::LBRACE: return "LBRACE";
            case Kind::RBRACE: return "RBRACE";
            case Kind::COLON: return "COLON";
            case Kind::SEMICOLON: return "SEMICOLON";
            case Kind::ID: return "ID";
            case Kind::ASSIGN: return "ASSIGN";
            case Kind::PLUS: return "PLUS";
            case Kind::MULT: return "MULT";
            case Kind::MINUS: return "MINUS";
            case Kind::DIV: return "DIV";
            case Kind::V_STRING: return "V_STRING";
            case Kind::COMMENT: return "COMMENT";
            case Kind::LEXICAL_ERROR: return "LEXICAL_ERROR";
            case Kind::SCI_ERROR: return "SCI_ERROR";
            case Kind::REL_OPERATOR_ERROR: return "REL_OPERATOR_ERROR";
            case Kind::LOG_OPERATOR_ERROR: return "LOG_OPERATOR_ERROR";
            case Kind::HEX_ERROR: return "HEX_ERROR";
            case Kind::INT_ERROR: return "INT_ERROR";
            case Kind::BINARY_ERROR: return "BINARY_ERROR";
            case Kind::FLOAT_ERROR: return "FLOAT_ERROR";
            case Kind::IMAGINARY_ERROR: return "IMAGINARY_ERROR";
            case Kind::ID_ERROR: return "ID_ERROR";
            case Kind::KW_LOCKEDIN: return "KW_LOCKEDIN";
            case Kind::KW_LEGIT: return "KW_LEGIT";
            case Kind::KW_FAKE: return "KW_FAKE";
            case Kind::KW_SAME: return "KW_SAME";
            case Kind::KW_AINTSAME: return "KW_AINTSAME";
            case Kind::KW_ANDALSO: return "KW_ANDALSO";
            case Kind::KW_EITHERWAY: return "KW_EITHERWAY";
            case Kind::KW_NAH: return "KW_NAH";
            case Kind::KW_CHECKDIS: return "KW_CHECKDIS";
            case Kind::KW_WHATABOUT: return "KW_WHATABOUT";
            case Kind::KW_OTHERWISE: return "KW_OTHERWISE";
            case Kind::KW_COOKING: return "KW_COOKING";
            case Kind::KW_COOK: return "KW_COOK";
            case Kind::KW_LETS: return "KW_LETS";
            case Kind::KW_LETHIMCOOK: return "KW_LETHIMCOOK";
            case Kind::KW_SENDDAT: return "KW_SENDDAT";
        }
        return "UNKNOWN";
    }

    std::string to_string() const {
        return "[Line " + std::to_string(line) +
               ", Col "  + std::to_string(column) +
               "] " + kind_to_string() + " (\"" + lexeme + "\")";
    }
};