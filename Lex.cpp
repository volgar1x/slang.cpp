/**
 * Antoine Chauvin © Slang
 */

#include "Lex.hpp"

std::string toString(LexType lexType) {
    switch (lexType) {
        case LexType::EOFF: return "EOFF";
        case LexType::LST_START: return "LST_START";
        case LexType::LST_END: return "LST_END";
        case LexType::VEC_START: return "VEC_START";
        case LexType::VEC_END: return "VEC_END";
        case LexType::SET_START: return "SET_START";
        case LexType::SET_END: return "SET_END";
        case LexType::STR: return "STR";
        case LexType::CHR: return "CHR";
        case LexType::LEX: return "LEX";
        case LexType::WS: return "WS";
        case LexType::HSH: return "HSH";
    }
}
