/**
 * Antoine Chauvin © Slang
 */

#include <sstream>
#include <iostream>
#include "Interpreter.hpp"
#include "Expressions.hpp"

InterpreterContext::InterpreterContext()
        : parent(nullptr)
        { }

InterpreterContext::InterpreterContext(const InterpreterContext* parent)
        : parent(parent)
        { }

InterpreterContext::~InterpreterContext() {

}

const Expression* InterpreterContext::get(std::string key) const {
    auto it = map.find(key);
    if (it != map.end()) {
        return it->second;
    }
    if (parent != nullptr) {
        return parent->get(key);
    }
    throw std::runtime_error("cannot find variable `" + key + "'");
}

void InterpreterContext::set(std::string key, const Expression* value) {
    map[key] = value;
}

Interpreter::Interpreter()
    : InterpreterContext()
{
    using namespace std::placeholders;
    set("print", new NaFun("print", &print, false));
    set("println", new NaFun("println", &println, false));
    set("+", new NaFun("+", &plus, false));
    set("let", new NaFun("let", &Interpreter::let, true));
}

Interpreter::Interpreter(const InterpreterContext* parent)
    : InterpreterContext(parent)
    { }

Interpreter::~Interpreter() {

}

std::string toString(const Expression* exp) {
    std::stringstream ss;

    const List* list;
    const Vector* vector;
    const Set* set;
    const Atom* atom;

    switch (exp->getType()) {
        case Expressions::LIST:
            list = static_cast<const List*>(exp);
            ss << "(";
            for (const Expression* child : list->values) {
                ss << toString(child);
                ss << " ";
            }
            ss << ")";
            break;

        case Expressions::VECTOR:
            vector = static_cast<const Vector*>(exp);
            ss << "[";
            for (const Expression* child : vector->values) {
                ss << toString(child);
                ss << " ";
            }
            ss << "]";
            break;

        case Expressions::SET:
            set = static_cast<const Set*>(exp);
            ss << "{";
            for (const Expression* child : set->values) {
                ss << toString(child);
                ss << " ";
            }
            ss << "}";
            break;

        case Expressions::ATOM:
            atom = static_cast<const Atom*>(exp);
            if (atom->value) {
                ss << ":";
            }
            ss << atom->atom;
            break;

        case Expressions::STRING:
            ss << static_cast<const String*>(exp)->str;
            break;

        case Expressions::INTEGER:
            ss << static_cast<const Integer*>(exp)->integer;
            break;

        case Expressions::NIL:
            ss << "nil";
            break;

        default:
            ss << exp->getName();
            break;
    }
    return ss.str();
}

const Expression* Interpreter::interpret(const Expression* exp) {
    switch (exp->getType()) {
        case Expressions::FUNCTION:
        case Expressions::INTEGER:
        case Expressions::NIL:
        case Expressions::SET:
        case Expressions::STRING:
        case Expressions::VECTOR:
            return exp;

        case Expressions::ATOM:
            return get(static_cast<const Atom*>(exp)->atom);

        case Expressions::LIST: {
            const List* list = static_cast<const List*>(exp);
            if (list->values.empty()) {
                return new Nil;
            }

            const Expression* hd = list->values[0];
            if (const Atom* atom = dynamic_cast<const Atom*>(hd)) {
                std::string identifier = atom->atom;
                if (atom->value) {
                    // set access
                    if (const Atom* setName = dynamic_cast<const Atom*>(list->values[1])) {
                        const Expression* setExpression = get(setName->atom);
                        if (const Set* set = dynamic_cast<const Set*>(setExpression)) {
                            auto it = set->values.begin();
                            while (it != set->values.end()) {
                                const Expression* key = *it;
                                it++;
                                const Expression* value = *it;
                                it++;

                                if (const Atom* atomKey = dynamic_cast<const Atom*>(key)) {
                                    if (atomKey->atom == identifier) {
                                        return value;
                                    }
                                }
                            }
                            return new Nil;
                        }
                    }
                } else {
                    // function call
                    const Expression* value = get(identifier);
                    if (const Fun* function = dynamic_cast<const Fun*>(value)) {
                        List::values_t parameters(list->values);
                        parameters.erase(parameters.begin());
                        if (function->requireParameterEvaluation()) {
                            for (auto it = parameters.begin(); it != parameters.end(); it++) {
                                *it = interpret(*it);
                            }
                        }
                        List* parametersList = new List(parameters);
                        const Expression* result = function->call(*this, parametersList);
                        return result;
                    }
                }
            }

            throw std::runtime_error(hd->getName() + " is not a valid function identifier");
        }
    }
}

const Expression* print(Interpreter& context, const List* parameters) {
    for (const Expression* parameter : parameters->values) {
        std::cout << toString(parameter);
    }
    std::cout.flush();
    return new Nil;
}

const Expression* println(Interpreter& context, const List* parameters) {
    print(context, parameters);
    std::cout << std::endl;
    return new Nil;
}

const Expression* plus(Interpreter& context, const List* parameters) {
    Integer::integer_t acc = 0;
    for (const Expression* parameter : parameters->values) {
        if (const Integer* integer = dynamic_cast<const Integer*>(parameter)) {
            acc += integer->integer;
        }
    }
    return new Integer(acc);
}

const Expression* Interpreter::let(const List* parameters) {
    Interpreter child(this);

    const Vector* bindings = static_cast<const Vector*>(parameters->values[0]);
    auto it = bindings->values.begin();
    while (it != bindings->values.end()) {
        const Atom* name = static_cast<const Atom*>(*it);
        it++;
        const Expression* value = *it;
        it++;

        child.set(name->atom, interpret(value));
    }

    const Expression* result = new Nil;
    for (int i = 1; i < parameters->values.size(); i++) {
        result = child.interpret(parameters->values[i]);
    }

    return result;
}
