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
    virtual std::unique_ptr<const Expression> copy() const = 0;
    virtual bool coerceBoolean() const { return true; }
};

#endif //SLANG_EXPRESSION_HPP
