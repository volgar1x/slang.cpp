/**
 * Antoine Chauvin © Slang
 */

#ifndef SLANG_SET_HPP
#define SLANG_SET_HPP

#include "../Expression.hpp"
#include <vector>

class Set : public Expression {
public:
    typedef std::vector<std::shared_ptr<const Expression>> values_t;

    const values_t values;

    Set() : values() {}
    Set(const values_t& values) : values(values) {}

    virtual std::string getName() const { return "SET"; }
    virtual Expressions getType() const { return Expressions::SET; }

    virtual std::unique_ptr<const Expression> copy() const {
        values_t newValues;
        for (auto value : values) {
            newValues.push_back(value->copy());
        }
        return std::unique_ptr<const Expression>(new Set(newValues));
    }
};

#endif //SLANG_SET_HPP
