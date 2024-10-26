#ifndef MODIFIABLE_PRIMARY_H
#define MODIFIABLE_PRIMARY_H

#include "node.h"
#include <string>
#include <memory>
#include <vector>

class ModifiablePrimary : public Node {
public:
    ModifiablePrimary() : identifier(nullptr), specialIdentifier(nullptr), expression(nullptr) {}

    std::string ToString(int counter) const override {
        std::string result =  "ModifiablePrimary: " + identifier->ToString(counter + 2) + "\n";
        if (specialIdentifier != nullptr) {
            result += specialIdentifier->ToString(counter + 2);
        }
        if (expression != nullptr) {
            result += expression->ToString(counter + 2);
        }
        return result;
    }

private:
    // Identifier { . Identifier | [ Expression ] }
    std::shared_ptr<Node> identifier;
    //по сути это Identifier
    std::shared_ptr<Node> specialIdentifier;  
    std::shared_ptr<Node> expression;         
};

#endif // MODIFIABLE_PRIMARY_H
