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

bool InterpreterContext::containsKey(const std::string& key) const {
    auto it = map.find(key);
    return it != map.end();
}

const Expression& InterpreterContext::get(const std::string& key) const {
    auto it = map.find(key);
    if (it != map.end()) {
        return *it->second;
    }
    if (parent != nullptr) {
        return parent->get(key);
    }
    throw std::runtime_error("cannot find variable `" + key + "'");
}

void InterpreterContext::set(const std::string& key, std::unique_ptr<const Expression> value) {
    map[key] = std::move(value);
}

Interpreter::Interpreter()
    : InterpreterContext()
{
    load_stdlib(*this);
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

        case Expressions::EXCEPTION:
            ss << static_cast<const Exception&>(exp).cause;
            break;

        case Expressions::INTEGER:
            ss << static_cast<const Integer&>(exp).integer;
            break;

        case Expressions::NIL:
            ss << "nil";
            break;

        case Expressions::QUOTE:
            ss << "'";
            ss << toString(*static_cast<const Quote&>(exp).expression);
            break;

        case Expressions::UNQUOTE:
            ss << '#';
            ss << static_cast<const Unquote&>(exp).name;
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
        case Expressions::EXCEPTION:
            return exp.copy();

        case Expressions::ATOM: {
            auto atom = static_cast<const Atom&>(exp);
            if (atom.value) {
                return atom.copy();
            }
            return get(atom.atom).copy();
        }

        case Expressions::QUOTE:
            return static_cast<const Quote&>(exp).expression->copy();

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

        case Expressions::UNQUOTE:
            throw Exception("cannot unquote here");
    }
}

template<class ExprItr>
std::unique_ptr<const Expression> Interpreter::interpretAll(ExprItr begin, ExprItr end) {
    std::unique_ptr<const Expression> result(new Nil);
    for (auto it = begin; it != end; it++) {
        result = interpret(**it);
    }
    return result;
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
    auto atom = std::static_pointer_cast<const Atom>(values[0]);
    const Fun& function = static_cast<const Fun&>(get(atom->atom));

    List::values_t parameters(values.begin() + 1, values.end());

    if (function.requireParameterEvaluation()) {
        for (auto& parameter : parameters) {
            parameter = interpret(*parameter);
        }
    }

    return function.call(*this, parameters);
}


