//
// Created by ramon on 10/12/2017.
//

#include <fstream>
#include "Parser.h"
#include "Lexer.h"

Parser::Parser(vector<Token> &tokens) {
    this->tokens = tokens;
    this->current_token = &tokens[0];
}

int idIterator = 0;
Node::Node() {
    this->id = idIterator;
    idIterator++;
    this->node_child_left = nullptr;
    this->node_child_center = nullptr;
    this->node_child_right = nullptr;
}

Node::~Node() {
    if(node_child_left != nullptr)
        delete node_child_left;

    if(node_child_center != nullptr)
        delete node_child_center;

    if(node_child_right != nullptr)
        delete node_child_right;
}

void Node::PrintSubtree(int depth) {
    string tabs = "";

    for (int i = 0; i < depth; ++i) {
        tabs = tabs + "\t";
    }

    Log(tabs + string(this->value));

    if(node_child_left != nullptr)
        node_child_left->PrintSubtree(depth+1);

    if(node_child_center != nullptr)
        node_child_center->PrintSubtree(depth+1);

    if(node_child_right != nullptr)
        node_child_right->PrintSubtree(depth+1);
}

void Node::PrintDotSubtree(ostream &ostream1) {

    ostream1 << to_string(this->id) + " [label=\"" + this->value + "\"]" << endl;

    if(node_child_left != nullptr) {
        ostream1 << to_string(this->id) + " -- " + to_string(node_child_left->id) << endl;
        node_child_left->PrintDotSubtree(ostream1);
    }

    if(node_child_center != nullptr) {
        ostream1 << to_string(this->id) + " -- " + to_string(node_child_center->id) << endl;
        node_child_center->PrintDotSubtree(ostream1);
    }

    if(node_child_right != nullptr) {
        ostream1 << to_string(this->id) + " -- " + to_string(node_child_right->id) << endl;
        node_child_right->PrintDotSubtree(ostream1);
    }
}

void Parser::PrintTree(Node* node) {
    ofstream outputFile("graph.txt");
    outputFile << "graph graphname {" << endl;
    node->PrintDotSubtree(outputFile);
    outputFile << "}";
    outputFile.close();
}

Node *Parser::Match_Tokens() {
    // -- Change start symbol here
    this->root_node = Match_E();

    if(root_node != nullptr && current_token->tokenType == TokenType::EOF_DOLLAR)
        return root_node;

    if(root_node != nullptr && current_token->tokenType != TokenType::EOF_DOLLAR)
        Log("File parsed, but file end was not reached.");

    return nullptr;
}

// -- Non Terminals

Node* Parser::Match_T() {
    // -- Try each production until match is found
    Node* t1 = Match_T1();
    if(t1 != nullptr)
        return t1;

    Node* t2 = Match_T2();
    if(t2 != nullptr)
        return t2;

    Node* t3 = Match_T3();
    if(t3 != nullptr)
        return t3;

    // -- No match found
    return nullptr;
}

Node* Parser::Match_T1() {
    Token* start_token = current_token;

    Node* num = Match_Terminal_num();
    if(num == nullptr) {
        current_token = start_token;
        return nullptr;
    }

    Node* multiply = Match_Terminal_multiply();
    if(multiply == nullptr) {
        current_token = start_token;
        return nullptr;
    }

    Node* t = Match_T();
    if(t == nullptr) {
        current_token = start_token;
        return nullptr;
    }

    Node* new_node = new Node();
    new_node->value = "T1";
    new_node->node_child_left = num;
    new_node->node_child_center = multiply;
    new_node->node_child_right = t;
    return new_node;
}

Node* Parser::Match_T2() {
    Token* start_token = current_token;

    Node* num = Match_Terminal_num();
    if(num == nullptr) {
        current_token = start_token;
        return nullptr;
    }

    Node* new_node = new Node();
    new_node->value = "T2";
    new_node->node_child_left = num;
    return new_node;
}

Node* Parser::Match_T3() {
    Token* start_token = current_token;

    Node* lb = Match_Terminal_left_brace();
    if(lb == nullptr) {
        current_token = start_token;
        return nullptr;
    }

    Node* e = Match_E();
    if(e == nullptr) {
        current_token = start_token;
        return nullptr;
    }

    Node* rb = Match_Terminal_right_brace();
    if(rb == nullptr) {
        current_token = start_token;
        return nullptr;
    }

    Node* new_node = new Node();
    new_node->value = "T3";
    new_node->node_child_left = lb;
    new_node->node_child_center = e;
    new_node->node_child_right = rb;
    return new_node;
}


Node* Parser::Match_E() {
    // -- Try each production until match is found
    Node* e1 = Match_E1();
    if(e1 != nullptr)
        return e1;

    Node* e2 = Match_E2();
    if(e2 != nullptr)
        return e2;

    // -- No match found
    return nullptr;
}

Node* Parser::Match_E1() {
    Token* start_token = current_token;

    Node* t = Match_T();
    if(t == nullptr) {
        current_token = start_token;
        return nullptr;
    }

    Node* plus = Match_Terminal_plus();
    if(plus == nullptr) {
        current_token = start_token;
        return nullptr;
    }

    Node* e = Match_E();
    if(e == nullptr) {
        current_token = start_token;
        return nullptr;
    }

    Node* new_node = new Node();
    new_node->value = "E1";
    new_node->node_child_left = t;
    new_node->node_child_center = plus;
    new_node->node_child_right = e;
    return new_node;
}

Node* Parser::Match_E2() {
    Token* start_token = current_token;

    Node* t = Match_T();
    if(t == nullptr) {
        current_token = start_token;
        return nullptr;
    }

    Node* new_node = new Node();
    new_node->value = "E2";
    new_node->node_child_left = t;
    return new_node;
}

// -- Terminals

Node* Parser::Match_Terminal_num() {
    if(current_token->tokenType == TokenType::NUMBER) {
        Node* new_node = new Node();
        new_node->value = current_token->value;
        current_token++;
        return new_node;
    } else {
        return nullptr;
    }
}

Node* Parser::Match_Terminal_plus() {
    if(current_token->tokenType == TokenType::OPERATOR_PLUS) {
        Node* new_node = new Node();
        new_node->value = "+";
        current_token++;
        return new_node;
    } else {
        return nullptr;
    }
}

Node* Parser::Match_Terminal_multiply() {
    if(current_token->tokenType == TokenType::OPERATOR_MULTIPLY) {
        Node* new_node = new Node();
        new_node->value = "*";
        current_token++;
        return new_node;
    } else {
        return nullptr;
    }
}

Node* Parser::Match_Terminal_left_brace() {
    if(current_token->tokenType == TokenType::OPERATOR_LBRACE) {
        Node* new_node = new Node();
        new_node->value = "(";
        current_token++;
        return new_node;
    } else {
        return nullptr;
    }
}

Node* Parser::Match_Terminal_right_brace() {
    if(current_token->tokenType == TokenType::OPERATOR_RBRACE) {
        Node* new_node = new Node();
        new_node->value = ")";
        current_token++;
        return new_node;
    } else {
        return nullptr;
    }
}
