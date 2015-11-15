/**
 * Antoine Chauvin © Slang
 */

#include "MacroTransformer.hpp"
#include "Expressions.hpp"


std::unique_ptr<const Expression> MacroTransformer::transform(const Expression& expression) {
    if (expression.getType() == Expressions::LIST) {
        const List& list = static_cast<const List&>(expression);

        if (auto atom = std::dynamic_pointer_cast<const Atom>(list.values[0])) {
            if (atom->atom == "defmacro") {
                auto name = std::static_pointer_cast<const Atom>(list.values[1]);
                auto vector = std::static_pointer_cast<const Vector>(list.values[2]);
                std::vector<std::string> arguments;
                for (auto vectorItem : vector->values) {
                    auto vectorAtom = std::static_pointer_cast<const Atom>(vectorItem);
                    arguments.push_back(vectorAtom->atom);
                }
                List::values_t rest(list.values.begin() + 3, list.values.end());
                set(name->atom, std::unique_ptr<const Expression>(new UserFunction(name->atom, rest, arguments)));

                return name->copyAsValue(true);
            }
        }

        return unquote(list, *this);
    }

    return expression.copy();
}

std::unique_ptr<const Expression> MacroTransformer::unquote(const Expression& expression, const InterpreterContext& context) {
    switch (expression.getType()) {
        case Expressions::UNQUOTE:
            return context.get(static_cast<const Unquote&>(expression).name).copy();

        case Expressions::LIST:
            return unquoteCol<List>(static_cast<const List&>(expression).values, context);
        case Expressions::SET:
            return unquoteCol<Set>(static_cast<const Set&>(expression).values, context);
        case Expressions::VECTOR:
            return unquoteCol<Vector>(static_cast<const Vector&>(expression).values, context);

        default:
            return expression.copy();
    }
}


template<typename Col>
std::unique_ptr<const Expression> MacroTransformer::unquoteCol(const List::values_t& values, const InterpreterContext& context) {
    if (!values.empty() && values[0]->getType() == Expressions::ATOM) {
        auto atom = std::static_pointer_cast<const Atom>(values[0]);
        if (containsKey(atom->atom)) {
            Interpreter interpreter(&context);
            load_stdlib(interpreter);

            List::values_t parameters(values.begin() + 1, values.end());
            for (auto& parameter : parameters) {
                parameter = unquote(*parameter, context);
            }

            auto function = static_cast<const UserFunction&>(get(atom->atom));
            auto result = function.call(interpreter, parameters);

            InterpreterContext subContext(&context);
            function.mapNames(subContext, parameters);
            return unquote(*result, subContext);
        }
    }

    List::values_t result;

    for (auto value : values) {
        result.push_back(unquote(*value, context));
    }

    return std::unique_ptr<const Expression>(new Col(result));
}
