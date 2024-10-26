#ifndef ARRAYTYPE_H
#define ARRAYTYPE_H

#include <memory>
#include "node.h"

class ArrayType : public Node {
public:
    ArrayType(std::shared_ptr<Node> elementType, std::shared_ptr<Node> expression = nullptr)
        : type(elementType), expression(expression) {}

    std::string ToString(int count) const override {
        std::string result = "ArrayType: ";
        if (expression) {
            result += expression->ToString(count);
        } else {
            result += "No expression";
        }
        result += type->ToString(count);
        return result;
    }

private:
    std::shared_ptr<Node> type;  
    std::shared_ptr<Node> expression;   
};

#endif // ARRAYTYPE_H
