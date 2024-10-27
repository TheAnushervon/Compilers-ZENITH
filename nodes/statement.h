#ifndef STATEMENT_H
#define STATEMENT_H

#include <string>
#include <memory>
#include "node.h"

class Statement : public Node {
public:
    // Assignment | RoutineCall 
    //| WhileLoop | ForLoop | /* ForeachLoop */ 
    //| IfStatement
     std::shared_ptr<Node> child;

    Statement(std::shared_ptr<Node> childNode) : child(childNode) {}

    std::string ToString(int count) const override {
        std::string result = "Statement: " + child->ToString(2);
        return result;
    }

};

#endif // STATEMENT_H
