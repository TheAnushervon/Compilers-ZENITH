#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <memory>
#include <vector>
#include "node.h"

class Expression : public Node{
public:

    //Relation { ( and | or | xor ) Relation }
    std::vector<std::shared_ptr<Node>>relations;
    Expression(const std::vector<std::shared_ptr<Node>>& relationsList) : relations(relationsList) {}

    std::string ToString(int count) const override {
    std::string result = "Expression: ";
    for (const auto& relation : relations) {
        result += relation->ToString(count) + " ";
    }
    return result;
}

};

#endif // EXPRESSION_H
