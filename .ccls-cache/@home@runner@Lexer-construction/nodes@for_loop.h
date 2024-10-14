#ifndef FORLOOP_H
#define FORLOOP_H

#include <memory>
#include "statement.h"
#include "expression.h"
#include "statement.h"
#include "identifier.h"

class ForLoop : public Statement {
public:
    ForLoop(std::shared_ptr<Identifier> variable, std::shared_ptr<Expression> start, 
            std::shared_ptr<Expression> end, std::shared_ptr<Statement> body, bool reverse = false)
        : variable(variable), start(start), end(end), body(body), reverse(reverse) {}

    std::string ToString() const override {
        std::string direction = reverse ? "reverse " : "";
        return "for " + variable->ToString() + " in " + direction + start->ToString() + " .. " + end->ToString() + " loop " + body->ToString() + " end";
    }

private:
    std::shared_ptr<Identifier> variable;  
    std::shared_ptr<Expression> start;    
    std::shared_ptr<Expression> end;       
    std::shared_ptr<Statement> body;       
    bool reverse;                         
};

#endif // FORLOOP_H
