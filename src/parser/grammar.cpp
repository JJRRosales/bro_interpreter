#include "grammar.h"



const std::vector<std::vector<std::string>> Grammar::grammar = {
//Start
{"<STMT_LIST>"},
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
{"<STMT_LIST>", {{"KW_COOKING", 1 },{"KW_COOK", 1 },{"ID", 1 },{"KW_SENDDAT", 1 },{"EOF", 2 },{"KW_CHECKDIS", 1 },{"KW_LOCKEDIN", 1 },{"KW_LETHIMCOOK", 1 },{"KW_LETS", 1 },{"COMMENT", 1 },{"$", 2 },{"RBRACE", 2 }}},
{"<STMT>", {{"KW_COOKING", 7 },{"KW_LETHIMCOOK", 10 },{"KW_SENDDAT", 11 },{"KW_LETS", 9 },{"COMMENT", 3 },{"KW_LOCKEDIN", 4 },{"ID", 5 },{"KW_COOK", 8 },{"KW_CHECKDIS", 6 }}},
{"<ID_STMT_FACTOR>", {{"MINUS_ASSIGN", 13 },{"DIV_ASSIGN", 13 },{"PLUS_ASSIGN", 13 },{"ASSIGN", 12 },{"LPAREN", 15 },{"DEC", 14 },{"MULT_ASSIGN", 13 },{"INC", 14 }}},
{"<RETURN_FACTOR>", {{"V_STRING", 16 },{"KW_FAKE", 16 },{"V_SCI", 16 },{"ID", 16 },{"V_IMAGINARY", 16 },{"KW_LEGIT", 16 },{"LPAREN", 16 },{"V_HEX", 16 },{"EOF", 17 },{"V_FLOAT", 16 },{"KW_GHOSTED", 16 },{"V_INT", 16 },{"KW_NAH", 16 },{"V_BINARY", 16 },{"SEMICOLON", 17 }}},
{"<VARC_FACTOR>", {{"LPAREN", 18 },{"EOF", 19 },{"RBRACE", 19 },{"ID", 19 },{"KW_COOKING", 19 },{"KW_LOCKEDIN", 19 },{"GTE", 19 },{"MULT", 19 },{"RPAREN", 19 },{"KW_ANDALSO", 19 },{"COMMA", 19 },{"KW_LETHIMCOOK", 19 },{"MOD", 19 },{"KW_SAME", 19 },{"KW_AINTSAME", 19 },{"SEMICOLON", 19 },{"MINUS", 19 },{"GT", 19 },{"KW_CHECKDIS", 19 },{"KW_COOK", 19 },{"KW_EITHERWAY", 19 },{"DIV", 19 },{"COMMENT", 19 },{"LT", 19 },{"LTE", 19 },{"KW_SENDDAT", 19 },{"$", 19 },{"KW_LETS", 19 },{"PLUS", 19 }}},
{"<UNARY_STMT_FACTOR>", {{"DEC", 21 },{"INC", 20 }}},
{"<BOOL_EXPR_FACTOR>", {{"GTE", 22 },{"KW_AINTSAME", 22 },{"KW_ANDALSO", 23 },{"KW_EITHERWAY", 23 },{"LTE", 22 },{"EOF", 23 },{"GT", 22 },{"KW_SAME", 22 },{"LT", 22 },{"RBRACE", 23 },{"SEMICOLON", 23 },{"ID", 23 },{"KW_COOKING", 23 },{"KW_CHECKDIS", 23 },{"KW_COOK", 23 },{"COMMENT", 23 },{"KW_LOCKEDIN", 23 },{"$", 23 },{"RPAREN", 23 },{"KW_SENDDAT", 23 },{"COMMA", 23 },{"KW_LETS", 23 },{"KW_LETHIMCOOK", 23 }}},
{"<FOR_UPDATE_FACTOR>", {{"MULT_ASSIGN", 25 },{"PLUS_ASSIGN", 25 },{"ASSIGN", 26 },{"DEC", 24 },{"MINUS_ASSIGN", 25 },{"DIV_ASSIGN", 25 },{"INC", 24 }}},
{"<COMMENT>", {{"COMMENT", 27 }}},
{"<DECL>", {{"ID", 29 },{"KW_LOCKEDIN", 28 }}},
{"<VAR_DECL>", {{"ID", 30 }}},
{"<CONST_DECL>", {{"KW_LOCKEDIN", 31 }}},
{"<ASSIGN>", {{"ID", 32 }}},
{"<COMPOUND_ASSIGN>", {{"PLUS_ASSIGN", 33 },{"MULT_ASSIGN", 35 },{"DIV_ASSIGN", 36 },{"MINUS_ASSIGN", 34 }}},
{"<UNARY_STMT>", {{"ID", 37 }}},
{"<BOOL_VAL>", {{"KW_LEGIT", 38 },{"KW_FAKE", 39 }}},
{"<VAL>", {{"V_INT", 40 },{"V_SCI", 44 },{"KW_FAKE", 47 },{"V_HEX", 43 },{"KW_GHOSTED", 48 },{"V_FLOAT", 41 },{"KW_LEGIT", 47 },{"V_IMAGINARY", 46 },{"V_STRING", 42 },{"V_BINARY", 45 }}},
{"<VARC>", {{"ID", 49 },{"V_FLOAT", 50 },{"KW_GHOSTED", 50 },{"V_INT", 50 },{"V_STRING", 50 },{"V_HEX", 50 },{"KW_FAKE", 50 },{"V_BINARY", 50 },{"V_SCI", 50 },{"V_IMAGINARY", 50 },{"KW_LEGIT", 50 }}},
{"<EXPR>", {{"V_FLOAT", 53 },{"KW_GHOSTED", 53 },{"V_INT", 53 },{"V_BINARY", 53 },{"V_HEX", 53 },{"V_STRING", 53 },{"KW_FAKE", 53 },{"V_SCI", 53 },{"LPAREN", 51 },{"V_IMAGINARY", 53 },{"KW_NAH", 52 },{"KW_LEGIT", 53 },{"ID", 53 }}},
{"<EXPR_TAIL>", {{"KW_SAME", 54 },{"LT", 54 },{"DIV", 55 },{"GTE", 54 },{"KW_EITHERWAY", 56 },{"KW_AINTSAME", 54 },{"PLUS", 55 },{"MULT", 55 },{"EOF", 56 },{"LTE", 54 },{"MINUS", 55 },{"MOD", 55 },{"GT", 54 },{"KW_ANDALSO", 56 },{"RBRACE", 56 },{"SEMICOLON", 56 },{"ID", 56 },{"KW_COOKING", 56 },{"KW_CHECKDIS", 56 },{"KW_COOK", 56 },{"COMMENT", 56 },{"KW_LOCKEDIN", 56 },{"$", 56 },{"RPAREN", 56 },{"KW_SENDDAT", 56 },{"COMMA", 56 },{"KW_LETS", 56 },{"KW_LETHIMCOOK", 56 }}},
{"<BOOL_EXPR>", {{"KW_LEGIT", 58 },{"KW_NAH", 57 },{"V_IMAGINARY", 58 },{"ID", 58 },{"V_FLOAT", 58 },{"KW_GHOSTED", 58 },{"V_INT", 58 },{"V_BINARY", 58 },{"V_HEX", 58 },{"V_STRING", 58 },{"KW_FAKE", 58 },{"V_SCI", 58 }}},
{"<BOOL_TAIL>", {{"KW_ANDALSO", 59 },{"KW_EITHERWAY", 59 },{"EOF", 60 },{"RBRACE", 60 },{"SEMICOLON", 60 },{"ID", 60 },{"KW_COOKING", 60 },{"KW_CHECKDIS", 60 },{"KW_COOK", 60 },{"COMMENT", 60 },{"KW_LOCKEDIN", 60 },{"$", 60 },{"RPAREN", 60 },{"KW_SENDDAT", 60 },{"COMMA", 60 },{"KW_LETS", 60 },{"KW_LETHIMCOOK", 60 }}},
{"<REL_OPERATOR>", {{"LTE", 64 },{"KW_SAME", 65 },{"GT", 61 },{"GTE", 62 },{"KW_AINTSAME", 66 },{"LT", 63 }}},
{"<LOG_OPERATOR>", {{"KW_EITHERWAY", 68 },{"KW_ANDALSO", 67 }}},
{"<ARITH_EXPR>", {{"LPAREN", 69 },{"V_BINARY", 70 },{"V_HEX", 70 },{"V_STRING", 70 },{"KW_FAKE", 70 },{"V_SCI", 70 },{"KW_GHOSTED", 70 },{"ID", 70 },{"V_IMAGINARY", 70 },{"KW_LEGIT", 70 },{"V_FLOAT", 70 },{"V_INT", 70 }}},
{"<ARITH_TAIL>", {{"MINUS", 71 },{"MOD", 71 },{"EOF", 72 },{"DIV", 71 },{"PLUS", 71 },{"MULT", 71 },{"RBRACE", 72 },{"ID", 72 },{"KW_COOKING", 72 },{"KW_LOCKEDIN", 72 },{"RPAREN", 72 },{"KW_ANDALSO", 72 },{"COMMA", 72 },{"KW_LETHIMCOOK", 72 },{"SEMICOLON", 72 },{"KW_CHECKDIS", 72 },{"KW_COOK", 72 },{"KW_EITHERWAY", 72 },{"COMMENT", 72 },{"$", 72 },{"KW_SENDDAT", 72 },{"KW_LETS", 72 }}},
{"<ARITH_OPERATOR>", {{"DIV", 76 },{"MULT", 75 },{"MOD", 77 },{"PLUS", 73 },{"MINUS", 74 }}},
{"<IF>", {{"KW_CHECKDIS", 78 }}},
{"<ELSEIF>", {{"EOF", 80 },{"KW_WHATABOUT", 79 },{"KW_OTHERWISE", 80 },{"RBRACE", 80 },{"ID", 80 },{"KW_COOKING", 80 },{"KW_CHECKDIS", 80 },{"KW_COOK", 80 },{"COMMENT", 80 },{"KW_LOCKEDIN", 80 },{"$", 80 },{"KW_SENDDAT", 80 },{"KW_LETS", 80 },{"KW_LETHIMCOOK", 80 }}},
{"<ELSE>", {{"KW_OTHERWISE", 81 },{"EOF", 82 },{"RBRACE", 82 },{"ID", 82 },{"KW_COOKING", 82 },{"KW_CHECKDIS", 82 },{"KW_COOK", 82 },{"COMMENT", 82 },{"KW_LOCKEDIN", 82 },{"$", 82 },{"KW_SENDDAT", 82 },{"KW_LETS", 82 },{"KW_LETHIMCOOK", 82 }}},
{"<FOR>", {{"KW_COOKING", 83 }}},
{"<FOR_INIT>", {{"ID", 84 },{"KW_LOCKEDIN", 84 },{"EOF", 85 },{"SEMICOLON", 85 }}},
{"<FOR_UPDATE>", {{"EOF", 87 },{"ID", 86 },{"RPAREN", 87 }}},
{"<WHILE>", {{"KW_COOK", 88 }}},
{"<DOWHILE>", {{"KW_LETS", 89 }}},
{"<FUNC_DECL>", {{"KW_LETHIMCOOK", 90 }}},
{"<PARAM_LIST>", {{"EOF", 92 },{"ID", 91 },{"RPAREN", 92 }}},
{"<PARAM_TAIL>", {{"COMMA", 93 },{"EOF", 94 },{"RPAREN", 94 }}},
{"<PARAM>", {{"ID", 95 }}},
{"<RETURN>", {{"KW_SENDDAT", 96 }}},
{"<FUNC_CALL>", {{"ID", 97 }}},
{"<ARG_LIST>", {{"V_FLOAT", 98 },{"KW_GHOSTED", 98 },{"LPAREN", 98 },{"EOF", 99 },{"V_INT", 98 },{"KW_NAH", 98 },{"V_BINARY", 98 },{"V_HEX", 98 },{"V_STRING", 98 },{"KW_FAKE", 98 },{"V_SCI", 98 },{"KW_LEGIT", 98 },{"V_IMAGINARY", 98 },{"ID", 98 },{"RPAREN", 99 }}},
{"<ARG_TAIL>", {{"COMMA", 100 },{"EOF", 101 },{"RPAREN", 101 }}},
{"<ARG>", {{"V_INT", 102 },{"V_STRING", 102 },{"V_HEX", 102 },{"KW_NAH", 102 },{"V_BINARY", 102 },{"KW_FAKE", 102 },{"V_SCI", 102 },{"V_IMAGINARY", 102 },{"KW_GHOSTED", 102 },{"ID", 102 },{"KW_LEGIT", 102 },{"LPAREN", 102 },{"V_FLOAT", 102 }}}
};