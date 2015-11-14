/**
 * Antoine Chauvin © Slang
 */

#ifndef SLANG_SET_HPP
#define SLANG_SET_HPP

#include "../Expression.hpp"
#include <vector>

class Set : public Expression {
public:
    const std::vector<const Expression*> values;

    Set(const std::vector<const Expression*> values) : values(values) {}

    virtual std::string getName() const { return "SET"; }
    virtual Expressions getType() const { return Expressions::SET; }
};

#endif //SLANG_SET_HPP
