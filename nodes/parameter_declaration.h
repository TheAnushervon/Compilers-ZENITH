#ifndef PARAMETERDECLARATION_H
#define PARAMETERDECLARATION_H

#include <memory>
#include "node.h"
#include "identifier.h"
#include "type.h"

class ParameterDeclaration : public Node {
public:
    ParameterDeclaration(std::shared_ptr<Identifier> identifier, std::shared_ptr<Type> type)
        : identifier(identifier), type(type) {}

    std::string ToString() const override {
        return identifier->ToString() + " : " + type->ToString();
    }

    void Print() const {
        std::cout << "ParameterDeclaration: " << ToString() << std::endl;
    }

private:
    std::shared_ptr<Identifier> identifier; 
    std::shared_ptr<Type> type;            
};

#endif // PARAMETERDECLARATION_H
