/**
 * Antoine Chauvin © Slang
 */

#include <exception>
#include <sstream>
#include "Parser.hpp"
#include "Expressions.hpp"

Parser::Parser(Lexer& lexer)
        : _lexer(lexer)
{ }

const Expression* Parser::next() {
    const Lex* lex;
    while (true) {
        lex = _lexer.next();
        if (lex->lexType != LexType::WS) {
            break;
        }
    }
    return next(lex);
}

const Expression* Parser::next(const Lex* lex) {
    switch (lex->lexType) {
        case LexType::STR:
            return nextString();

        case LexType::LEX:
            return scanTerm(static_cast<const StrLex*>(lex));

        case LexType::LST_START:
            return nextCol<List>(LexType::LST_END);

        case LexType::VEC_START:
            return nextCol<Vector>(LexType::VEC_END);

        case LexType::SET_START:
            return nextCol<Set>(LexType::SET_END);

        default:
            return new Nil;
    }
}

const Expression* Parser::nextString() {
    const Lex* lex = _lexer.next();
    if (lex->lexType == LexType::LEX) {
        const StrLex* strLex = static_cast<const StrLex*>(lex);
        _lexer.expect(&LEX_STR);
        return new String(strLex->str);
    } else if (lex->lexType == LexType::STR) {
        return new String("");
    }

    std::string msg;
    msg += "didnt expect a ";
    msg += lex->lexName;
    throw std::runtime_error(msg);
}

const Expression* Parser::scanTerm(const StrLex* lex) {
    std::string term = lex->str;

    if (term[0] == ':') {
        return new Atom(term.substr(1), true);
    }

    bool isInteger = true;
    for (auto ch : term) {
        if (ch < '0' || ch > '9') {
            isInteger = false;
            break;
        }
    }

    if (isInteger) {
        std::stringstream ss(term);
        Integer::integer_t res;
        ss >> res;
        return new Integer(res);
    }

    return new Atom(term, false);
}

template<typename Col>
const Expression* Parser::nextCol(LexType delimiter) {
    std::vector<const Expression*> expressions;
    while (true) {
        const Lex* lex = _lexer.next();
        if (lex->lexType == LexType::WS) {
            continue;
        }
        if (lex->lexType == delimiter) {
            break;
        }
        expressions.push_back(next(lex));
    }

    return new Col(expressions);
}
