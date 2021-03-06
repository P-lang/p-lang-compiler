//
// Created by ramon on 10/12/2017.
//

#ifndef PLANGCOMPILER_PARSER_H
#define PLANGCOMPILER_PARSER_H

#include <vector>
#include "iostream"
#include "CommonTypes.h"

using namespace std;

struct Node {
    int id;
    Node* node_child_left;
    Node* node_child_center;
    Node* node_child_right;
    string value;

    Node();
    ~Node();
    void PrintSubtree(int depth);
    void PrintDotSubtree(ostream &ostream1);
};

class Parser {

private:
    vector<Token> tokens;
    Token* current_token;

public:

    Parser(vector<Token> &tokens);

    void PrintTree(Node* node);
    Node* root_node = nullptr;

    Node* Match_Tokens();

    // -- Non Terminals
    Node* Match_T();
    Node* Match_T1();
    Node* Match_T2();
    Node* Match_T3();

    Node* Match_E();
    Node* Match_E1();
    Node* Match_E2();

    // -- Terminals
    Node* Match_Terminal_num();
    Node* Match_Terminal_plus();
    Node* Match_Terminal_multiply();
    Node* Match_Terminal_left_brace();
    Node* Match_Terminal_right_brace();

};


#endif //PLANGCOMPILER_PARSER_H
