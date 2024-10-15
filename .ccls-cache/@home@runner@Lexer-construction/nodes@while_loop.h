#ifndef WHILELOOP_H
#define WHILELOOP_H

#include <memory>
#include "statement.h"
#include "expression.h"

class WhileLoop : public Statement {
public:
    WhileLoop(std::shared_ptr<Expression> condition, std::shared_ptr<Statement> body)
        : condition(condition), body(body) {}

    std::string ToString(int counter) const override {
        return "while " + condition->ToString(2) + " loop " + body->ToString(2) + " end";
    }

private:
    std::shared_ptr<Expression> condition;  
    std::shared_ptr<Statement> body;    
};

#endif // WHILELOOP_H
