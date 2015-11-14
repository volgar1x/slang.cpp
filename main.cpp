#include <iostream>
#include <fstream>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"

using namespace std;

int main() {
    ifstream f("Source.slang");

    Lexer lexer(f);
    Parser parser(lexer);
    Interpreter interpreter;

    while (true) {
        const Expression* expression = parser.next();
        if (expression->getType() == Expressions::NIL) {
            break;
        }
        const Expression* result = interpreter.interpret(expression);
        cout << endl << "> " << toString(result) << endl;
    }

    return 0;
}