#include "grammar.h"



const std::vector<std::vector<std::string>> Grammar::grammar = {
//Start
{"<PROGRAM>"},
//<PROGRAM>
{ "<STMT_LIST>"},
//<STMT_LIST>
{ "<STMT>", "<STMT_LIST>"},{ "EPSILON"},
//<STMT>
{ "<COMMENT>"},{ "<CONST_DECL>"},{ "ID", "<ID_STMT_FACTOR>"},{ "<IF>"},{ "<FOR>"},{ "<WHILE>"},{ "<DOWHILE>"},{ "<FUNC_DECL>"},{ "<RETURN>"},
//<ID_STMT_FACTOR>
{ "ASSIGN", "<EXPR>"},{ "<COMPOUND_ASSIGN>", "<EXPR>"},{ "<UNARY_STMT_FACTOR>"},{ "LPAREN", "<ARG_LIST>", "RPAREN"},
//<RETURN_FACTOR>
{ "<EXPR>"},{ "EPSILON"},
//<VARC_FACTOR>
{ "LPAREN", "<ARG_LIST>", "RPAREN"},{ "EPSILON"},
//<UNARY_STMT_FACTOR>
{ "INC"},{ "DEC"},
//<BOOL_EXPR_FACTOR>
{ "<REL_OPERATOR>", "<ARITH_EXPR>", "<BOOL_TAIL>"},{ "<BOOL_TAIL>"},
//<FOR_UPDATE_FACTOR>
{ "<UNARY_STMT_FACTOR>"},{ "<COMPOUND_ASSIGN>"},{ "ASSIGN", "<EXPR>"},
//<COMMENT>
{ "COMMENT"},
//<DECL>
{ "<CONST_DECL>"},{ "<VAR_DECL>"},
//<VAR_DECL>
{ "ID", "ASSIGN", "<EXPR>"},
//<CONST_DECL>
{ "KW_LOCKEDIN", "ID", "ASSIGN", "<EXPR>"},
//<ASSIGN>
{ "ID", "<COMPOUND_ASSIGN>", "<EXPR>"},
//<COMPOUND_ASSIGN>
{ "PLUS_ASSIGN"},{ "MINUS_ASSIGN"},{ "MULT_ASSIGN"},{ "DIV_ASSIGN"},
//<UNARY_STMT>
{ "ID", "<UNARY_STMT_FACTOR>"},
//<BOOL_VAL>
{ "KW_LEGIT"},{ "KW_FAKE"},
//<VAL>
{ "V_INT"},{ "V_FLOAT"},{ "V_STRING"},{ "V_HEX"},{ "V_SCI"},{ "V_BINARY"},{ "V_IMAGINARY"},{ "<BOOL_VAL>"},{ "KW_GHOSTED"},
//<VARC>
{ "ID", "<VARC_FACTOR>"},{ "<VAL>"},
//<EXPR>
{ "LPAREN", "<EXPR>", "RPAREN", "<EXPR_TAIL>"},{ "KW_NAH", "LPAREN", "<BOOL_EXPR>", "RPAREN", "<BOOL_TAIL>"},{ "<VARC>", "<EXPR_TAIL>"},
//<EXPR_TAIL>
{ "<REL_OPERATOR>", "<ARITH_EXPR>", "<BOOL_TAIL>"},{ "<ARITH_OPERATOR>", "<ARITH_EXPR>"},{ "<BOOL_TAIL>"},
//<BOOL_EXPR>
{ "KW_NAH", "LPAREN", "<BOOL_EXPR>", "RPAREN", "<BOOL_TAIL>"},{ "<VARC>", "<BOOL_EXPR_FACTOR>"},
//<BOOL_TAIL>
{ "<LOG_OPERATOR>", "<BOOL_EXPR>"},{ "EPSILON"},
//<REL_OPERATOR>
{ "GT"},{ "GTE"},{ "LT"},{ "LTE"},{ "KW_SAME"},{ "KW_AINTSAME"},
//<LOG_OPERATOR>
{ "KW_ANDALSO"},{ "KW_EITHERWAY"},
//<ARITH_EXPR>
{ "LPAREN", "<ARITH_EXPR>", "RPAREN", "<ARITH_TAIL>"},{ "<VARC>", "<ARITH_TAIL>"},
//<ARITH_TAIL>
{ "<ARITH_OPERATOR>", "<ARITH_EXPR>"},{ "EPSILON"},
//<ARITH_OPERATOR>
{ "PLUS"},{ "MINUS"},{ "MULT"},{ "DIV"},{ "MOD"},
//<IF>
{ "KW_CHECKDIS", "LPAREN", "<BOOL_EXPR>", "RPAREN", "LBRACE", "<STMT_LIST>", "RBRACE", "<ELSEIF>"},
//<ELSEIF>
{ "KW_WHATABOUT", "LPAREN", "<BOOL_EXPR>", "RPAREN", "LBRACE", "<STMT_LIST>", "RBRACE", "<ELSEIF>"},{ "<ELSE>"},
//<ELSE>
{ "KW_OTHERWISE", "LBRACE", "<STMT_LIST>", "RBRACE"},{ "EPSILON"},
//<FOR>
{ "KW_COOKING", "LPAREN", "<FOR_INIT>", "SEMICOLON", "<BOOL_EXPR>", "SEMICOLON", "<FOR_UPDATE>", "RPAREN", "LBRACE", "<STMT_LIST>", "RBRACE"},
//<FOR_INIT>
{ "<DECL>"},{ "EPSILON"},
//<FOR_UPDATE>
{ "ID", "<FOR_UPDATE_FACTOR>"},{ "EPSILON"},
//<WHILE>
{ "KW_COOK", "LPAREN", "<BOOL_EXPR>", "RPAREN", "LBRACE", "<STMT_LIST>", "RBRACE"},
//<DOWHILE>
{ "KW_LETS", "LBRACE", "<STMT_LIST>", "RBRACE", "KW_COOK", "LPAREN", "<BOOL_EXPR>", "RPAREN"},
//<FUNC_DECL>
{ "KW_LETHIMCOOK", "ID", "LPAREN", "<PARAM_LIST>", "RPAREN", "LBRACE", "<STMT_LIST>", "RBRACE"},
//<PARAM_LIST>
{ "<PARAM>", "<PARAM_TAIL>"},{ "EPSILON"},
//<PARAM_TAIL>
{ "COMMA", "<PARAM>", "<PARAM_TAIL>"},{ "EPSILON"},
//<PARAM>
{ "ID"},
//<RETURN>
{ "KW_SENDDAT", "<RETURN_FACTOR>", "SEMICOLON"},
//<FUNC_CALL>
{ "ID", "LPAREN", "<ARG_LIST>", "RPAREN"},
//<ARG_LIST>
{ "<ARG>", "<ARG_TAIL>"},{ "EPSILON"},
//<ARG_TAIL>
{ "COMMA", "<ARG>", "<ARG_TAIL>"},{ "EPSILON"},
//<ARG>
{ "<EXPR>"}
}; 

