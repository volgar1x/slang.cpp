/**
 * Antoine Chauvin © Slang
 */

#ifndef SLANG_ATOM_HPP
#define SLANG_ATOM_HPP

#include "../Expression.hpp"

class Atom : public Expression {
public:
    const std::string atom;
    const bool value;

    Atom(const std::string atom, bool value) : atom(atom), value(value) {}

    virtual std::string getName() const { return "ATOM"; }
    virtual Expressions getType() const { return Expressions::ATOM; }
};

#endif //SLANG_ATOM_HPP
