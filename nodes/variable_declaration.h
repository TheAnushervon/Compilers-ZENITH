#ifndef VARIABLEDECLARATION_H
#define VARIABLEDECLARATION_H

#include <memory>
#include "simple_declaration.h"
#include "identifier.h"
#include "type.h"
#include "expression.h"

class VariableDeclaration : public SimpleDeclaration {
public:
    VariableDeclaration(std::shared_ptr<Identifier> identifier, std::shared_ptr<Type> type, std::shared_ptr<Expression> expression = nullptr)
        : identifier(identifier), type(type), expression(expression) {}

    std::string ToString() const override {
        std::string result = "var " + identifier->ToString() + " : " + (type ? type->ToString() : "inferred");
        if (expression) {
            result += " = " + expression->ToString();
        }
        return result;
    }

private:
    std::shared_ptr<Identifier> identifier;
    std::shared_ptr<Type> type;
    std::shared_ptr<Expression> expression; 
};

#endif // VARIABLEDECLARATION_H
