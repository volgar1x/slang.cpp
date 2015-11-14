/**
 * Antoine Chauvin © Slang
 */

#ifndef SLANG_INTERPRETER_HPP
#define SLANG_INTERPRETER_HPP

#include <map>
#include "Expression.hpp"
#include "Expressions/List.hpp"
#include "Expressions/Function.hpp"

class InterpreterContext {
public:
    typedef std::map<std::string, const Expression*> map_t;

    InterpreterContext();
    InterpreterContext(const InterpreterContext* parent);
    virtual ~InterpreterContext();

    const Expression* get(std::string key) const;
    void set(std::string key, const Expression* value);

private:
    const InterpreterContext* parent;
    map_t map;
};

class Interpreter : public InterpreterContext {
public:
    typedef Function<Interpreter> Fun;
    typedef NativeFunction<Interpreter> NaFun;

    Interpreter();
    Interpreter(const InterpreterContext* parent);
    virtual ~Interpreter();

    const Expression* interpret(const Expression* exp);

private:
    const Expression* let(const List* parameters);
};

std::string toString(const Expression* exp);

const Expression* print(Interpreter& context, const List* parameters);
const Expression* println(Interpreter& context, const List* parameters);
const Expression* readln(Interpreter& context, const List* parameters);
const Expression* plus(Interpreter& context, const List* parameters);

#endif //SLANG_INTERPRETER_HPP
