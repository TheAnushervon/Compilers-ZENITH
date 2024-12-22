#ifndef WHILELOOP_H
#define WHILELOOP_H

#include <memory>
#include "statement.h"
#include "expression.h"

class WhileLoop : public Statement {
public:
    WhileLoop(std::shared_ptr<Expression> condition, std::shared_ptr<Statement> body)
        : condition(condition), body(body) {}

    std::string ToString() const override {
        return "while " + condition->ToString() + " loop " + body->ToString() + " end";
    }

private:
    std::shared_ptr<Expression> condition;  
    std::shared_ptr<Statement> body;    
};

#endif // WHILELOOP_H
