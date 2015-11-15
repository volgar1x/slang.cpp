/**
 * Antoine Chauvin © Slang
 */

#ifndef SLANG_MACRO_HPP
#define SLANG_MACRO_HPP

#include "Interpreter.hpp"


class MacroTransformer : public InterpreterContext {
private:

public:
    std::unique_ptr<const Expression> transform(const Expression& expression);
    std::unique_ptr<const Expression> onePass(const List::values_t& values);
    std::unique_ptr<const Expression> unquote(const Expression& expression, const InterpreterContext& context);
    template<typename Col>
    std::unique_ptr<const Expression> unquoteCol(const List::values_t& values, const InterpreterContext& context);
};


#endif//SLANG_MACRO_HPP
