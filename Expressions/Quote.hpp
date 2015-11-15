/**
 * Antoine Chauvin © Slang
 */

#ifndef SLANG_QUOTE_HPP
#define SLANG_QUOTE_HPP

#include "../Expression.hpp"


class Quote : public Expression {
public:
    const std::unique_ptr<const Expression> expression;

    Quote(std::unique_ptr<const Expression> expression)
            : expression(std::move(expression))
            {}


    virtual std::string getName() const { return "QUOTE"; }
    virtual Expressions getType() const { return Expressions::QUOTE; }

    virtual std::unique_ptr<const Expression> copy() const {
        return std::unique_ptr<const Expression>(new Quote(expression->copy()));
    }
};


#endif//SLANG_QUOTE_HPP
