cmake_minimum_required(VERSION 3.3)
project(slang)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Lexer.cpp Lexer.hpp Lex.cpp Lex.hpp Parser.cpp Parser.hpp Expressions/List.hpp Expressions/Vector.hpp Expressions/Set.hpp Expressions/Integer.hpp Expressions/String.hpp Expressions/Atom.hpp Expression.hpp Expressions.hpp Expressions/Nil.hpp Interpreter.cpp Interpreter.hpp Expressions/Function.hpp Expressions/Exception.hpp MacroTransformer.cpp MacroTransformer.hpp Expressions/Quote.hpp)
add_executable(slang ${SOURCE_FILES})