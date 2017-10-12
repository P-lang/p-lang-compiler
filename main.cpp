#include <iostream>
#include <fstream>
#include <vector>
#include "Lexer.h"

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


    return 0;
}