#include <iostream>
#include <vector>
#include <string>
#include "./tokens/structs/token.h"

class TokenAnalyzer {
public:
    TokenAnalyzer(const std::vector<Token>& tokens) : tokens(tokens), currentIndex(0) {}

    void Analyze() {
        while (currentIndex < tokens.size()) {
            const Token& token = tokens[currentIndex];
            switch (token.type) {
                case TokenType::tk_routine:
                    ParseRoutine();
                    break;
                case TokenType::tk_type:
                    ParseTypeDeclaration();
                    break;
                case TokenType::tk_var:
                    ParseVariableDeclaration();
                    break;
                case TokenType::tk_if:
                    ParseIfStatement();
                    break;
                case TokenType::tk_for:
                    ParseForLoop();
                    break;
                case TokenType::tk_while:
                    ParseWhileLoop();
                    break;
                default:
                    std::cout << "Unknown or unhandled token: " << token.value << std::endl;
                    AdvanceToken();
                    break;
            }
        }
    }

private:
    const std::vector<Token>& tokens;
    size_t currentIndex;

    const Token& GetCurrentToken() const {
        return tokens[currentIndex];
    }

    const Token& AdvanceToken() {
        if (currentIndex < tokens.size() - 1) {
            currentIndex++;
        }
        return tokens[currentIndex];
    }

    bool ExpectToken(TokenType expectedType) {
        if (GetCurrentToken().type == expectedType) {
            AdvanceToken();
            return true;
        } else {
            std::cerr << "Syntax error: Expected token of type " << TokenTypeToString(expectedType)
                      << ", but got " << TokenTypeToString(GetCurrentToken().type) << std::endl;
            return false;
        }
    }

    void ParseRoutine() {
        std::cout << "Parsing routine..." << std::endl;
        AdvanceToken(); // Переходим к следующему токену, который, предположительно, является именем подпрограммы
        if (ExpectToken(TokenType::tk_identifier)) {
            std::cout << "Routine name: " << GetCurrentToken().value << std::endl;
        }

        if (ExpectToken(TokenType::tk_open_parenthesis)) {
            ParseParameters();
            ExpectToken(TokenType::tk_close_parenthesis);
        }

        if (ExpectToken(TokenType::tk_is)) {
            std::cout << "Routine body:" << std::endl;
            ParseBody();
        }
    }

    void ParseTypeDeclaration() {
        std::cout << "Parsing type declaration..." << std::endl;
        AdvanceToken(); // Переход к имени типа
        if (ExpectToken(TokenType::tk_identifier)) {
            std::cout << "Type name: " << GetCurrentToken().value << std::endl;
        }

        if (ExpectToken(TokenType::tk_is)) {
            if (ExpectToken(TokenType::tk_integer) || ExpectToken(TokenType::tk_real) ||
                ExpectToken(TokenType::tk_boolean) || ExpectToken(TokenType::tk_record)) {
                std::cout << "Type defined as: " << TokenTypeToString(GetCurrentToken().type) << std::endl;
            }
        }
    }

    void ParseVariableDeclaration() {
        std::cout << "Parsing variable declaration..." << std::endl;
        AdvanceToken(); // Переход к имени переменной
        if (ExpectToken(TokenType::tk_identifier)) {
            std::cout << "Variable name: " << GetCurrentToken().value << std::endl;
        }

        if (ExpectToken(TokenType::tk_colon)) {
            if (ExpectToken(TokenType::tk_integer) || ExpectToken(TokenType::tk_real) ||
                ExpectToken(TokenType::tk_boolean) || ExpectToken(TokenType::tk_identifier)) {
                std::cout << "Variable type: " << TokenTypeToString(GetCurrentToken().type) << std::endl;
            }
        }

        if (ExpectToken(TokenType::tk_assign)) {
            ParseExpression();
        }
    }

    void ParseIfStatement() {
        std::cout << "Parsing if statement..." << std::endl;
        AdvanceToken(); // Переход к условию
        ParseExpression();

        if (ExpectToken(TokenType::tk_then)) {
            ParseBody();
        }

        if (GetCurrentToken().type == TokenType::tk_else) {
            std::cout << "Else block:" << std::endl;
            AdvanceToken();
            ParseBody();
        }

        ExpectToken(TokenType::tk_end);
    }

    void ParseForLoop() {
        std::cout << "Parsing for loop..." << std::endl;
        AdvanceToken(); // Переход к переменной
        if (ExpectToken(TokenType::tk_identifier)) {
            std::cout << "Loop variable: " << GetCurrentToken().value << std::endl;
        }

        if (ExpectToken(TokenType::tk_in)) {
            ParseExpression();
            if (ExpectToken(TokenType::tk_range)) {
                ParseExpression();
            }
        }

        if (ExpectToken(TokenType::tk_loop)) {
            ParseBody();
        }

        ExpectToken(TokenType::tk_end);
    }

