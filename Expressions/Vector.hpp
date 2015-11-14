/**
 * Antoine Chauvin © Slang
 */

#ifndef SLANG_VECTOR_HPP
#define SLANG_VECTOR_HPP

#include "../Expression.hpp"
#include <vector>

class Vector : public Expression {
public:
    typedef std::vector<std::shared_ptr<const Expression>> values_t;

    const values_t values;

    Vector() : values() {}
    Vector(const values_t& values) : values(values) {}

    virtual std::string getName() const { return "VECTOR"; }
    virtual Expressions getType() const { return Expressions::VECTOR; }

    virtual std::unique_ptr<const Expression> copy() const {
        values_t newValues;
        for (auto value : values) {
            newValues.push_back(value->copy());
        }
        return std::unique_ptr<const Expression>(new Vector(newValues));
    }
};

#endif //SLANG_VECTOR_HPP