void load_stdlib(Interpreter& context) {
    context.set("print", std::unique_ptr<const Expression>(new Interpreter::NaFun("print", &print, false)));
    context.set("println", std::unique_ptr<const Expression>(new Interpreter::NaFun("println", &println, false)));
    context.set("readln", std::unique_ptr<const Expression>(new Interpreter::NaFun("readln", &readln, false)));
    context.set("+", std::unique_ptr<const Expression>(new Interpreter::NaFun("+", &plus, false)));
    context.set("-", std::unique_ptr<const Expression>(new Interpreter::NaFun("-", &minus, false)));
    context.set("not", std::unique_ptr<const Expression>(new Interpreter::NaFun("not", &not_fun, false)));
    context.set(">", std::unique_ptr<const Expression>(new Interpreter::NaFun(">", &sp, false)));
    context.set("let", std::unique_ptr<const Expression>(new Interpreter::NaFun("let", &let_macro, true)));
    context.set("defun", std::unique_ptr<const Expression>(new Interpreter::NaFun("defun", &defun_macro, true)));
    context.set("case", std::unique_ptr<const Expression>(new Interpreter::NaFun("case", &case_macro, true)));
    context.set("do", std::unique_ptr<const Expression>(new Interpreter::NaFun("do", &do_macro, true)));
    context.set("try", std::unique_ptr<const Expression>(new Interpreter::NaFun("try", &try_macro, true)));
    context.set("raise", std::unique_ptr<const Expression>(new Interpreter::NaFun("raise", &raise_macro, true)));
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

std::unique_ptr<const Expression> minus(Interpreter& context, const List::values_t& parameters) {
    Integer::integer_t acc = std::static_pointer_cast<const Integer>(parameters[0])->integer;
    for (auto it = parameters.begin() + 1; it != parameters.end(); ++it) {
        acc -= std::static_pointer_cast<const Integer>(*it)->integer;
    }
    return std::unique_ptr<const Expression>(new Integer(acc));
}

std::unique_ptr<const Expression> sp(Interpreter& context, const List::values_t& parameters) {
    auto lhs = std::static_pointer_cast<const Integer>(parameters[0]);
    auto rhs = std::static_pointer_cast<const Integer>(parameters[1]);
    if (lhs->integer > rhs->integer) {
        return std::unique_ptr<const Expression>(new Atom("true", true));
    } else {
        return std::unique_ptr<const Expression>(new Nil);
    }
}

std::unique_ptr<const Expression> not_fun(Interpreter& context, const List::values_t& parameters) {
    if (parameters[0]->coerceBoolean()) {
        return std::unique_ptr<const Expression>(new Nil);
    } else {
        return std::unique_ptr<const Expression>(new Atom("true", true));
    }
}

std::unique_ptr<const Expression> let_macro(Interpreter& context, const List::values_t& parameters) {
    Interpreter child(&context);

    std::shared_ptr<const Vector> bindings = std::static_pointer_cast<const Vector>(parameters[0]);
    auto it = bindings->values.begin();
    while (it != bindings->values.end()) {
        std::shared_ptr<const Atom> name = std::static_pointer_cast<const Atom>(*it);
        it++;
        std::shared_ptr<const Expression> value = *it;
        it++;

        child.set(name->atom, context.interpret(*value));
    }

    return child.interpretAll(parameters.begin() + 1, parameters.end());
}

std::unique_ptr<const Expression> defun_macro(Interpreter& context, const List::values_t& parameters) {
    std::shared_ptr<const Atom> atom = std::static_pointer_cast<const Atom>(parameters[0]);
    std::shared_ptr<const Vector> vector = std::static_pointer_cast<const Vector>(parameters[1]);

    std::string name = atom->atom;
    std::vector<std::string> arguments;
    for (auto expression : vector->values) {
        std::shared_ptr<const Atom> atomExpression = std::static_pointer_cast<const Atom>(expression);
        arguments.push_back(atomExpression->atom);
    }

    List::values_t rest(parameters);
    rest.erase(rest.begin(), rest.begin() + 2);

    context.set(name, std::unique_ptr<const Expression>(new UserFunction(name, rest, arguments)));

    return atom->copyAsValue(true);
}

std::unique_ptr<const Expression> case_macro(Interpreter& context, const List::values_t& parameters) {
    for (auto parameter : parameters) {
        auto caseItem = std::static_pointer_cast<const List>(parameter);
        auto test = context.interpret(*caseItem->values[0]);
        if (test->coerceBoolean()) {
            return context.interpret(*caseItem->values[1]);
        }
    }
    return std::unique_ptr<const Expression>(new Nil);
}

std::unique_ptr<const Expression> do_macro(Interpreter& context, const List::values_t& parameters) {
    return context.interpretAll(parameters.begin(), parameters.end());
}

std::unique_ptr<const Expression> try_macro(Interpreter& context, const List::values_t& parameters) {
    try {
        Interpreter child(&context);
        return child.interpret(*parameters[0]);
    } catch (const Exception& exception) {
        auto branch = std::static_pointer_cast<const List>(parameters[1]);
        auto arguments = std::static_pointer_cast<const Vector>(branch->values[0]);
        auto argument = std::static_pointer_cast<const Atom>(arguments->values[0]);
        Interpreter child(&context);
        child.set(argument->atom, exception.copy());
        return child.interpretAll(branch->values.begin() + 1, branch->values.end());
    }
}

std::unique_ptr<const Expression> raise_macro(Interpreter& context, const List::values_t& parameters) {
    std::string cause;
    if (parameters.empty()) {
        cause = "empty";
    } else {
        for (auto parameter : parameters) {
            auto expression = context.interpret(*parameter);
            cause += toString(*expression);
        }
    }

    throw Exception(cause);
}

bool UserFunction::requireParameterEvaluation() const {
    return true;
}

std::unique_ptr<const Expression> UserFunction::call(Interpreter& scope, const List::values_t& parameters) const {
    Interpreter child(&scope);
    mapNames(child, parameters);

    return child.interpretAll(expressions.begin(), expressions.end());
}

std::unique_ptr<const Expression> UserFunction::copy() const {
    return std::unique_ptr<const Expression>(new UserFunction(this->functionName, expressions, arguments));
}

void UserFunction::mapNames(InterpreterContext& context, const List::values_t& parameters) const {
    for (int i = 0; i < arguments.size(); i++) {
        context.set(arguments[i], parameters[i]->copy());
    }
}

