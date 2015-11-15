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

                return std::unique_ptr<const Expression>(new Nil);
            }
        }

        return onePass(list.values);
    }

    return expression.copy();
}

std::unique_ptr<const Expression> MacroTransformer::onePass(const List::values_t& values) {
    auto atom = std::dynamic_pointer_cast<const Atom>(values[0]);
    if (!atom) {
        return std::unique_ptr<const Expression>(new List(values));
    }

    if (containsKey(atom->atom)) {
        auto expression = static_cast<const UserFunction&>(get(atom->atom));
        Interpreter interpreter(this);
        load_stdlib(interpreter);
        List::values_t parameters(values.begin() + 1, values.end());
        auto result = expression.call(interpreter, parameters);
        InterpreterContext unquoteContext;
        expression.mapNames(unquoteContext, parameters);
        return unquote(*result, unquoteContext);
    }

    List::values_t result;
    result.push_back(atom);
    for (auto it = values.begin() + 1; it != values.end(); it++) {
        auto expression = *it;
        if (auto list = std::dynamic_pointer_cast<const List>(expression)) {
            result.push_back(onePass(list->values));
        } else {
            result.push_back(expression);
        }
    }
    return std::unique_ptr<const Expression>(new List(result));
}

std::unique_ptr<const Expression> MacroTransformer::unquote(const Expression& expression, const InterpreterContext& context) {
    switch (expression.getType()) {
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
    List::values_t result;
    for (auto value : values) {
        if (auto atom = std::dynamic_pointer_cast<const Atom>(value)) {
            if (atom->atom[0] == '#') {
                auto key = atom->atom.substr(1);
                result.push_back(context.get(key).copy());
            } else {
                result.push_back(value->copy());
            }
        } else if (auto list = std::dynamic_pointer_cast<const List>(value)) {
            result.push_back(unquoteCol<List>(list->values, context));
        } else if (auto set = std::dynamic_pointer_cast<const Set>(value)) {
            result.push_back(unquoteCol<Set>(set->values, context));
        } else if (auto vector = std::dynamic_pointer_cast<const Vector>(value)) {
            result.push_back(unquoteCol<Vector>(vector->values, context));
        } else {
            result.push_back(value->copy());
        }
    }
    return std::unique_ptr<const Expression>(new Col(result));
}
