#ifndef VARIABLEDECLARATION_H
#define VARIABLEDECLARATION_H

#include "expression.h"
#include "identifier.h"
#include "simple_declaration.h"
#include "type.h"
#include <memory>

class VariableDeclaration : public SimpleDeclaration {
  public:
    VariableDeclaration(std::shared_ptr<Identifier> identifier,
                        std::shared_ptr<Type> type,
                        std::shared_ptr<Expression> expression = nullptr)
        : identifier(identifier), type(type), expression(expression) {}

    std::string ToString(int counter) const override {
        std::string result = "";
        for (int i = 0; i < counter; i++) {
            result += " ";
        }
        result += "Variable: " + identifier->ToString(2) + "\n";
        for (int i = 0; i < counter + 2; i++) {
            result += " ";
        }
        result += "Type: " + (type ? type->ToString(2) : "inferred") + "\n";
        for (int i = 0; i < counter + 2; i++) {
            result += " ";
        }
        if (expression) {
            result += "Value: " + expression->ToString(2);
        }
        return result;
    }

  private:
    std::shared_ptr<Identifier> identifier;
    std::shared_ptr<Type> type;
    std::shared_ptr<Expression> expression;
};

#endif // VARIABLEDECLARATION_H
