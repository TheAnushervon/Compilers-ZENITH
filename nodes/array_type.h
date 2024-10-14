#ifndef ARRAYTYPE_H
#define ARRAYTYPE_H

#include <memory>
#include "type.h"
#include "expression.h"

class ArrayType : public Type {
public:

    ArrayType(std::shared_ptr<Type> elementType, std::shared_ptr<Expression> size = nullptr)
        : elementType(elementType), size(size) {}

    std::string ToString() const override {
        std::string result = "array [";
        if (size) {
            result += size->ToString();
        }
        result += "] of " + elementType->ToString();
        return result;
    }

private:
    std::shared_ptr<Type> elementType;  
    std::shared_ptr<Expression> size;   
};

#endif // ARRAYTYPE_H
