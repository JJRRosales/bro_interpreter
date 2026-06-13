#include "parser.h"
#include "grammar.h"
#include <stack>
#include <string>
#include <stdexcept>


Parser::Parser(const std::vector<Token>& tokens)
    : m_tokens(tokens) {}

void Parser::parse() {
    validate_syntax();
}

void Parser::validate_syntax(){
    
    std::stack<std::string> parse_stack;
    parse_stack.push("EOF"); // push $
    parse_stack.push(Grammar::grammar[0][0]); // push start symbol
    size_t i = 0;

    while(!parse_stack.empty()){
        if (i >= m_tokens.size()){
            throw std::runtime_error("Syntax error: unexpected end of input");
        }
        std::string current = parse_stack.top();
        parse_stack.pop();
        if (current == "EPSILON") {continue;};
        if (current == m_tokens[i].kind_to_string()){
            i++;
            continue;
        }
        const std::string err = "Syntax error: at " + m_tokens[i].to_string() + ", expected " + current;

        auto row = Grammar::parsing_table.find(current);
        if (row == Grammar::parsing_table.end()){
            throw std::runtime_error(err);
        }

        auto production_idx = row->second.find(m_tokens[i].kind_to_string());
        if(production_idx == row->second.end()){
            throw std::runtime_error(err);
        }

        const auto& production = Grammar::grammar[production_idx->second];

        for (auto it = production.rbegin(); it != production.rend(); ++it){
            parse_stack.push(*it);
        }
    }
    if (i != m_tokens.size()){
        throw std::runtime_error("Syntax error: unexpected token at " + m_tokens[i].to_string());
    }
};
