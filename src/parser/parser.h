#pragma once

#include <string>
#include <vector>
#include "../lexer/token.h"

class Parser {
    public:
        Parser(const std::vector<Token>& tokens);

        void parse();

    private:
        void validate_syntax();
        std::vector<Token> m_tokens;
};