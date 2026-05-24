#include "lexer.h"
#include "transition_matrix.h"

Lexer::Lexer(const std::string& source )
    : source(source) {}

std::vector<Token> Lexer::tokenize(){
    TransitionMatrix tm;

    int state  = 0;
    int line   = 0;
    int column = 0;
    int i      = 0;
    std::string current_lexeme;
    int lexeme_start_line   = 1;
    int lexeme_start_column = 1;
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
            if (c == '\n') { ++line; column = 1; }
            else           { ++column; }
            state = 0;
            ++i;
            lexeme_start_line   = line;
            lexeme_start_column = column;
            continue;
        }

        state = next_state;

        if (state >= 100){
            if(state >= 200){
                current_lexeme += c;
                if (c == '\n') { ++line; column = 1; }
                else           { ++column; }
                ++i;
                m_had_error = true;
            }
            tokens.push_back(Token{state, current_lexeme, line, column - static_cast<int>(current_lexeme.size())});
            state = 0;
            current_lexeme.clear();
            lexeme_start_line   = line;
            lexeme_start_column = column;
            continue;
        }

        if (current_lexeme.empty())
        {
            lexeme_start_line   = line;
            lexeme_start_column = column;
        }
 
        current_lexeme += c;
 
        if (c == '\n') { ++line; column = 1; }
        else           { ++column; }
        ++i;
    }
    return tokens;
}