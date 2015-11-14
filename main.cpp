#include <iostream>
#include <fstream>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"

using namespace std;

int main() {
    ifstream f("Source.slang");
    Lexer lexer(f);
//    while (true) {
//        const Lex* lex = lexer.next();
//        if (lex->lexName == "LEX") {
//            cout << "\"" << static_cast<const StrLex*>(lex)->str << "\"";
//            delete lex;
//        } else if (lex->lexName == "EOF") {
//            break;
//        } else {
//            cout << lex->lexName;
//        }
//        cout << endl;
//    }
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