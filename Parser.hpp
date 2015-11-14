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

    std::unique_ptr<const Expression> next();

private:
    std::unique_ptr<const Expression> next(const Lex& lex);
    std::unique_ptr<const Expression> nextString();
    std::unique_ptr<const Expression> scanTerm(const std::string& term);
    template<typename Col>
    std::unique_ptr<const Expression> nextCol(LexType delimiter);

    Lexer& _lexer;
};


#endif//SLANG_PARSER_HPP
