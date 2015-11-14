/**
 * Antoine Chauvin © Slang
 */

#include "../Expression.hpp"

#ifndef SLANG_NIL_HPP
#define SLANG_NIL_HPP

class Nil : public Expression {
public:
    Nil() {}

    virtual std::string getName() const { return "NIL"; }
    virtual Expressions getType() const { return Expressions::NIL; }

    virtual std::unique_ptr<const Expression> copy() const {
        return std::unique_ptr<const Expression>(new Nil);
    }

    virtual bool coerceBoolean() const { return false; }
};

#endif //SLANG_NIL_HPP


