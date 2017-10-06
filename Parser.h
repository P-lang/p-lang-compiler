//
// Created by ramon on 10/6/2017.
//

#ifndef PLANGCOMPILER_PARSER_H
#define PLANGCOMPILER_PARSER_H

#include "CommonTypes.h"

using namespace std;

void Log(string message);

/*
struct Node;
struct Node_VarDec;
struct Node_Value;
struct Node_Terminal;
struct Node_Terminal_Word;
struct Node_Terminal_Number;
*/

struct Node {
    virtual void Print() = 0;
};

struct Node_Terminal : Node {
    virtual void Print() = 0;
    string value;
};

struct Node_Terminal_Word : Node_Terminal {
    //string value;
    void Print(){Log(value);}
};

struct Node_Terminal_Number : Node_Terminal {
    //string value;
    void Print(){Log(value);}
};

struct Node_Value : Node {
    Node_Terminal* value;
    void Print(){value->Print();}
};

struct Node_VarDec : Node {
    Node_Terminal_Word* type;
    Node_Terminal_Word* name;
    Node_Value* value;
    void Print(){
        //type->Print(); name->Print(); value->Print();
        Log("In");
    }
};

bool Match_START(vector<Token> &tokens);
bool Match_VAR_DEC(vector<Token> &tokens, Node* parent);
bool Match_VALUE(vector<Token> &tokens, Node* parent);

bool Match_word(vector<Token> &tokens, Node* parent);
bool Match_number(vector<Token> &tokens, Node* parent);

bool Match_equal(vector<Token> &tokens);

void JumpSpace(vector<Token> &tokens);

#endif //PLANGCOMPILER_PARSER_H
