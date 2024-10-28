#ifndef TYPEDECLARATION_H
#define TYPEDECLARATION_H

#include <memory>
#include "node.h"

class TypeDeclaration : public Node {
public:
    std::shared_ptr<Node> identifier;
    std::shared_ptr<Node> type;

    TypeDeclaration(std::shared_ptr<Node> identifier, std::shared_ptr<Node> type)
        : identifier(identifier), type(type) {}

    std::string ToString(int counter) const override {
        std::string ots(counter * 2, ' '); // Отступы с учетом уровня вложенности
        return ots + "TypeDeclaration:\n" +
               identifier->ToString(counter + 2) + 
               type->ToString(counter + 2);
    }
};

#endif // TYPEDECLARATION_H
