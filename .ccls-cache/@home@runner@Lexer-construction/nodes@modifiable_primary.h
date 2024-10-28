#ifndef MODIFIABLE_PRIMARY_H
#define MODIFIABLE_PRIMARY_H

#include "node.h"
#include <string>
#include <memory>
#include <vector>

class ModifiablePrimary : public Node {
public:
    // Identifier { . Identifier | [ Expression ] }
    std::shared_ptr<Node> identifier;
    // по сути это Identifier
    std::shared_ptr<Node> specialIdentifier;  
    std::shared_ptr<Node> expression; 

    ModifiablePrimary() : identifier(nullptr), specialIdentifier(nullptr), expression(nullptr) {}

    std::string ToString(int counter) const override {
        std::string ots(counter * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "ModifiablePrimary:\n";

        result += identifier->ToString(counter + 1) + "\n";
        if (specialIdentifier != nullptr) {
            result += specialIdentifier->ToString(counter + 1);
        }
        if (expression != nullptr) {
            result += expression->ToString(counter + 1);
        }

        return result;
    }
};

#endif // MODIFIABLE_PRIMARY_H
