#ifndef FORLOOP_H
#define FORLOOP_H

#include "expression.h"
#include "identifier.h"
#include "statement.h"
#include <memory>
// check
class ForLoop : public Statement {
  public:
    ForLoop(std::shared_ptr<Identifier> variable,
            std::shared_ptr<Expression> start, std::shared_ptr<Expression> end,
            std::shared_ptr<Statement> body, bool reverse = false)
        : variable(variable), start(start), end(end), body(body),
          reverse(reverse) {}

    std::string ToString(int counter) const override {
        std::string result = "";
        for (int i = 0; i < counter; i++) {
            result += " ";
        }
        std::string direction = reverse ? "reverse " : "";
        result += "ForLoop:\n";
        for (int i = 0; i < counter + 2; i++) {
            result += " ";
        }
        result += "Variable: " + variable->ToString(counter) + "\n" + direction;
        for (int i = 0; i < counter + 4; i++) {
            result += " ";
        }
        result += "From:" + start->ToString(counter) + "\n";
        for (int i = 0; i < counter + 4; i++) {
            result += " ";
        }
        result +=
            "To:" + end->ToString(counter) + "\n" + body->ToString(counter + 2);

        return result;
    }

  private:
    std::shared_ptr<Identifier> variable;
    std::shared_ptr<Expression> start;
    std::shared_ptr<Expression> end;
    std::shared_ptr<Statement> body;
    bool reverse;
};

#endif // FORLOOP_H
