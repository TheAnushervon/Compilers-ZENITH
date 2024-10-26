#ifndef PARAMETERDECLARATION_H
#define PARAMETERDECLARATION_H

#include "identifier.h"
#include "node.h"
#include "type.h"
#include <memory>

class ParameterDeclaration : public Node {
  public:
    ParameterDeclaration(std::shared_ptr<Node> identifier,
                         std::shared_ptr<Node> type)
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

  private:
    std::shared_ptr<Node> identifier;
    std::shared_ptr<Node> type;
};

#endif // PARAMETERDECLARATION_H
