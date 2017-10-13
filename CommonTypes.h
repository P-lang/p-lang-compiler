//
// Created by ramon on 10/6/2017.
//

#ifndef PLANGCOMPILER_COMMONTYPES_H
#define PLANGCOMPILER_COMMONTYPES_H

#include <iostream>

using namespace std;

void Log(string message);

enum TokenType {
    NUMBER,
    SPACE,
    TAB,
    WORD,
    END_OF_LINE,
    OPERATOR_EQUALS,
    OPERATOR_PLUS,
    OPERATOR_MINUS,
    OPERATOR_MULTIPLY,
    OPERATOR_DIVIDE,
    OPERATOR_MORE,
    OPERATOR_LESS,
    OPERATOR_MORE_OR_EQUAL,
    OPERATOR_LESS_OR_EQUAL,
    OPERATOR_PLUS_EQUALS,
    OPERATOR_MINUS_EQUALS,
    OPERATOR_MULTIPLY_EQUALS,
    OPERATOR_DIVIDE_EQUALS,
    OPERATOR_NOT_EQUALS,
    OPERATOR_NOT,
    COMMENT,
    OPERATOR_LBRACE,
    OPERATOR_RBRACE,
    EOF_DOLLAR
};

struct Token {
    TokenType tokenType;
    string value;
};


#endif //PLANGCOMPILER_COMMONTYPES_H
