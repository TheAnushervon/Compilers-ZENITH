#ifndef SIMPLEDECLARATION_H
#define SIMPLEDECLARATION_H

#include <string>
#include <memory>
#include "node.h"

class SimpleDeclaration : public Node {
public:
    std::shared_ptr<Node> child;

    SimpleDeclaration(std::shared_ptr<Node> childNode) : child(childNode) {}

    std::string ToString(int counter) const override {
        std::string ots(counter * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "SimpleDeclaration:\n" + child->ToString(counter + 1);
        return result;
    }
};

#endif // SIMPLEDECLARATION_H
