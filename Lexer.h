//
// Created by ramon on 10/12/2017.
//

#ifndef PLANGCOMPILER_LEXER_H
#define PLANGCOMPILER_LEXER_H

#include <vector>
#include <string>
#include "CommonTypes.h"

using namespace std;

extern const char * TokenTypeString[];

class Lexer {
public:
    vector<Token> tokens;

public:
    void PrintTokens();

    void MakeToken(const string *source, TokenType tokenType, int startIndex, int length);
    void Tokenize(const string *source);

};

#endif //PLANGCOMPILER_LEXER_H
