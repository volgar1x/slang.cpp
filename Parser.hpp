/**
 * Antoine Chauvin © Slang
 */

#ifndef SLANG_PARSER_HPP
#define SLANG_PARSER_HPP


#include "Lexer.hpp"
#include "Expression.hpp"

class Parser {
public:
    Parser(Lexer& lexer);

    const Expression* next();

private:
    const Expression* next(const Lex* lex);
    const Expression* nextString();
    const Expression* scanTerm(const StrLex* lex);
    template<typename Col>
    const Expression* nextCol(LexType delimiter);

    Lexer& _lexer;
};


#endif//SLANG_PARSER_HPP
