//strucs/token.h
#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include "../enums/token_type.h"

struct Token {
    TokenType type;
    std::string value;

    Token(TokenType t, const std::string& v) : type(t), value(v) {}
};
#endif //TOKEN_H
