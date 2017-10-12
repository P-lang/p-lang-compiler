//
// Created by ramon on 10/12/2017.
//

#include "Parser.h"
#include "Lexer.h"

Parser::Parser(vector<Token> &tokens) {
    this->tokens = tokens;
    this->current_token = &tokens[0];
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

// -- Non Terminals

Node* Parser::Match_T() {
    // -- Try each production until match is found
    Node* t1 = Match_T1();
    if(t1 != nullptr)
        return t1;

    Node* t2 = Match_T2();
    if(t2 != nullptr)
        return t2;

    //Node* t3 = Match_T3();
    //if(t3 != nullptr)
    //    return t3;

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
    return nullptr;
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
