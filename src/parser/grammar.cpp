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
{ "LPAREN", "<EXPR>", "RPAREN", "<EXPR_TAIL>"},{ "KW_NAH", "LPAREN", "<BOOL_EXPR>", "RPAREN", "<BOOL_TAIL>"},{ "<UNARY_EXPR>", "<EXPR_TAIL>"},
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
{ "LPAREN", "<ARITH_EXPR>", "RPAREN", "<ARITH_TAIL>"},{ "<UNARY_EXPR>", "<ARITH_TAIL>"},
//<UNARY_EXPR>
{ "MINUS", "<UNARY_EXPR>"},{ "<VARC>"},
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
{"<STMT_LIST>", {{"COMMENT", 1 },{"KW_LOCKEDIN", 1 },{"KW_CHECKDIS", 1 },{"ID", 1 },{"KW_COOKING", 1 },{"KW_LETS", 1 },{"KW_COOK", 1 },{"KW_SENDDAT", 1 },{"EOF", 2 },{"KW_LETHIMCOOK", 1 },{"RBRACE", 2 },{"$", 2 }}},
{"<STMT>", {{"KW_LETS", 9 },{"COMMENT", 3 },{"ID", 5 },{"KW_LOCKEDIN", 4 },{"KW_LETHIMCOOK", 10 },{"KW_COOK", 8 },{"KW_COOKING", 7 },{"KW_SENDDAT", 11 },{"KW_CHECKDIS", 6 }}},
{"<ID_STMT_FACTOR>", {{"ASSIGN", 12 },{"MINUS_ASSIGN", 13 },{"DIV_ASSIGN", 13 },{"MULT_ASSIGN", 13 },{"PLUS_ASSIGN", 13 },{"LPAREN", 15 },{"INC", 14 },{"DEC", 14 }}},
{"<RETURN_FACTOR>", {{"KW_FAKE", 16 },{"EOF", 17 },{"MINUS", 16 },{"KW_LEGIT", 16 },{"V_STRING", 16 },{"V_FLOAT", 16 },{"V_INT", 16 },{"V_SCI", 16 },{"LPAREN", 16 },{"V_IMAGINARY", 16 },{"KW_GHOSTED", 16 },{"KW_NAH", 16 },{"V_BINARY", 16 },{"ID", 16 },{"V_HEX", 16 },{"SEMICOLON", 17 }}},
{"<VARC_FACTOR>", {{"LPAREN", 18 },{"EOF", 19 },{"KW_COOK", 19 },{"KW_ANDALSO", 19 },{"KW_SENDDAT", 19 },{"KW_EITHERWAY", 19 },{"RBRACE", 19 },{"MULT", 19 },{"KW_CHECKDIS", 19 },{"KW_LETHIMCOOK", 19 },{"SEMICOLON", 19 },{"MOD", 19 },{"KW_SAME", 19 },{"COMMA", 19 },{"KW_COOKING", 19 },{"LT", 19 },{"LTE", 19 },{"COMMENT", 19 },{"MINUS", 19 },{"GT", 19 },{"DIV", 19 },{"KW_LOCKEDIN", 19 },{"$", 19 },{"KW_AINTSAME", 19 },{"KW_LETS", 19 },{"PLUS", 19 },{"ID", 19 },{"RPAREN", 19 },{"GTE", 19 }}},
{"<UNARY_STMT_FACTOR>", {{"INC", 20 },{"DEC", 21 }}},
{"<BOOL_EXPR_FACTOR>", {{"EOF", 23 },{"KW_ANDALSO", 23 },{"LTE", 22 },{"GTE", 22 },{"KW_SAME", 22 },{"KW_AINTSAME", 22 },{"LT", 22 },{"GT", 22 },{"KW_EITHERWAY", 23 },{"KW_COOK", 23 },{"COMMENT", 23 },{"KW_SENDDAT", 23 },{"RBRACE", 23 },{"KW_CHECKDIS", 23 },{"KW_LETHIMCOOK", 23 },{"SEMICOLON", 23 },{"KW_LOCKEDIN", 23 },{"$", 23 },{"KW_LETS", 23 },{"COMMA", 23 },{"ID", 23 },{"RPAREN", 23 },{"KW_COOKING", 23 }}},
{"<FOR_UPDATE_FACTOR>", {{"DEC", 24 },{"DIV_ASSIGN", 25 },{"ASSIGN", 26 },{"PLUS_ASSIGN", 25 },{"INC", 24 },{"MINUS_ASSIGN", 25 },{"MULT_ASSIGN", 25 }}},
{"<COMMENT>", {{"COMMENT", 27 }}},
{"<DECL>", {{"KW_LOCKEDIN", 28 },{"ID", 29 }}},
{"<VAR_DECL>", {{"ID", 30 }}},
{"<CONST_DECL>", {{"KW_LOCKEDIN", 31 }}},
{"<ASSIGN>", {{"ID", 32 }}},
{"<COMPOUND_ASSIGN>", {{"PLUS_ASSIGN", 33 },{"MINUS_ASSIGN", 34 },{"DIV_ASSIGN", 36 },{"MULT_ASSIGN", 35 }}},
{"<UNARY_STMT>", {{"ID", 37 }}},
{"<BOOL_VAL>", {{"KW_LEGIT", 38 },{"KW_FAKE", 39 }}},
{"<VAL>", {{"V_INT", 40 },{"V_STRING", 42 },{"V_HEX", 43 },{"V_IMAGINARY", 46 },{"KW_GHOSTED", 48 },{"KW_LEGIT", 47 },{"V_BINARY", 45 },{"V_SCI", 44 },{"V_FLOAT", 41 },{"KW_FAKE", 47 }}},
{"<VARC>", {{"V_SCI", 50 },{"KW_FAKE", 50 },{"V_IMAGINARY", 50 },{"V_BINARY", 50 },{"KW_GHOSTED", 50 },{"V_HEX", 50 },{"ID", 49 },{"V_INT", 50 },{"KW_LEGIT", 50 },{"V_STRING", 50 },{"V_FLOAT", 50 }}},
{"<EXPR>", {{"KW_GHOSTED", 53 },{"V_FLOAT", 53 },{"V_SCI", 53 },{"KW_FAKE", 53 },{"V_IMAGINARY", 53 },{"ID", 53 },{"V_BINARY", 53 },{"V_HEX", 53 },{"LPAREN", 51 },{"KW_NAH", 52 },{"KW_LEGIT", 53 },{"MINUS", 53 },{"V_INT", 53 },{"V_STRING", 53 }}},
{"<EXPR_TAIL>", {{"DIV", 55 },{"MINUS", 55 },{"MULT", 55 },{"MOD", 55 },{"PLUS", 55 },{"LT", 54 },{"KW_SAME", 54 },{"KW_AINTSAME", 54 },{"KW_EITHERWAY", 56 },{"GTE", 54 },{"LTE", 54 },{"EOF", 56 },{"GT", 54 },{"KW_ANDALSO", 56 },{"KW_COOK", 56 },{"COMMENT", 56 },{"KW_SENDDAT", 56 },{"RBRACE", 56 },{"KW_CHECKDIS", 56 },{"ID", 56 },{"KW_LETHIMCOOK", 56 },{"SEMICOLON", 56 },{"KW_LOCKEDIN", 56 },{"$", 56 },{"KW_LETS", 56 },{"COMMA", 56 },{"KW_COOKING", 56 },{"RPAREN", 56 }}},
{"<BOOL_EXPR>", {{"KW_NAH", 57 },{"KW_LEGIT", 58 },{"V_STRING", 58 },{"V_INT", 58 },{"V_IMAGINARY", 58 },{"V_FLOAT", 58 },{"KW_GHOSTED", 58 },{"V_SCI", 58 },{"KW_FAKE", 58 },{"ID", 58 },{"V_HEX", 58 },{"V_BINARY", 58 }}},
{"<BOOL_TAIL>", {{"KW_EITHERWAY", 59 },{"KW_ANDALSO", 59 },{"EOF", 60 },{"KW_COOK", 60 },{"COMMENT", 60 },{"KW_SENDDAT", 60 },{"RBRACE", 60 },{"KW_CHECKDIS", 60 },{"KW_LETHIMCOOK", 60 },{"SEMICOLON", 60 },{"KW_LOCKEDIN", 60 },{"$", 60 },{"KW_LETS", 60 },{"COMMA", 60 },{"ID", 60 },{"RPAREN", 60 },{"KW_COOKING", 60 }}},
{"<REL_OPERATOR>", {{"LT", 63 },{"GT", 61 },{"GTE", 62 },{"KW_SAME", 65 },{"KW_AINTSAME", 66 },{"LTE", 64 }}},
{"<LOG_OPERATOR>", {{"KW_EITHERWAY", 68 },{"KW_ANDALSO", 67 }}},
{"<ARITH_EXPR>", {{"ID", 70 },{"V_BINARY", 70 },{"LPAREN", 69 },{"V_HEX", 70 },{"KW_GHOSTED", 70 },{"MINUS", 70 },{"KW_LEGIT", 70 },{"V_STRING", 70 },{"V_INT", 70 },{"V_IMAGINARY", 70 },{"V_FLOAT", 70 },{"V_SCI", 70 },{"KW_FAKE", 70 }}},
{"<UNARY_EXPR>", {{"V_STRING", 72 },{"MINUS", 71 },{"V_FLOAT", 72 },{"KW_GHOSTED", 72 },{"V_SCI", 72 },{"KW_FAKE", 72 },{"V_IMAGINARY", 72 },{"ID", 72 },{"V_BINARY", 72 },{"V_HEX", 72 },{"KW_LEGIT", 72 },{"V_INT", 72 }}},
{"<ARITH_TAIL>", {{"MULT", 73 },{"MOD", 73 },{"PLUS", 73 },{"EOF", 74 },{"DIV", 73 },{"MINUS", 73 },{"KW_COOK", 74 },{"KW_ANDALSO", 74 },{"KW_SENDDAT", 74 },{"KW_EITHERWAY", 74 },{"RBRACE", 74 },{"KW_CHECKDIS", 74 },{"KW_LETHIMCOOK", 74 },{"SEMICOLON", 74 },{"COMMA", 74 },{"KW_COOKING", 74 },{"COMMENT", 74 },{"KW_LOCKEDIN", 74 },{"$", 74 },{"KW_LETS", 74 },{"ID", 74 },{"RPAREN", 74 }}},
{"<ARITH_OPERATOR>", {{"PLUS", 75 },{"MINUS", 76 },{"MULT", 77 },{"MOD", 79 },{"DIV", 78 }}},
{"<IF>", {{"KW_CHECKDIS", 80 }}},
{"<ELSEIF>", {{"KW_OTHERWISE", 82 },{"KW_WHATABOUT", 81 },{"EOF", 82 },{"KW_COOK", 82 },{"COMMENT", 82 },{"KW_SENDDAT", 82 },{"RBRACE", 82 },{"KW_CHECKDIS", 82 },{"ID", 82 },{"KW_LETHIMCOOK", 82 },{"KW_LOCKEDIN", 82 },{"$", 82 },{"KW_LETS", 82 },{"KW_COOKING", 82 }}},
{"<ELSE>", {{"KW_OTHERWISE", 83 },{"EOF", 84 },{"KW_COOK", 84 },{"COMMENT", 84 },{"KW_SENDDAT", 84 },{"RBRACE", 84 },{"KW_CHECKDIS", 84 },{"ID", 84 },{"KW_LETHIMCOOK", 84 },{"KW_LOCKEDIN", 84 },{"$", 84 },{"KW_LETS", 84 },{"KW_COOKING", 84 }}},
{"<FOR>", {{"KW_COOKING", 85 }}},
{"<FOR_INIT>", {{"ID", 86 },{"KW_LOCKEDIN", 86 },{"EOF", 87 },{"SEMICOLON", 87 }}},
{"<FOR_UPDATE>", {{"EOF", 89 },{"ID", 88 },{"RPAREN", 89 }}},
{"<WHILE>", {{"KW_COOK", 90 }}},
{"<DOWHILE>", {{"KW_LETS", 91 }}},
{"<FUNC_DECL>", {{"KW_LETHIMCOOK", 92 }}},
{"<PARAM_LIST>", {{"EOF", 94 },{"ID", 93 },{"RPAREN", 94 }}},
{"<PARAM_TAIL>", {{"EOF", 96 },{"COMMA", 95 },{"RPAREN", 96 }}},
{"<PARAM>", {{"ID", 97 }}},
{"<RETURN>", {{"KW_SENDDAT", 98 }}},
{"<FUNC_CALL>", {{"ID", 99 }}},
{"<ARG_LIST>", {{"KW_LEGIT", 100 },{"MINUS", 100 },{"V_INT", 100 },{"LPAREN", 100 },{"V_STRING", 100 },{"KW_GHOSTED", 100 },{"V_FLOAT", 100 },{"V_SCI", 100 },{"KW_FAKE", 100 },{"V_IMAGINARY", 100 },{"ID", 100 },{"V_BINARY", 100 },{"V_HEX", 100 },{"KW_NAH", 100 },{"EOF", 101 },{"RPAREN", 101 }}},
{"<ARG_TAIL>", {{"EOF", 103 },{"COMMA", 102 },{"RPAREN", 103 }}},
{"<ARG>", {{"V_FLOAT", 104 },{"V_SCI", 104 },{"LPAREN", 104 },{"KW_FAKE", 104 },{"V_IMAGINARY", 104 },{"V_BINARY", 104 },{"KW_GHOSTED", 104 },{"KW_NAH", 104 },{"ID", 104 },{"V_HEX", 104 },{"MINUS", 104 },{"V_INT", 104 },{"KW_LEGIT", 104 },{"V_STRING", 104 }}}
};