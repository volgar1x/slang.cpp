/**
 * Antoine Chauvin © Slang
 */

#ifndef SLANG_LIST_HPP
#define SLANG_LIST_HPP

#include "../Expression.hpp"
#include <vector>

class List : public Expression {
public:
    typedef std::vector<const Expression*> values_t;

    const values_t values;

    List() : values() {}
    List(const values_t values) : values(values) {}

    virtual std::string getName() const { return "LIST"; }
    virtual Expressions getType() const { return Expressions::LIST; }
};

#endif //SLANG_LIST_HPP
