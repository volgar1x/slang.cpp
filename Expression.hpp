/**
 * Antoine Chauvin © Slang
 */

#ifndef SLANG_EXPRESSION_HPP
#define SLANG_EXPRESSION_HPP

#include <string>

enum class Expressions {
    ATOM,
    FUNCTION,
    INTEGER,
    LIST,
    NIL,
    SET,
    STRING,
    VECTOR,
};


class Expression {
public:
    virtual ~Expression() {}

    virtual std::string getName() const = 0;
    virtual Expressions getType() const = 0;
};

#endif //SLANG_EXPRESSION_HPP
