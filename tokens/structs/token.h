#ifndef TOKEN_H
#define TOKEN_H

#include "../enums/token_type.h"
#include <string>

struct Token {
    TokenType type;
    std::string value;

    Token(TokenType t, const std::string &v) : type(t), value(v) {}
};
#endif // TOKEN_H
