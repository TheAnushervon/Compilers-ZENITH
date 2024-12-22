#ifndef LITERAL_EXPRESSION_H
#define LITERAL_EXPRESSION_H

#include "expression.h"

class LiteralExpression : public Expression {
public:
    LiteralExpression(const std::string& value) : value(value) {}

    std::string ToString() const override {
        return value;
    }

private:
    std::string value;
};

#endif // LITERAL_EXPRESSION_H
