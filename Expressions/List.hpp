/**
 * Antoine Chauvin © Slang
 */

#ifndef SLANG_LIST_HPP
#define SLANG_LIST_HPP

#include "../Expression.hpp"
#include <vector>

class List : public Expression {
public:
    typedef std::vector<std::shared_ptr<const Expression>> values_t;

    const values_t values;

    List() : values() {}
    List(const values_t& values) : values(values) {}

    virtual std::string getName() const { return "LIST"; }
    virtual Expressions getType() const { return Expressions::LIST; }

    virtual std::unique_ptr<const Expression> copy() const {
        values_t newValues;
        for (auto value : values) {
            newValues.push_back(value->copy());
        }
        return std::unique_ptr<const Expression>(new List(newValues));
    }

    virtual bool coerceBoolean() const { return !values.empty(); }
};

#endif //SLANG_LIST_HPP
