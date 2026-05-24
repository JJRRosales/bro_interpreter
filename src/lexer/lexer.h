#pragma once

#include <string>
#include <vector>
#include "token.h"


class Lexer {
public:
    Lexer(const std::string& source);

    std::vector<Token> tokenize();
    
    bool had_error() const { return m_had_error; }
    
private:
    std::string source;
    std::vector<Token> tokens;
    bool        m_had_error = false;  
};
