#ifndef VARIABLEDECLARATION_H
#define VARIABLEDECLARATION_H

#include "node.h"
#include <memory>

class VariableDeclaration : public Node {
public:
    std::shared_ptr<Node> identifier;
    std::shared_ptr<Node> type;
    std::shared_ptr<Node> expression;

    VariableDeclaration(std::shared_ptr<Node> identifier,
                        std::shared_ptr<Node> type = nullptr,
                        std::shared_ptr<Node> expression = nullptr)
        : identifier(identifier), type(type), expression(expression) {}

    std::string ToString(int counter) const override {
        std::string ots(counter * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "VariableDeclaration:\n";

        result += identifier->ToString(counter + 1);

        if (type != nullptr) {
            result += type->ToString(counter + 1);
        }
        if (expression != nullptr) {
            result += expression->ToString(counter + 1);
        }

        return result;
    }
};

#endif // VARIABLEDECLARATION_H
