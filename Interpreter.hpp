/**
 * Antoine Chauvin © Slang
 */

#ifndef SLANG_INTERPRETER_HPP
#define SLANG_INTERPRETER_HPP

#include <map>
#include "Expression.hpp"
#include "Expressions/List.hpp"

class InterpreterContext {
public:
    typedef std::map<std::string, const Expression*> map_t;

    InterpreterContext();
    InterpreterContext(InterpreterContext* parent);
    virtual ~InterpreterContext();

    const Expression* get(std::string key) const;
    void set(std::string key, const Expression* value);

private:
    map_t::const_iterator lookup(std::string key) const;
    map_t::iterator lookup(std::string key);

    InterpreterContext* parent;
    map_t map;
};

class Interpreter {
public:
    Interpreter();
    virtual ~Interpreter();

    const Expression* interpret(const Expression* exp);
    std::string toString(const Expression* exp);

private:
    InterpreterContext* currentContext;

    const Expression* print(const List* parameters);
    const Expression* println(const List* parameters);
    const Expression* plus(const List* parameters);
    const Expression* let(const List* parameters);
};

#endif //SLANG_INTERPRETER_HPP
