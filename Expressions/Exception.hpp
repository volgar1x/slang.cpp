/**
 * Antoine Chauvin © Slang
 */

#ifndef SLANG_EXCEPTION_HPP
#define SLANG_EXCEPTION_HPP

#include "../Expression.hpp"

class Exception : public Expression {
public:
    std::string cause;

    Exception(const std::string& cause)
            : cause(cause)
            {}


    virtual std::string getName() const { return "EXCEPTION"; }

    virtual Expressions getType() const { return Expressions::EXCEPTION; }

    virtual std::unique_ptr<const Expression> copy() const {
        return std::unique_ptr<const Expression>(new Exception(cause));
    }
};

#endif //SLANG_EXCEPTION_HPP
