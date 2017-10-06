#include <iostream>
#include <fstream>
#include <vector>
#include "CommonTypes.h"
#include "Parser.h"

using namespace std;

void Log(string message) {
    cout << message << endl;
}

// -- Tokenizer

const char *TokenTypeString[] = {"NUMBER\t\t", "SPACE\t\t", "TAB\t\t", "WORD\t\t", "END_OF_LINE\t", "OPERATOR_EQUALS\t",
                                 "OPERATOR_PLUS\t", "OPERATOR_MINUS\t", "OPERATOR_MULTIPLY", "OPERATOR_DIVIDE\t",
                                 "OPERATOR_MORE\t", "OPERATOR_LESS\t", "OPERATOR_MORE_OR_EQUAL",
                                 "OPERATOR_LESS_OR_EQUAL", "OPERATOR_PLUS_EQUALS", "OPERATOR_MINUS_EQUALS", "OPERATOR_MULTIPLY_EQUALS",
                                 "OPERATOR_DIVIDE_EQUALS", "OPERATOR_NOT_EQUALS", "OPERATOR_NOT", "COMMENT\t\t"};

vector<Token> tokens;

void MakeToken(const string *source, TokenType tokenType, int startIndex, int length) {
    Token token;
    token.tokenType = tokenType;
    token.value = source->substr(startIndex, length);
    tokens.push_back(token);

    //Log("Making token: " + string(to_string(startIndex)) + " -> " + to_string(length) + " Value: |" + token.value + "|");
}

