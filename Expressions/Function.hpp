/**
 * Antoine Chauvin © Slang
 */

#ifndef SLANG_FUNCTION_HPP
#define SLANG_FUNCTION_HPP

#include <functional>
#include "../Expression.hpp"
#include "List.hpp"

class Function : public Expression {
public:
    const std::string functionName;

    Function(std::string functionName)
            : functionName(functionName)
    {}

    virtual ~Function() {}

    virtual std::string getName() const { return "FUNCTION"; }
    virtual Expressions getType() const { return Expressions::FUNCTION; }
};

class NativeFunction : public Function {
public:
    typedef std::function<const Expression*(const List*)> function_t;

    const function_t functionPtr;
    const bool macro;

    NativeFunction(std::string functionName, const function_t functionPtr, bool macro)
            : Function(functionName)
            , functionPtr(functionPtr)
            , macro(macro)
    {}

    virtual ~NativeFunction() {}

private:
};

#endif //SLANG_FUNCTION_HPP
