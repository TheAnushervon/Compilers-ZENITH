#ifndef IFSTATEMENT_H
#define IFSTATEMENT_H

#include <memory>
#include "statement.h"
#include "expression.h"

class IfStatement : public Statement {
public:
IfStatement(std::shared_ptr<Expression> condition, 
        std::shared_ptr<Statement> trueBody, 
        std::shared_ptr<Statement> falseBody = nullptr)
: condition(condition), trueBody(trueBody), falseBody(falseBody) {}

    std::string ToString() const override {
        std::string result = "if " + condition->ToString() + " then " + trueBody->ToString();
        if (falseBody) {
            result += " else " + falseBody->ToString();
        }
        result += " end";
        return result;
    }

private:
std::shared_ptr<Expression> condition;
std::shared_ptr<Statement> trueBody;
std::shared_ptr<Statement> falseBody;   
};

#endif // IFSTATEMENT_H