    void ParseWhileLoop() {
        std::cout << "Parsing while loop..." << std::endl;
        AdvanceToken(); // Переход к условию
        ParseExpression();

        if (ExpectToken(TokenType::tk_loop)) {
            ParseBody();
        }

        ExpectToken(TokenType::tk_end);
    }

    void ParseParameters() {
        std::cout << "Parsing parameters..." << std::endl;
        while (GetCurrentToken().type != TokenType::tk_close_parenthesis) {
            if (ExpectToken(TokenType::tk_identifier)) {
                std::cout << "Parameter name: " << GetCurrentToken().value << std::endl;
            }

            if (ExpectToken(TokenType::tk_colon)) {
                if (ExpectToken(TokenType::tk_integer) || ExpectToken(TokenType::tk_real) ||
                    ExpectToken(TokenType::tk_boolean)) {
                    std::cout << "Parameter type: " << TokenTypeToString(GetCurrentToken().type) << std::endl;
                }
            }

            if (GetCurrentToken().type == TokenType::tk_comma) {
                AdvanceToken();
            } else {
                break;
            }
        }
    }

    void ParseExpression() {
        std::cout << "Parsing expression..." << std::endl;
        AdvanceToken();
    }

    void ParseBody() {
        std::cout << "Parsing body..." << std::endl;
        while (GetCurrentToken().type != TokenType::tk_end) {
            switch (GetCurrentToken().type) {
                case TokenType::tk_var:
                    ParseVariableDeclaration();
                    break;
                case TokenType::tk_if:
                    ParseIfStatement();
                    break;
                case TokenType::tk_for:
                    ParseForLoop();
                    break;
                case TokenType::tk_while:
                    ParseWhileLoop();
                    break;
                default:
                    ParseExpression();
                    break;
            }
        }
        AdvanceToken();
    }
};

std::string toString(TokenType token) {
    switch (token) {
    case TokenType::tk_routine:
        return "tk_routine";
    case TokenType::tk_type:
        return "tk_type";
    case TokenType::tk_is:
        return "tk_is";
    case TokenType::tk_var:
        return "tk_var";
    case TokenType::tk_end:
        return "tk_end";
    case TokenType::tk_for:
        return "tk_for";
    case TokenType::tk_loop:
        return "tk_loop";
    case TokenType::tk_in:
        return "tk_in";
    case TokenType::tk_while:
        return "tk_while";
    case TokenType::tk_if:
        return "tk_if";
    case TokenType::tk_then:
        return "tk_then";
    case TokenType::tk_else:
        return "tk_else";
    case TokenType::tk_integer:
        return "tk_integer";
    case TokenType::tk_boolean:
        return "tk_boolean";
    case TokenType::tk_real:
        return "tk_real";
    case TokenType::tk_record:
        return "tk_record";
    case TokenType::tk_array:
        return "tk_array";
    case TokenType::tk_true:
        return "tk_true";
    case TokenType::tk_false:
        return "tk_false";
    case TokenType::tk_reverse:
        return "tk_reverse";

    case TokenType::tk_add:
        return "tk_add";
    case TokenType::tk_subtract:
        return "tk_subtract";
    case TokenType::tk_multiply:
        return "tk_multiply";
    case TokenType::tk_divide:
        return "tk_divide";
    case TokenType::tk_and:
        return "tk_and";
    case TokenType::tk_or:
        return "tk_or";
    case TokenType::tk_xor:
        return "tk_xor";
    case TokenType::tk_mod:
        return "tk_mod";
    case TokenType::tk_equal:
        return "tk_equal";
    case TokenType::tk_not_equal:
        return "tk_not_equal";
    case TokenType::tk_less_than:
        return "tk_less_than";
    case TokenType::tk_greater_than:
        return "tk_greater_than";
    case TokenType::tk_less_than_equal:
        return "tk_less_than_equal";
    case TokenType::tk_greater_than_equal:
        return "tk_greater_than_equal";
    case TokenType::tk_assign:
        return "tk_assign";

    case TokenType::tk_open_parenthesis:
        return "tk_open_parenthesis";
    case TokenType::tk_close_parenthesis:
        return "tk_close_parenthesis";
    case TokenType::tk_open_bracket:
        return "tk_open_bracket";
    case TokenType::tk_close_bracket:
        return "tk_close_bracket";
    case TokenType::tk_colon:
        return "tk_colon";
    case TokenType::tk_comma:
        return "tk_comma";
    case TokenType::tk_dot:
        return "tk_dot";
    case TokenType::tk_range:
        return "tk_range";
    case TokenType::tk_newline:
        return "tk_newline";
    case TokenType::tk_identifier:
        return "tk_identifier";
    default:
        return "Unknown token";
    }
};