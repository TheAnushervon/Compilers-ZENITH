#ifndef STATEMENT_H
#define STATEMENT_H

#include <string>
#include <memory>
#include "node.h"

class Statement : public Node {
public:

Statement(std::shared_ptr<Node> childNode) : child(childNode) {}

    std::string ToString(int count) const override {
        std::string result = "Statement: " + child->ToString(2);
        return result;
    }

 private:
// Assignment | RoutineCall 
//| WhileLoop | ForLoop | /* ForeachLoop */ 
//| IfStatement
 std::shared_ptr<Node> child;
};

#endif // STATEMENT_H
