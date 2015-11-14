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

std::unique_ptr<const Expression> Parser::next() {
    while (true) {
        std::unique_ptr<const Lex> lex = _lexer.next();
        if (lex->lexType != LexType::WS) {
            return next(*lex);
        }
    }
}

std::unique_ptr<const Expression> Parser::next(const Lex& lex) {
    switch (lex.lexType) {
        case LexType::STR:
            return nextString();

        case LexType::LEX:
            return scanTerm(static_cast<const StrLex&>(lex).str);

        case LexType::LST_START:
            return nextCol<List>(LexType::LST_END);

        case LexType::VEC_START:
            return nextCol<Vector>(LexType::VEC_END);

        case LexType::SET_START:
            return nextCol<Set>(LexType::SET_END);

        default:
            return std::unique_ptr<const Expression>(new Nil);
    }
}

std::unique_ptr<const Expression> Parser::nextString() {
    std::unique_ptr<const Lex> lex = _lexer.next();
    if (lex->lexType == LexType::LEX) {
        _lexer.expect(LexType::STR);
        const StrLex* strLex = static_cast<const StrLex*>(lex.get());
        return std::unique_ptr<const Expression>(new String(strLex->str));
    } else if (lex->lexType == LexType::STR) {
        return std::unique_ptr<const Expression>(new String(""));
    }

    throw std::runtime_error("didnt expect a " + toString(lex->lexType));
}

std::unique_ptr<const Expression> Parser::scanTerm(const std::string& term) {
    if (term == "nil") {
        return std::unique_ptr<const Expression>(new Nil);
    }

    if (term[0] == ':') {
        return std::unique_ptr<const Expression>(new Atom(term.substr(1), true));
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
        return std::unique_ptr<const Expression>(new Integer(res));
    }

    return std::unique_ptr<const Expression>(new Atom(term, false));
}

template<typename Col>
std::unique_ptr<const Expression> Parser::nextCol(LexType delimiter) {
    std::vector<std::shared_ptr<const Expression>> expressions;
    while (true) {
        std::unique_ptr<const Lex> lex = _lexer.next();
        if (lex->lexType == LexType::WS) {
            continue;
        }
        if (lex->lexType == delimiter) {
            break;
        }
        expressions.push_back(next(*lex));
    }

    return std::unique_ptr<const Expression>(new Col(expressions));
}
