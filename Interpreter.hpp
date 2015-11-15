/**
 * Antoine Chauvin © Slang
 */

#ifndef SLANG_INTERPRETER_HPP
#define SLANG_INTERPRETER_HPP

#include <map>
#include "Expression.hpp"
#include "Expressions/List.hpp"
#include "Expressions/Function.hpp"
#include "Expressions/Set.hpp"

class InterpreterContext {
public:
    typedef std::map<std::string, std::unique_ptr<const Expression>> map_t;

    InterpreterContext();
    InterpreterContext(const InterpreterContext* parent);

    bool containsKey(const std::string& key) const;
    const Expression& get(const std::string& key) const;
    void set(const std::string& key, std::unique_ptr<const Expression> value);

    const InterpreterContext& borrowParent(const InterpreterContext& ifAbsent) const;

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

    std::unique_ptr<const Expression> interpret(const Expression& exp);
    template<class ExprItr>
    std::unique_ptr<const Expression> interpretAll(ExprItr begin, ExprItr end);

private:
    std::unique_ptr<const Expression> accessSet(const std::string& key, const List::values_t& values);
    std::unique_ptr<const Expression> callFunction(const List::values_t& values);

};

std::string toString(const Expression& exp);

void load_stdlib(Interpreter& context);
std::unique_ptr<const Expression> print(Interpreter& context, const List::values_t& parameters);
std::unique_ptr<const Expression> println(Interpreter& context, const List::values_t& parameters);
std::unique_ptr<const Expression> readln(Interpreter& context, const List::values_t& parameters);
std::unique_ptr<const Expression> plus(Interpreter& context, const List::values_t& parameters);
std::unique_ptr<const Expression> not_fun(Interpreter& context, const List::values_t& parameters);
std::unique_ptr<const Expression> let_macro(Interpreter& context, const List::values_t& parameters);
std::unique_ptr<const Expression> defun_macro(Interpreter& context, const List::values_t& parameters);
std::unique_ptr<const Expression> case_macro(Interpreter& context, const List::values_t& parameters);
std::unique_ptr<const Expression> do_macro(Interpreter& context, const List::values_t& parameters);
std::unique_ptr<const Expression> try_macro(Interpreter& context, const List::values_t& parameters);
std::unique_ptr<const Expression> raise_macro(Interpreter& context, const List::values_t& parameters);


class UserFunction : public Function<Interpreter> {
private:
    List::values_t expressions;
    std::vector<std::string> arguments;

public:
    UserFunction(const std::string& functionName, List::values_t expressions, const std::vector<std::string>& arguments)
            : Function(functionName)
            , expressions(expressions)
            , arguments(arguments)
            { }

    virtual bool requireParameterEvaluation() const;
    virtual std::unique_ptr<const Expression> call(Interpreter& scope, const List::values_t& parameters) const;
    virtual std::unique_ptr<const Expression> copy() const;

    virtual void mapNames(InterpreterContext& context, const List::values_t& parameters) const;
};

#endif //SLANG_INTERPRETER_HPP
