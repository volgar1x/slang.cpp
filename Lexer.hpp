/**
 * Antoine Chauvin © Slang
 */

#ifndef SLANG_LEXER_HPP
#define SLANG_LEXER_HPP


#include <istream>
#include "Lex.hpp"

class Lexer {
public:
    Lexer(std::istream& is);

    const Lex* next();
    void expect(const Lex* lex);

private:
    const Lex* nextStr();

    void ignoreLine();
    void ignoreWhitespaces();
    std::string untilWhitespace();
    bool isValidIdentifierPart(char c);
    bool isWhitespacePart(char c);
    bool isEndLex(char c);

    std::istream& _is;
    bool _inStr;
};


#endif//SLANG_LEXER_HPP