const std::unordered_map<std::string, std::unordered_map<std::string, int>> Grammar::parsing_table = {
{"<PROGRAM>", {{"KW_CHECKDIS", 1 },{"ID", 1 },{"KW_LOCKEDIN", 1 },{"COMMENT", 1 },{"KW_LETHIMCOOK", 1 },{"EOF", 1 },{"KW_LETS", 1 },{"KW_SENDDAT", 1 },{"KW_COOK", 1 },{"KW_COOKING", 1 },{"$", 1 }}},
{"<STMT_LIST>", {{"KW_CHECKDIS", 2 },{"EOF", 3 },{"ID", 2 },{"KW_LOCKEDIN", 2 },{"KW_LETHIMCOOK", 2 },{"COMMENT", 2 },{"KW_LETS", 2 },{"KW_SENDDAT", 2 },{"KW_COOK", 2 },{"KW_COOKING", 2 },{"$", 3 },{"RBRACE", 3 }}},
{"<STMT>", {{"KW_COOKING", 8 },{"ID", 6 },{"KW_LOCKEDIN", 5 },{"KW_LETS", 10 },{"KW_CHECKDIS", 7 },{"KW_COOK", 9 },{"KW_SENDDAT", 12 },{"KW_LETHIMCOOK", 11 },{"COMMENT", 4 }}},
{"<ID_STMT_FACTOR>", {{"LPAREN", 16 },{"INC", 15 },{"MINUS_ASSIGN", 14 },{"ASSIGN", 13 },{"MULT_ASSIGN", 14 },{"DEC", 15 },{"PLUS_ASSIGN", 14 },{"DIV_ASSIGN", 14 }}},
{"<RETURN_FACTOR>", {{"KW_NAH", 17 },{"V_BINARY", 17 },{"KW_FAKE", 17 },{"V_FLOAT", 17 },{"V_SCI", 17 },{"ID", 17 },{"EOF", 18 },{"KW_GHOSTED", 17 },{"V_INT", 17 },{"V_IMAGINARY", 17 },{"V_STRING", 17 },{"LPAREN", 17 },{"KW_LEGIT", 17 },{"V_HEX", 17 },{"SEMICOLON", 18 }}},
{"<VARC_FACTOR>", {{"EOF", 20 },{"LPAREN", 19 },{"KW_SAME", 20 },{"ID", 20 },{"RPAREN", 20 },{"KW_COOKING", 20 },{"MULT", 20 },{"COMMA", 20 },{"LT", 20 },{"KW_LETHIMCOOK", 20 },{"SEMICOLON", 20 },{"PLUS", 20 },{"KW_SENDDAT", 20 },{"MOD", 20 },{"RBRACE", 20 },{"$", 20 },{"KW_EITHERWAY", 20 },{"KW_AINTSAME", 20 },{"LTE", 20 },{"KW_LETS", 20 },{"COMMENT", 20 },{"KW_ANDALSO", 20 },{"DIV", 20 },{"KW_CHECKDIS", 20 },{"KW_LOCKEDIN", 20 },{"GT", 20 },{"KW_COOK", 20 },{"MINUS", 20 },{"GTE", 20 }}},
{"<UNARY_STMT_FACTOR>", {{"DEC", 22 },{"INC", 21 }}},
{"<BOOL_EXPR_FACTOR>", {{"LTE", 23 },{"LT", 23 },{"KW_EITHERWAY", 24 },{"GT", 23 },{"EOF", 24 },{"GTE", 23 },{"KW_SAME", 23 },{"KW_AINTSAME", 23 },{"KW_ANDALSO", 24 },{"KW_SENDDAT", 24 },{"$", 24 },{"RBRACE", 24 },{"ID", 24 },{"RPAREN", 24 },{"KW_LETS", 24 },{"KW_COOKING", 24 },{"COMMA", 24 },{"COMMENT", 24 },{"KW_CHECKDIS", 24 },{"KW_LOCKEDIN", 24 },{"KW_COOK", 24 },{"SEMICOLON", 24 },{"KW_LETHIMCOOK", 24 }}},
{"<FOR_UPDATE_FACTOR>", {{"ASSIGN", 27 },{"INC", 25 },{"MULT_ASSIGN", 26 },{"PLUS_ASSIGN", 26 },{"DIV_ASSIGN", 26 },{"MINUS_ASSIGN", 26 },{"DEC", 25 }}},
{"<COMMENT>", {{"COMMENT", 28 }}},
{"<DECL>", {{"ID", 30 },{"KW_LOCKEDIN", 29 }}},
{"<VAR_DECL>", {{"ID", 31 }}},
{"<CONST_DECL>", {{"KW_LOCKEDIN", 32 }}},
{"<ASSIGN>", {{"ID", 33 }}},
{"<COMPOUND_ASSIGN>", {{"DIV_ASSIGN", 37 },{"MINUS_ASSIGN", 35 },{"MULT_ASSIGN", 36 },{"PLUS_ASSIGN", 34 }}},
{"<UNARY_STMT>", {{"ID", 38 }}},
{"<BOOL_VAL>", {{"KW_FAKE", 40 },{"KW_LEGIT", 39 }}},
{"<VAL>", {{"KW_LEGIT", 48 },{"V_BINARY", 46 },{"V_INT", 41 },{"V_HEX", 44 },{"KW_FAKE", 48 },{"V_FLOAT", 42 },{"V_IMAGINARY", 47 },{"KW_GHOSTED", 49 },{"V_SCI", 45 },{"V_STRING", 43 }}},
{"<VARC>", {{"V_STRING", 51 },{"V_INT", 51 },{"V_HEX", 51 },{"ID", 50 },{"V_IMAGINARY", 51 },{"KW_LEGIT", 51 },{"KW_FAKE", 51 },{"KW_GHOSTED", 51 },{"V_BINARY", 51 },{"V_FLOAT", 51 },{"V_SCI", 51 }}},
{"<EXPR>", {{"V_SCI", 54 },{"KW_GHOSTED", 54 },{"ID", 54 },{"V_STRING", 54 },{"V_INT", 54 },{"V_HEX", 54 },{"KW_LEGIT", 54 },{"V_IMAGINARY", 54 },{"KW_FAKE", 54 },{"V_BINARY", 54 },{"KW_NAH", 53 },{"V_FLOAT", 54 },{"LPAREN", 52 }}},
{"<EXPR_TAIL>", {{"LTE", 55 },{"PLUS", 56 },{"LT", 55 },{"GT", 55 },{"KW_EITHERWAY", 57 },{"EOF", 57 },{"GTE", 55 },{"DIV", 56 },{"MOD", 56 },{"KW_ANDALSO", 57 },{"KW_AINTSAME", 55 },{"MINUS", 56 },{"MULT", 56 },{"KW_SAME", 55 },{"KW_SENDDAT", 57 },{"$", 57 },{"RBRACE", 57 },{"ID", 57 },{"RPAREN", 57 },{"KW_LETS", 57 },{"KW_COOKING", 57 },{"COMMA", 57 },{"COMMENT", 57 },{"KW_LOCKEDIN", 57 },{"KW_CHECKDIS", 57 },{"KW_COOK", 57 },{"SEMICOLON", 57 },{"KW_LETHIMCOOK", 57 }}},
{"<BOOL_EXPR>", {{"KW_FAKE", 59 },{"KW_NAH", 58 },{"V_FLOAT", 59 },{"V_SCI", 59 },{"KW_GHOSTED", 59 },{"V_INT", 59 },{"ID", 59 },{"V_STRING", 59 },{"KW_LEGIT", 59 },{"V_HEX", 59 },{"V_IMAGINARY", 59 },{"V_BINARY", 59 }}},
{"<BOOL_TAIL>", {{"KW_ANDALSO", 60 },{"KW_EITHERWAY", 60 },{"EOF", 61 },{"KW_SENDDAT", 61 },{"$", 61 },{"RBRACE", 61 },{"ID", 61 },{"RPAREN", 61 },{"KW_LETS", 61 },{"KW_COOKING", 61 },{"COMMA", 61 },{"COMMENT", 61 },{"KW_CHECKDIS", 61 },{"KW_LOCKEDIN", 61 },{"KW_COOK", 61 },{"SEMICOLON", 61 },{"KW_LETHIMCOOK", 61 }}},
{"<REL_OPERATOR>", {{"KW_AINTSAME", 67 },{"LTE", 65 },{"GTE", 63 },{"GT", 62 },{"KW_SAME", 66 },{"LT", 64 }}},
{"<LOG_OPERATOR>", {{"KW_ANDALSO", 68 },{"KW_EITHERWAY", 69 }}},
{"<ARITH_EXPR>", {{"V_STRING", 71 },{"KW_LEGIT", 71 },{"V_HEX", 71 },{"ID", 71 },{"V_IMAGINARY", 71 },{"LPAREN", 70 },{"V_BINARY", 71 },{"KW_FAKE", 71 },{"V_FLOAT", 71 },{"KW_GHOSTED", 71 },{"V_SCI", 71 },{"V_INT", 71 }}},
{"<ARITH_TAIL>", {{"MULT", 72 },{"MINUS", 72 },{"MOD", 72 },{"DIV", 72 },{"PLUS", 72 },{"EOF", 73 },{"ID", 73 },{"RPAREN", 73 },{"KW_COOKING", 73 },{"COMMA", 73 },{"SEMICOLON", 73 },{"KW_LETHIMCOOK", 73 },{"KW_SENDDAT", 73 },{"$", 73 },{"RBRACE", 73 },{"KW_EITHERWAY", 73 },{"KW_LETS", 73 },{"KW_ANDALSO", 73 },{"COMMENT", 73 },{"KW_CHECKDIS", 73 },{"KW_LOCKEDIN", 73 },{"KW_COOK", 73 }}},
{"<ARITH_OPERATOR>", {{"MOD", 78 },{"DIV", 77 },{"MINUS", 75 },{"PLUS", 74 },{"MULT", 76 }}},
{"<IF>", {{"KW_CHECKDIS", 79 }}},
{"<ELSEIF>", {{"EOF", 81 },{"KW_WHATABOUT", 80 },{"KW_OTHERWISE", 81 },{"KW_SENDDAT", 81 },{"$", 81 },{"RBRACE", 81 },{"ID", 81 },{"KW_LETS", 81 },{"KW_COOKING", 81 },{"COMMENT", 81 },{"KW_LOCKEDIN", 81 },{"KW_CHECKDIS", 81 },{"KW_COOK", 81 },{"KW_LETHIMCOOK", 81 }}},
{"<ELSE>", {{"EOF", 83 },{"KW_OTHERWISE", 82 },{"KW_SENDDAT", 83 },{"$", 83 },{"RBRACE", 83 },{"ID", 83 },{"KW_LETS", 83 },{"KW_COOKING", 83 },{"COMMENT", 83 },{"KW_LOCKEDIN", 83 },{"KW_CHECKDIS", 83 },{"KW_COOK", 83 },{"KW_LETHIMCOOK", 83 }}},
{"<FOR>", {{"KW_COOKING", 84 }}},
{"<FOR_INIT>", {{"ID", 85 },{"KW_LOCKEDIN", 85 },{"EOF", 86 },{"SEMICOLON", 86 }}},
{"<FOR_UPDATE>", {{"ID", 87 },{"EOF", 88 },{"RPAREN", 88 }}},
{"<WHILE>", {{"KW_COOK", 89 }}},
{"<DOWHILE>", {{"KW_LETS", 90 }}},
{"<FUNC_DECL>", {{"KW_LETHIMCOOK", 91 }}},
{"<PARAM_LIST>", {{"ID", 92 },{"EOF", 93 },{"RPAREN", 93 }}},
{"<PARAM_TAIL>", {{"COMMA", 94 },{"EOF", 95 },{"RPAREN", 95 }}},
{"<PARAM>", {{"ID", 96 }}},
{"<RETURN>", {{"KW_SENDDAT", 97 }}},
{"<FUNC_CALL>", {{"ID", 98 }}},
{"<ARG_LIST>", {{"V_SCI", 99 },{"KW_GHOSTED", 99 },{"ID", 99 },{"EOF", 100 },{"V_STRING", 99 },{"LPAREN", 99 },{"V_INT", 99 },{"V_HEX", 99 },{"KW_LEGIT", 99 },{"V_IMAGINARY", 99 },{"KW_FAKE", 99 },{"KW_NAH", 99 },{"V_BINARY", 99 },{"V_FLOAT", 99 },{"RPAREN", 100 }}},
{"<ARG_TAIL>", {{"EOF", 102 },{"COMMA", 101 },{"RPAREN", 102 }}},
{"<ARG>", {{"V_INT", 103 },{"V_HEX", 103 },{"KW_LEGIT", 103 },{"V_IMAGINARY", 103 },{"KW_FAKE", 103 },{"KW_GHOSTED", 103 },{"KW_NAH", 103 },{"V_BINARY", 103 },{"V_FLOAT", 103 },{"V_SCI", 103 },{"ID", 103 },{"V_STRING", 103 },{"LPAREN", 103 }}}
};