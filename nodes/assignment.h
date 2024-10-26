#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include "expression.h"
#include "identifier.h"
#include "simple_declaration.h"
#include "statement.h"
#include <memory>

class Assignment : public SimpleDeclaration {
  public:
    Assignment(std::shared_ptr<Identifier> variable,
               std::shared_ptr<Expression> expression)
        : variable(variable), expression(expression) {}

    std::string ToString(int counter) const override {
        std::string result;

        for (int i = 0; i < counter; i++) {
            result += " ";
        }

        result += "Assignment:\n";

        for (int i = 0; i < counter + 2; i++) {
            result += " ";
        }
        result += "Variable: " + variable->ToString(counter + 2) + "\n";

        for (int i = 0; i < counter + 2; i++) {
            result += " ";
        }
        result += "Expression: " + expression->ToString(counter + 2) + "\n";

        return result;
    }

  private:
    std::shared_ptr<Identifier> variable;
    std::shared_ptr<Expression> expression;
};

#endif // ASSIGNMENT_H
