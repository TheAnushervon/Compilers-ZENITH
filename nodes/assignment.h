#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include "node.h"
#include <memory>

class Assignment : public Node {
public:
    //: ModifiablePrimary := Expression 
    std::shared_ptr<Node> modifiablePrimary;
    std::shared_ptr<Node> expression;

    Assignment(std::shared_ptr<Node> modifiablePrim, std::shared_ptr<Node> expr)
        : modifiablePrimary(modifiablePrim), expression(expr) {}

    std::string ToString(int count) const override {
        std::string ots(count * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "Assignment:\n";

        result += modifiablePrimary->ToString(count + 2);
        result += expression->ToString(count + 2);

        return result;
    }
};

#endif // ASSIGNMENT_H
