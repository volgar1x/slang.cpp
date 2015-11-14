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

const Expression& InterpreterContext::get(std::string key) const {
    auto it = map.find(key);
    if (it != map.end()) {
        return *it->second;
    }
    if (parent != nullptr) {
        return parent->get(key);
    }
    throw std::runtime_error("cannot find variable `" + key + "'");
}

void InterpreterContext::set(std::string key, std::unique_ptr<const Expression> value) {
    map[key] = std::move(value);
}

Interpreter::Interpreter()
    : InterpreterContext()
{
    using namespace std::placeholders;
    set("print", std::unique_ptr<const Expression>(new NaFun("print", &print, false)));
    set("println", std::unique_ptr<const Expression>(new NaFun("println", &println, false)));
    set("readln", std::unique_ptr<const Expression>(new NaFun("readln", &readln, false)));
    set("+", std::unique_ptr<const Expression>(new NaFun("+", &plus, false)));
    set("let", std::unique_ptr<const Expression>(new NaFun("let", &Interpreter::let, true)));
}

Interpreter::Interpreter(const InterpreterContext* parent)
    : InterpreterContext(parent)
    { }

Interpreter::~Interpreter() {

}

std::string toString(const Expression& exp) {
    std::stringstream ss;

    switch (exp.getType()) {
        case Expressions::LIST: {
            const List& list = static_cast<const List&>(exp);
            ss << "(";
            for (std::shared_ptr<const Expression> child : list.values) {
                ss << toString(*child);
                ss << " ";
            }
            ss << ")";
            break;
        }


        case Expressions::VECTOR: {
            const Vector& vector = static_cast<const Vector&>(exp);
            ss << "[";
            for (std::shared_ptr<const Expression> child : vector.values) {
                ss << toString(*child);
                ss << " ";
            }
            ss << "]";
            break;
        }

        case Expressions::SET: {
            const Set& set = static_cast<const Set&>(exp);
            ss << "{";
            for (std::shared_ptr<const Expression> child : set.values) {
                ss << toString(*child);
                ss << " ";
            }
            ss << "}";
            break;
        }

        case Expressions::ATOM: {
            const Atom& atom = static_cast<const Atom&>(exp);
            if (atom.value) {
                ss << ":";
            }
            ss << atom.atom;
            break;
        }

        case Expressions::STRING:
            ss << static_cast<const String&>(exp).str;
            break;

        case Expressions::INTEGER:
            ss << static_cast<const Integer&>(exp).integer;
            break;

        case Expressions::NIL:
            ss << "nil";
            break;

        default:
            ss << exp.getName();
            break;
    }
    return ss.str();
}

std::unique_ptr<const Expression> Interpreter::interpret(const Expression& exp) {
    switch (exp.getType()) {
        case Expressions::FUNCTION:
        case Expressions::INTEGER:
        case Expressions::NIL:
        case Expressions::SET:
        case Expressions::STRING:
        case Expressions::VECTOR:
            return exp.copy();

        case Expressions::ATOM:
            return get(static_cast<const Atom&>(exp).atom).copy();

        case Expressions::LIST: {
            List::values_t values = static_cast<const List&>(exp).values;
            if (values.empty()) {
                return std::unique_ptr<const Expression>(new Nil);
            }

            std::shared_ptr<const Atom> atom = std::static_pointer_cast<const Atom>(values[0]);
            if (atom->value) {
                std::unique_ptr<const Expression> set = interpret(*values[1]);
                return accessSet(atom->atom, static_cast<const Set*>(set.get())->values);
            } else {
                return callFunction(values);
            }
        }
    }
}

std::unique_ptr<const Expression> Interpreter::accessSet(const std::string& key, const List::values_t& values) {
    auto it = values.begin();
    while (it != values.end()) {
        std::shared_ptr<const Atom> k = std::static_pointer_cast<const Atom>(*it);
        it++;
        if (it == values.end()) {
            break;
        }
        std::shared_ptr<const Expression> v = *it;
        it++;

        if (k->atom == key) {
            return v->copy();
        }
    }
    return std::unique_ptr<const Expression>(new Nil);
}

std::unique_ptr<const Expression> Interpreter::callFunction(const List::values_t& values) {
    std::shared_ptr<const Atom> atom = std::static_pointer_cast<const Atom>(values[0]);
    const Fun& function = static_cast<const Fun&>(get(atom->atom));

    List::values_t parameters(values);
    parameters.erase(parameters.begin());

    if (function.requireParameterEvaluation()) {
        for (auto it = parameters.begin(); it != parameters.end(); it++) {
            *it = interpret(**it);
        }
    }

    return function.call(*this, parameters);
}

std::unique_ptr<const Expression> print(Interpreter& context, const List::values_t& parameters) {
    for (std::shared_ptr<const Expression> parameter : parameters) {
        std::cout << toString(*parameter);
    }
    std::cout.flush();
    return std::unique_ptr<const Expression>(new Nil);
}

std::unique_ptr<const Expression> println(Interpreter& context, const List::values_t& parameters) {
    print(context, parameters);
    std::cout << std::endl;
    return std::unique_ptr<const Expression>(new Nil);
}

std::unique_ptr<const Expression> readln(Interpreter& context, const List::values_t& parameters) {
    print(context, parameters);
    std::string line;
    std::cin >> line;
    return std::unique_ptr<const Expression>(new String(line));
}

std::unique_ptr<const Expression> plus(Interpreter& context, const List::values_t& parameters) {
    Integer::integer_t acc = 0;
    for (std::shared_ptr<const Expression> parameter : parameters) {
        if (std::shared_ptr<const Integer> integer = std::dynamic_pointer_cast<const Integer>(parameter)) {
            acc += integer->integer;
        }
    }
    return std::unique_ptr<const Expression>(new Integer(acc));
}

std::unique_ptr<const Expression> Interpreter::let(const List::values_t& parameters) {
    Interpreter child(this);

    std::shared_ptr<const Vector> bindings = std::static_pointer_cast<const Vector>(parameters[0]);
    auto it = bindings->values.begin();
    while (it != bindings->values.end()) {
        std::shared_ptr<const Atom> name = std::static_pointer_cast<const Atom>(*it);
        it++;
        std::shared_ptr<const Expression> value = *it;
        it++;

        child.set(name->atom, interpret(*value));
    }

    std::unique_ptr<const Expression> result = std::unique_ptr<const Expression>(new Nil);
    for (int i = 1; i < parameters.size(); i++) {
        result = child.interpret(*parameters[i]);
    }

    return result;
}
