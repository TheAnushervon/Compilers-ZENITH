#ifndef RANGE_H
#define RANGE_H

#include <memory>
#include <string>
#include "expression.h"
#include "statement.h"

class Range : public Statement {
public:
    
Range(std::shared_ptr<Expression> start, std::shared_ptr<Expression> end)
        : start(start), end(end) {}

    std::string ToString(int counter) const override {
        return "Range(" + (start ? start->ToString(2) : "None") + " .. " +
               (end ? end->ToString(2) : "None") + ")";
    }

    std::shared_ptr<Expression> GetStart() const { return start; }
    std::shared_ptr<Expression> GetEnd() const { return end; }

private:
    std::shared_ptr<Expression> start; 
    std::shared_ptr<Expression> end;
};

#endif // RANGE_H
