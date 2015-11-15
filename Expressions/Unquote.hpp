/**
 * Antoine Chauvin © Slang
 */

#ifndef SLANG_UNQUOTE_HPP
#define SLANG_UNQUOTE_HPP

#include "../Expression.hpp"


class Unquote : public Expression {
public:
    const std::string name;

    Unquote(const std::string& name)
            : name(name)
            {}


    virtual std::string getName() const { return "UNQUOTE"; }

    virtual Expressions getType() const { return Expressions::UNQUOTE; }

    virtual std::unique_ptr<const Expression> copy() const {
        return std::unique_ptr<const Expression>(new Unquote(name));
    }
};


#endif //SLANG_UNQUOTE_HPP
