#ifndef IFSTATEMENT_H
#define IFSTATEMENT_H

#include "expression.h"
#include "statement.h"
#include <memory>

class IfStatement : public Statement {
  public:
    IfStatement(std::shared_ptr<Expression> condition,
                std::shared_ptr<Statement> trueBody,
                std::shared_ptr<Statement> falseBody = nullptr)
        : condition(condition), trueBody(trueBody), falseBody(falseBody) {}

    std::string ToString(int counter) const override {
        std::string result = "";
        for (int i = 0; i < counter; i++) {
            result += " ";
        }
        result += "IfStatement:\n";
        for (int i = 0; i < counter + 2; i++) {
            result += " ";
        }
        result += "Condition:\n";
        for (int i = 0; i < counter + 4; i++) {
            result += " ";
        }
        result += condition->ToString(counter) + "\n";

        result += trueBody->ToString(counter + 2);
        if (falseBody) {
            for (int i = 0; i < counter + 2; i++) {
                result += " ";
            }
            result += "Else:\n" + falseBody->ToString(counter + 4);
        }
        return result;
    }

  private:
    std::shared_ptr<Expression> condition;
    std::shared_ptr<Statement> trueBody;
    std::shared_ptr<Statement> falseBody;
};

#endif // IFSTATEMENT_H
