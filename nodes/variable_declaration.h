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
        std::string result = "VariableDeclaration :";
        result += identifier->ToString(counter);
        
        if (type != nullptr) {
            result += " = " + expression->ToString(counter);
        }
        if (expression != nullptr) {
            result += " = " + expression->ToString(counter);
        }
        return result;
    }


};

#endif // VARIABLEDECLARATION_H
