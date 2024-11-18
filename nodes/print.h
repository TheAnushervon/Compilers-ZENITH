#ifndef PRINT_H
#define PRINT_H

#include <memory>

#include "node.h"



class Print : public Node {
public:
    std::shared_ptr<Node> child;// Может быть Identifier или RoutineCall

    explicit Print(std::shared_ptr<Node> childNode)
        : child(std::move(childNode)) {}

    std::string ToString(int indent = 0) const override {
        std::string indentStr(indent, ' ');
        return indentStr + "Print:\n" + child->ToString(indent + 2);
    }
};





#endif //PRINT_H
