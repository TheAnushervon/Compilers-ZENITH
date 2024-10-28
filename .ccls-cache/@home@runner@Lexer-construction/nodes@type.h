#ifndef TYPE_H
#define TYPE_H

#include "node.h"
#include <memory>
#include <string>

class Type : public Node {
public:
    // PrimitiveType | UserType | Identifier 
    std::shared_ptr<Node> child;

    Type(std::shared_ptr<Node> childNode) : child(childNode) {}

    std::string ToString(int counter) const override {
        std::string ots(counter*2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "Type:\n" + child->ToString(counter + 2);
        return result; 
    }
};

#endif // TYPE_H
