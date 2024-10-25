#ifndef WHILELOOP_H
#define WHILELOOP_H

#include "expression.h"
#include "statement.h"
#include <memory>

class WhileLoop : public Statement {
  public:
    WhileLoop(std::shared_ptr<Expression> condition,
              std::shared_ptr<Statement> body)
        : condition(condition), body(body) {}

    std::string ToString(int counter) const override {
        std::string result = "";
        for (int i = 0; i < counter; i++) {
            result += " ";
        }
        result += "WhileLoop:\n";
        for (int i = 0; i < counter + 2; i++) {
            result += " ";
        }
        result += "Condition:\n";

        for (int i = 0; i < counter + 4; i++) {
            result += " ";
        }

        result +=
            condition->ToString(counter) + "\n" + body->ToString(counter + 2);
        return result;
    }

  private:
    std::shared_ptr<Expression> condition;
    std::shared_ptr<Statement> body;
};

#endif // WHILELOOP_H
