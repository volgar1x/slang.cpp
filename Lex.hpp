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

class Lex {
public:
    const LexType lexType;
    const std::string lexName;

    Lex(const LexType lexType, const std::string&& lexName)
            : lexType(lexType)
            , lexName(lexName)
    {}
};

const Lex LEX_EOF(LexType::EOFF, "EOF");
const Lex LEX_LST_START(LexType::LST_START, "LST_START");
const Lex LEX_LST_END(LexType::LST_END, "LST_END");
const Lex LEX_VEC_START(LexType::VEC_START, "VEC_START");
const Lex LEX_VEC_END(LexType::VEC_END, "VEC_END");
const Lex LEX_SET_START(LexType::SET_START, "SET_START");
const Lex LEX_SET_END(LexType::SET_END, "SET_END");
const Lex LEX_STR(LexType::STR, "STR");
const Lex LEX_CHR(LexType::CHR, "CHR");
const Lex LEX_WS(LexType::WS, "WS");

class StrLex : public Lex {
public:
    const std::string str;
    StrLex(const std::string str) : Lex(LexType::LEX, "LEX"), str(str) {}
};


#endif//SLANG_LEX_HPP