void Tokenize(const string *source) {
    Log("Tokenizing source.");

    enum {
        NORMAL,
        NUMBER,
        WORD,
        COMMENT
    } state = NORMAL;

    int sourceLength = source->length();
    int currentIndex = 0;
    int tokenStartIndex = 0;
    int tokenLength = 0;

    char character = source->at(0);
    char characterNext = 0xFF;

    while (true) {
        // -- End of file
        if (currentIndex == sourceLength) {
            break;
        }
        character = source->at(currentIndex);

        if (currentIndex < sourceLength - 1)
            characterNext = source->at(currentIndex + 1);

        enum {
            ALPHA,
            DIGIT,
            SPACE,
            OPERATOR,
            COMMENT_INDICATOR
        } characterCategory, characterNextCategory;

        enum {
            SPACE_SPACE,
            SPACE_TAB,
            SPACE_EOL
        } spaceType;

        // -- Set character category
        if ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z')) {
            characterCategory = ALPHA;
        } else if (character > '0' && character < '9') {
            characterCategory = DIGIT;
        } else if (character == ' ' || character == '\n' || character == '\r' || character == '\t') {
            characterCategory = SPACE;
            if (character == ' ') {
                spaceType = SPACE_SPACE;
            } else if (character == '\n' || character == '\r') {
                spaceType = SPACE_EOL;
            } else if (character == '\t') {
                spaceType = SPACE_TAB;
            }
        } else if (character == '+' || character == '-' || character == '*' || character == '/'
                   || character == '<' || character == '>' || character == '=' || character == '!') {
            characterCategory = OPERATOR;
        } else if (character == '#') {
            characterCategory = COMMENT_INDICATOR;
        }

        if ((characterNext >= 'a' && characterNext <= 'z') || (characterNext >= 'A' && characterNext <= 'Z')) {
            characterNextCategory = ALPHA;
        } else if (characterNext > '0' && characterNext < '9') {
            characterNextCategory = DIGIT;
        } else if (characterNext == ' ' || characterNext == '\n' || characterNext == '\r' || characterNext == '\t') {
            characterNextCategory = SPACE;
        } else if (characterNext == '+' || characterNext == '-' || characterNext == '*' || characterNext == '/'
                   || characterNext == '<' || characterNext == '>' || characterNext == '=' || characterNext == '!') {
            characterNextCategory = OPERATOR;
        } else if (characterNext == '#') {
            characterNextCategory = COMMENT_INDICATOR;
        }

        tokenLength = currentIndex - tokenStartIndex;



        switch (state) {
            case NORMAL:
                // -- No token being made, check for token initiators
                tokenStartIndex = currentIndex;
                if (characterCategory == ALPHA) {
                    state = WORD;
                } else if (characterCategory == DIGIT) {
                    state = NUMBER;
                } else if (characterCategory == SPACE) {
                    switch (spaceType) {
                        case SPACE_SPACE:
                            if (tokens.at(tokens.size() - 1).tokenType != TokenType::SPACE)
                                MakeToken(source, TokenType::SPACE, currentIndex, 0);
                            break;
                        case SPACE_TAB:
                            MakeToken(source, TokenType::TAB, currentIndex, 0);
                            break;
                        case SPACE_EOL:
                            MakeToken(source, TokenType::END_OF_LINE, currentIndex, 0);
                            break;
                    }
                } else if (characterCategory == COMMENT_INDICATOR) {
                    state = COMMENT;
                }
                break;

            case WORD:
                if (characterCategory == SPACE) {
                    state = NORMAL;
                    MakeToken(source, TokenType::WORD, tokenStartIndex, tokenLength);
                    switch (spaceType) {
                        case SPACE_SPACE:
                            MakeToken(source, TokenType::SPACE, currentIndex, 0);
                            break;
                        case SPACE_TAB:
                            MakeToken(source, TokenType::TAB, currentIndex, 0);
                            break;
                        case SPACE_EOL:
                            MakeToken(source, TokenType::END_OF_LINE, currentIndex, 0);
                            break;
                    }
                } else if (characterCategory == COMMENT_INDICATOR) {
                    state = COMMENT;
                    MakeToken(source, TokenType::WORD, tokenStartIndex, tokenLength);
                } else if (characterCategory == OPERATOR) {
                    state = NORMAL;
                    MakeToken(source, TokenType::WORD, tokenStartIndex, tokenLength);
                }
                break;

            case NUMBER:
                if (characterCategory == SPACE) {
                    state = NORMAL;
                    MakeToken(source, TokenType::NUMBER, tokenStartIndex, tokenLength);
                    switch (spaceType) {
                        case SPACE_SPACE:
                            MakeToken(source, TokenType::SPACE, currentIndex, 0);
                            break;
                        case SPACE_TAB:
                            MakeToken(source, TokenType::TAB, currentIndex, 0);
                            break;
                        case SPACE_EOL:
                            MakeToken(source, TokenType::END_OF_LINE, currentIndex, 0);
                            break;
                    }
                } else if (characterCategory == COMMENT_INDICATOR) {
                    state = COMMENT;
                    MakeToken(source, TokenType::NUMBER, tokenStartIndex, tokenLength);
                } else if (characterCategory == OPERATOR) {
                    state = NORMAL;
                    MakeToken(source, TokenType::NUMBER, tokenStartIndex, tokenLength);
                }
                break;

            case COMMENT:
                if (spaceType == SPACE_EOL) {
                    state = NORMAL;
                    MakeToken(source, TokenType::COMMENT, tokenStartIndex, tokenLength);
                    MakeToken(source, TokenType::END_OF_LINE, currentIndex, 0);
                }
        }

        // -- Deal with operators
        if (characterCategory == OPERATOR && state != COMMENT) {
            state = NORMAL;
            if(character == '+')
            {
                if(characterNext == '=')
                {
                    MakeToken(source, TokenType::OPERATOR_PLUS_EQUALS, currentIndex, 0);
                    currentIndex++;
                } else {
                    MakeToken(source, TokenType::OPERATOR_PLUS, currentIndex, 0);
                }
            }
            else if(character == '-')
            {
                if(characterNext == '=')
                {
                    MakeToken(source, TokenType::OPERATOR_MINUS_EQUALS, currentIndex, 0);
                    currentIndex++;
                } else {
                    MakeToken(source, TokenType::OPERATOR_MINUS, currentIndex, 0);
                }
            }
            else if(character == '*')
            {
                if(characterNext == '=')
                {
                    MakeToken(source, TokenType::OPERATOR_MULTIPLY_EQUALS, currentIndex, 0);
                    currentIndex++;
                } else {
                    MakeToken(source, TokenType::OPERATOR_MULTIPLY, currentIndex, 0);
                }
            }
            else if(character == '/')
            {
                if(characterNext == '=')
                {
                    MakeToken(source, TokenType::OPERATOR_DIVIDE_EQUALS, currentIndex, 0);
                    currentIndex++;
                } else {
                    MakeToken(source, TokenType::OPERATOR_DIVIDE, currentIndex, 0);
                }
            }
            else if(character == '<')
            {
                if(characterNext == '=')
                {
                    MakeToken(source, TokenType::OPERATOR_LESS_OR_EQUAL, currentIndex, 0);
                    currentIndex++;
                } else {
                    MakeToken(source, TokenType::OPERATOR_LESS, currentIndex, 0);
                }
            }
            else if(character == '>')
            {
                if(characterNext == '=')
                {
                    MakeToken(source, TokenType::OPERATOR_MORE_OR_EQUAL, currentIndex, 0);
                    currentIndex++;
                } else {
                    MakeToken(source, TokenType::OPERATOR_MORE, currentIndex, 0);
                }
            }
            else if(character == '!')
            {
                if(characterNext == '=')
                {
                    MakeToken(source, TokenType::OPERATOR_NOT_EQUALS, currentIndex, 0);
                    currentIndex++;
                } else {
                    MakeToken(source, TokenType::OPERATOR_NOT, currentIndex, 0);
                }
            }
            else if(character == '=')
            {
                MakeToken(source, TokenType::OPERATOR_EQUALS, currentIndex, 0);
            }
        }
        currentIndex++;
    }
}

int main() {
    std::ifstream fileStream("./plang.plg");
    std::string source((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());

    // Tokenize Source
    Tokenize(&source);

    for (int i = 0; i < tokens.size(); i++) {
        Log(TokenTypeString[tokens[i].tokenType] + string("\t|") + tokens[i].value + "|");
    }

    // Parse to AST
    Match_START(tokens);

    return 0;
}