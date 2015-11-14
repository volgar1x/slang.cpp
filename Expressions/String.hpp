/**
 * Antoine Chauvin © Slang
 */

#ifndef SLANG_STRING_HPP
#define SLANG_STRING_HPP

#include "../Expression.hpp"

class String : public Expression {
public:
    const std::string str;

    String(const std::string& str) : str(str) {}

    virtual std::string getName() const { return "STRING"; }
    virtual Expressions getType() const { return Expressions::STRING; }

    virtual std::unique_ptr<const Expression> copy() const {
        return std::unique_ptr<const Expression>(new String(str));
    }
};

#endif //SLANG_STRING_HPP
