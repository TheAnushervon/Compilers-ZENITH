#ifndef PARAMETERDECLARATION_H
#define PARAMETERDECLARATION_H

#include "identifier.h"
#include "node.h"
#include <memory>

class ParameterDeclaration : public Node {
public:
    std::shared_ptr<Node> identifier;
    std::shared_ptr<Node> type;

    ParameterDeclaration(std::shared_ptr<Node> identifier,
                         std::shared_ptr<Node> type)
        : identifier(identifier), type(type) {}

    std::string ToString(int counter) const override {
        std::string ots(counter * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "ParameterDeclaration:\n";

        result += identifier->ToString(counter + 2);
        result += type->ToString(counter + 2);

        return result;
    }
};

#endif // PARAMETERDECLARATION_H
