#ifndef PRIMARY_H
#define PRIMARY_H

#include <vector>
#include <memory>
#include "node.h"

class Primary : public Node {
public:
    // : IntegralLiteral 
    // | RealLiteral 
    // | true | false 
    // | ModifiablePrimary 
    std::shared_ptr<Node> child;

    Primary(std::shared_ptr<Node> childNode) : child(childNode) {}

    std::string ToString(int counter) const override {
        std::string ots(counter * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "Primary:\n" + child->ToString(counter + 1);
        return result;
    } 
};

#endif // PRIMARY_H
