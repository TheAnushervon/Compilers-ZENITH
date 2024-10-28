#ifndef RANGE_H
#define RANGE_H

#include <memory>
#include <string>
#include "node.h"

class Range : public Node {
public:
    // Expression .. Expression 
    std::shared_ptr<Node> start; 
    std::shared_ptr<Node> end;

    Range(std::shared_ptr<Node> startExpr, std::shared_ptr<Node> endExpr)
        : start(startExpr), end(endExpr) {}

    std::string ToString(int counter) const override {
        std::string ots(counter * 2, ' '); // Отступы с учетом уровня вложенности
        return ots + "Range:\n" + 
               ots + "  Start: " + (start ? start->ToString(counter + 1) : "None") + "\n" +
               ots + "  End: " + (end ? end->ToString(counter + 1) : "None");
    }
};

#endif // RANGE_H
