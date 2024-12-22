#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include <memory>
#include "statement.h"
#include "expression.h"
#include "identifier.h"

class Assignment : public Statement {
public:
    Assignment(std::shared_ptr<Identifier> variable, std::shared_ptr<Expression> expression)
        : variable(variable), expression(expression) {}

    std::string ToString() const override {
        return variable->ToString() + " := " + expression->ToString();
    }

private:
    std::shared_ptr<Identifier> variable;    
    std::shared_ptr<Expression> expression; 
};

#endif // ASSIGNMENT_H
