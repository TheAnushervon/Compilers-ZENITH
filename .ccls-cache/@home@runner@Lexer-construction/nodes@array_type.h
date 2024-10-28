#ifndef ARRAYTYPE_H
#define ARRAYTYPE_H

#include <memory>
#include "node.h"

class ArrayType : public Node {
public:
    std::shared_ptr<Node> type;  
    std::shared_ptr<Node> expression;

    ArrayType(std::shared_ptr<Node> elementType, std::shared_ptr<Node> expression = nullptr)
        : type(elementType), expression(expression) {}

    std::string ToString(int count) const override {
        std::string ots(count * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "ArrayType:\n";

        if (expression) {
            result += ots + "  Expression:\n" + expression->ToString(count + 2);
        } else {
            result += ots + "  Expression: None\n";
        }

        result += ots + "  Type:\n" + type->ToString(count + 2);

        return result;
    }
};

#endif // ARRAYTYPE_H
