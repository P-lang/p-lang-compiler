#include <iostream>
#include <fstream>
#include <vector>
#include "Lexer.h"
#include "Parser.h"

using namespace std;

void Log(string message) {
    cout << message << endl;
}

int main() {
    std::ifstream fileStream("./plang.plg");
    std::string source((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());

    // Lexer
    Lexer lexer;
    lexer.Tokenize(&source);
    lexer.PrintTokens();

    // Parser
    Parser parser(lexer.tokens);
    Node* node_start = parser.Match_T1();

    if(node_start == nullptr)
        Log("Its null");

    node_start->PrintSubtree(0);

    return 0;
}