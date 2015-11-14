/**
 * Antoine Chauvin © Slang
 */

#include <exception>
#include "Lexer.hpp"

Lexer::Lexer(std::istream& is)
    : _is(is)
    , _inStr(false)
    { }

void Lexer::ignoreLine() {
    char c;
    while (_is.get(c) && c != '\n');
}

void Lexer::ignoreWhitespaces() {
    char c;
    while (_is.get(c) && isWhitespacePart(c));
    _is.putback(c);
}

bool Lexer::isWhitespacePart(char c) {
    return c == '\0' || c == ' ' || c == '\n';
}

bool Lexer::isEndLex(char c) {
    switch (c) {
        case ')':
        case ']':
        case '}':
            return true;

        default:
            return false;
    }
}

std::string Lexer::untilWhitespace() {
    char c;
    std::string s;
    while (_is.get(c)) {
        if (isWhitespacePart(c) || isEndLex(c)) {
            _is.putback(c);
            break;
        }
        s += c;
    }
    return s;
}

const Lex* Lexer::next() {
    if (_inStr && _is.peek() != '"') {
        return nextStr();
    }

    char c;

    if (!_is.get(c)) {
        return new Lex(LexType::EOFF);
    }

    if (isWhitespacePart(c)) {
        ignoreWhitespaces();
        return new Lex(LexType::WS);
    }

    switch (c) {
        case ';':
            ignoreLine();
            return next();

        case '(':
            return new Lex(LexType::LST_START);

        case ')':
            return new Lex(LexType::LST_END);

        case '[':
            return new Lex(LexType::VEC_START);

        case ']':
            return new Lex(LexType::VEC_END);

        case '{':
            return new Lex(LexType::SET_START);

        case '}':
            return new Lex(LexType::SET_END);

        case '"':
            _inStr = !_inStr;
            return new Lex(LexType::STR);

        case '\'':
            return new Lex(LexType::CHR);

        default:
            _is.putback(c);
            return new StrLex(untilWhitespace());
    }
}

const Lex* Lexer::nextStr() {
    std::string s;
    char c;

    while (_is.get(c) && c != '"') {
        s += c;
    }
    _is.putback('"');

    return new StrLex(s);
}

void Lexer::expect(LexType lexType) {
    const Lex* lex = next();
    if (lexType != lex->lexType) {
        std::string msg;
        msg += "expected ";
        msg += toString(lexType);
        msg += " but got ";
        msg += toString(lex->lexType);
        throw std::runtime_error(msg);
    }
}