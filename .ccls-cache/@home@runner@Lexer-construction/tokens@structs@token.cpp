#include "../enums/token_type.cpp"

struct Token {
   TokenType type;
   std::string value;
 Token(TokenType t, const std::string& v) : type(t), value(v) {}
};