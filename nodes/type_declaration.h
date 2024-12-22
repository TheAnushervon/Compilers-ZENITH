#ifndef TYPEDECLARATION_H
#define TYPEDECLARATION_H

#include <memory>
#include "simple_declaration.h"
#include "identifier.h"
#include "type.h"

class TypeDeclaration : public SimpleDeclaration {
public:
    TypeDeclaration(std::shared_ptr<Identifier> identifier, std::shared_ptr<Type> type)
        : identifier(identifier), type(type) {}

    std::string ToString() const override {
        return "type " + identifier->ToString() + " is " + type->ToString();
    }

private:
    std::shared_ptr<Identifier> identifier;
    std::shared_ptr<Type> type;
};

#endif // TYPEDECLARATION_H
