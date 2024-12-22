#ifndef STATEMENT_H
#define STATEMENT_H

#include <string>
#include <memory>
#include "node.h"

class Statement : public Node {
public:
    // Assignment | RoutineCall 
    // | WhileLoop | ForLoop | /* ForeachLoop */ 
    // | IfStatement
    std::shared_ptr<Node> child;

    Statement(std::shared_ptr<Node> childNode) : child(childNode) {}

    std::string ToString(int count) const override {
        std::string ots(count * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "Statement:\n" + child->ToString(count + 1);
        return result;
    }
};

#endif // STATEMENT_H
