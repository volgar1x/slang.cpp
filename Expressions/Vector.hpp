/**
 * Antoine Chauvin © Slang
 */

#ifndef SLANG_VECTOR_HPP
#define SLANG_VECTOR_HPP

#include "../Expression.hpp"
#include <vector>

class Vector : public Expression {
public:
    const std::vector<const Expression*> values;

    Vector(const std::vector<const Expression*> values) : values(values) {}

    virtual std::string getName() const { return "VECTOR"; }
    virtual Expressions getType() const { return Expressions::VECTOR; }
};

#endif //SLANG_VECTOR_HPP
