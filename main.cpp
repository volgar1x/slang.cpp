#include <iostream>
#include <fstream>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"
#include "MacroTransformer.hpp"
#include "Expressions/Exception.hpp"

using namespace std;

int main() {
    ifstream f("Source.slang");

    Lexer lexer(f);
    Parser parser(lexer);
    MacroTransformer macroTransformer;
    Interpreter interpreter;

    while (true) {
        std::unique_ptr<const Expression> expression = parser.next();
        if (expression->getType() == Expressions::NIL) {
            break;
        }
        try {
            std::unique_ptr<const Expression> transformed = macroTransformer.transform(*expression);
            std::unique_ptr<const Expression> result = interpreter.interpret(*transformed);
            cout << endl << "> " << toString(*result) << endl;
        } catch (const Exception& exception) {
            cerr << endl << "Unexpected exception: " << exception.cause << endl;
        }
    }

    return 0;
}