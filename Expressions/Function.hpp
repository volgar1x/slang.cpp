/**
 * Antoine Chauvin © Slang
 */

#ifndef SLANG_FUNCTION_HPP
#define SLANG_FUNCTION_HPP

#include <functional>
#include "../Expression.hpp"
#include "List.hpp"

template<typename Scope>
class Function : public Expression {
public:
    const std::string functionName;

    Function(std::string functionName)
            : functionName(functionName)
    {}

    virtual ~Function() {}

    virtual std::string getName() const { return "FUNCTION"; }
    virtual Expressions getType() const { return Expressions::FUNCTION; }

    virtual const Expression* call(Scope& scope, const List* parameters) const = 0;
    virtual bool requireParameterEvaluation() const = 0;
};

template<typename Scope>
class NativeFunction : public Function<Scope> {
public:
    typedef std::function<const Expression*(Scope& scope, const List*)> function_t;

    const function_t functionPtr;
    const bool macro;

    NativeFunction(std::string functionName, const function_t functionPtr, bool macro)
            : Function<Scope>(functionName)
            , functionPtr(functionPtr)
            , macro(macro)
    {}

    virtual ~NativeFunction() {}

    virtual const Expression* call(Scope& scope, const List* parameters) const {
        return functionPtr(scope, parameters);
    }

    virtual bool requireParameterEvaluation() const {
        return !macro;
    }

private:
};

#endif //SLANG_FUNCTION_HPP
