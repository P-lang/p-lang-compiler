//
// Created by ramon on 10/6/2017.
//

#include <iostream>
#include <vector>
#include "Parser.h"

#include "CommonTypes.h"

using namespace std;

void Log(string message);

int currentToken = 0;

bool Match_START(vector<Token> &tokens) {
    Node_VarDec* resultNode;
    if(Match_VAR_DEC(tokens, resultNode)){
        Log("Done");
        Log(resultNode->name->value));
        Log("Over");
        return true;
    }
    return false;
}

bool Match_VAR_DEC(vector<Token> &tokens, Node* parent) {
    Node_VarDec* node_varDec = new Node_VarDec();
    if(Match_word(tokens, node_varDec->type) && Match_word(tokens, node_varDec->name) && Match_equal(tokens) && Match_VALUE(tokens, node_varDec->value)){
        parent = node_varDec;
        return true;
    }
    return false;
}

bool Match_VALUE(vector<Token> &tokens, Node* parent) {
    Node_Value* node_value = new Node_Value();
    if(Match_word(tokens, node_value->value) || Match_number(tokens, node_value->value)){
        parent = node_value;
        return true;
    }
    return false;
}

// -- Terminals
bool Match_word(vector<Token> &tokens, Node* parent) {
    JumpSpace(tokens);

    if(tokens[currentToken].tokenType != TokenType::WORD)
        return false;

    Log("Match: word");
    Node_Terminal_Word* node_terminal_word = new Node_Terminal_Word();
    node_terminal_word->value = tokens[currentToken].value;
    parent = node_terminal_word;
    currentToken++;
    return true;
}

bool Match_number(vector<Token> &tokens, Node* parent) {
    JumpSpace(tokens);

    if(tokens[currentToken].tokenType != TokenType::NUMBER)
        return false;

    Log("Match: number");
    Node_Terminal_Number* node_terminal_number = new Node_Terminal_Number();
    node_terminal_number->value = tokens[currentToken].value;
    parent = node_terminal_number;
    currentToken++;
    return true;
}

bool Match_equal(vector<Token> &tokens) {
    JumpSpace(tokens);

    if(tokens[currentToken].tokenType != TokenType::OPERATOR_EQUALS)
        return false;

    Log("Match: equal");
    currentToken++;
    return true;
}

// -- Utility
void JumpSpace(vector<Token> &tokens) {
    while(tokens[currentToken].tokenType == TokenType::SPACE)
    {
        currentToken++;
    }
}
