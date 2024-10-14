#ifndef PARAMETERDECLARATION_H
#define PARAMETERDECLARATION_H

#include "identifier.h"
#include "node.h"
#include "type.h"
#include <memory>

class ParameterDeclaration : public Node {
  public:
    ParameterDeclaration(std::shared_ptr<Identifier> identifier,
                         std::shared_ptr<Type> type)
        : identifier(identifier), type(type) {}

    std::string ToString(int counter) const override {
        std::string result = "";
        for (int i = 0; i < counter; i++) {
            result += " ";
        }
        result += "Parameter: " + identifier->ToString(counter) + "\n";
        for (int i = 0; i < counter + 2; i++) {
            result += " ";
        }
        return result += "Type: " + type->ToString(counter) + "\n";
    }

    void Print() const {
        std::cout << "ParameterDeclaration: " << ToString(2) << std::endl;
    }

  private:
    std::shared_ptr<Identifier> identifier;
    std::shared_ptr<Type> type;
};

#endif // PARAMETERDECLARATION_H
