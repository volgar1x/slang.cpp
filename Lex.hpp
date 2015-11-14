/**
 * Antoine Chauvin © Slang
 */

#ifndef SLANG_LEX_HPP
#define SLANG_LEX_HPP


#include <string>

enum class LexType {
    EOFF,
    LST_START,
    LST_END,
    VEC_START,
    VEC_END,
    SET_START,
    SET_END,
    STR,
    CHR,
    LEX,
    WS,
};

std::string toString(LexType lexType);

class Lex {
public:
    const LexType lexType;

    Lex(const LexType lexType)
            : lexType(lexType)
            {}
};

class StrLex : public Lex {
public:
    const std::string str;
    StrLex(const std::string str) : Lex(LexType::LEX), str(str) {}
};


#endif//SLANG_LEX_HPP
