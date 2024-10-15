#include <iostream>
#include <vector>
#include <string>
#include <cctype>

enum class TokenType {
    Keyword,
    Identifier,
    Operator,
    Literal,
    Punctuation,
    Unknown
};

struct Token {
    TokenType type;
    std::string value;
};

class Lexer {
public:
    Lexer(const std::string& input) : input(input), pos(0) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        while (pos < input.size()) {
            char current = input[pos];
            if (std::isspace(current)) {
                ++pos;
                continue;
            }
            if (std::isalpha(current)) {
                tokens.push_back(parseIdentifierOrKeyword());
            } else if (std::isdigit(current)) {
                tokens.push_back(parseLiteral());
            } else if (current == '=' || current == '+') {  // Handling basic operators
                tokens.push_back({TokenType::Operator, std::string(1, current)});
                ++pos;
            } else if (current == ';' || current == ',') {  // Handling punctuation
                tokens.push_back({TokenType::Punctuation, std::string(1, current)});
                ++pos;
            } else {
                tokens.push_back({TokenType::Unknown, std::string(1, current)});
                ++pos;
            }
        }
        return tokens;
    }

private:
    std::string input;
    size_t pos;

    Token parseIdentifierOrKeyword() {
        size_t start = pos;
        while (pos < input.size() && std::isalnum(input[pos])) {
            ++pos;
        }
        std::string word = input.substr(start, pos - start);
        if (word == "int") {
            return {TokenType::Keyword, word};
        }
        return {TokenType::Identifier, word};
    }

    Token parseLiteral() {
        size_t start = pos;
        while (pos < input.size() && std::isdigit(input[pos])) {
            ++pos;
        }
        return {TokenType::Literal, input.substr(start, pos - start)};
    }
};

int main3() {
    std::string code = "int x = 10;";
    Lexer lexer(code);
    std::vector<Token> tokens = lexer.tokenize();

    for (const Token& token : tokens) {
        std::cout << "Token Type: " << static_cast<int>(token.type)
                  << ", Value: " << token.value << std::endl;
    }

    return 0;
}
