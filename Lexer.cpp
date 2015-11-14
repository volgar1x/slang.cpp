/**
 * Antoine Chauvin © Slang
 */

#include <exception>
#include "Lexer.hpp"


bool isAlphanumeric(char c) {
    return (c >= '0' && c <= '9') ||
            (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z');
}


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
        return &LEX_EOF;
    }

    if (isWhitespacePart(c)) {
        ignoreWhitespaces();
        return &LEX_WS;
    }

    switch (c) {
        case ';':
            ignoreLine();
            return next();

        case '(':
            return &LEX_LST_START;

        case ')':
            return &LEX_LST_END;

        case '[':
            return &LEX_VEC_START;

        case ']':
            return &LEX_VEC_END;

        case '{':
            return &LEX_SET_START;

        case '}':
            return &LEX_SET_END;

        case '"':
            _inStr = !_inStr;
            return &LEX_STR;

        case '\'':
            return &LEX_CHR;

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

void Lexer::expect(const Lex* lex) {
    const Lex* lex2 = next();
    if (lex != lex2 && lex->lexName != lex2->lexName) {
        std::string msg;
        msg += "expected ";
        msg += lex->lexName;
        msg += " but got ";
        msg += lex2->lexName;
        throw std::runtime_error(msg);
    }
}