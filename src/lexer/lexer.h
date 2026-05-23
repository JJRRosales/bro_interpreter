#pragma once

#include <string>
#include <vector>
#include "token.h"


class Lexer {
public:
    Lexer(const std::string& source, const std::string& filepath);

    std::vector<Token> tokenize();
    
    bool had_error() const { return m_had_error; }
    
private:

    std::string source;
    std::string filepath;
    bool        m_had_error = false;  
};
