#include "lexer.h"
#include "transition_matrix.h"

Lexer::Lexer(const std::string& source, const std::string& filepath)
    : source(source), filepath(filepath) {}

std::vector<Token> Lexer::tokenize(){
    TransitionMatrix tm;
    std::vector<Token> tokens;

    int state  = 0;
    int line   = 0;
    int column = 0;
    int i      = 0;
    std::string current_lexeme;
    source+=' ';

    while (i < source.size())
    {
        char c = source[i];
        int next_state = tm.lookup(state, c);
        if (next_state == -1){
            if(!current_lexeme.empty()){
                tokens.push_back(Token{TokenKind::LEXICAL_ERROR, current_lexeme, line, column - static_cast<int>(current_lexeme.size())});
                current_lexeme.clear();
                m_had_error = true;
            }
            state = 0;
            i += 1;
            continue;
        }

        state = next_state;

        if (state >= 100){
            if(state >= 200){
                current_lexeme += c;
                i += 1;
                m_had_error = true;
            }
            tokens.push_back(Token{state, current_lexeme, line, column - static_cast<int>(current_lexeme.size())});
            state = 0;
            current_lexeme.clear();
            continue;
        }

        current_lexeme += c;
        i+= 1;
    }
    return tokens;
}