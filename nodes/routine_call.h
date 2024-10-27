#ifndef ROUTINECALL_H
#define ROUTINECALL_H

#include <vector>
#include <memory>
#include "node.h"

class RoutineCall : public Node {
public:

    std::shared_ptr<Node> identifier;              
    std::vector<std::shared_ptr<Node>> expressions;
    RoutineCall(std::shared_ptr<Node> id,                 std::vector<std::shared_ptr<Node>> exprs)
: identifier(id), expressions(exprs) {}


    std::string ToString(int counter) const override {
       std::string result = "RoutineCall: ";
        result += identifier->ToString(counter);
        for (const auto& item : expressions) {
            result += item->ToString(counter);
        }
        return result;
    }   
};

#endif // ROUTINECALL_H
