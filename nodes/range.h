#ifndef RANGE_H
#define RANGE_H

#include <memory>
#include <string>
#include "node.h"

class Range : public Node {
public:
    Range(std::shared_ptr<Node> startExpr, std::shared_ptr<Node> endExpr)
    : start(startExpr), end(endExpr) {}


    std::string ToString(int counter) const override {
        return "Range(" + (start ? start->ToString(2) : "None") + " .. " +
               (end ? end->ToString(2) : "None") + ")";
    }

   

private:
//Expression .. Expression 
    std::shared_ptr<Node> start; 
    std::shared_ptr<Node> end;
};

#endif // RANGE_H
