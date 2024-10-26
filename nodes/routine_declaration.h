#ifndef ROUTINEDECLARATION_H
#define ROUTINEDECLARATION_H

#include <memory>
#include <vector>
#include "node.h"

class RoutineDeclaration : public Node {
public:
    RoutineDeclaration(std::shared_ptr<Node> identifier, 
                       std::shared_ptr<Node> parameters, 
                       std::shared_ptr<Node> returnType = nullptr, 
                       std::shared_ptr<Node> body = nullptr)
        : identifier(identifier), parameters(parameters), returnType(returnType), body(body) {}

    std::string ToString(int counter) const override {
        std::string result = "RoutineDeclaration: ";
        result += identifier->ToString(counter) + " ";
        result += parameters->ToString(counter) + " ";

        if (returnType) {
            result += returnType->ToString(counter) + " ";
        } else {
            result += "void ";
        }

        if (body) {
            result += body->ToString(counter);
        } else {
            result += "No body";
        }

        return result;
    }

private:
    std::shared_ptr<Node> identifier;
    std::shared_ptr<Node> parameters;
    std::shared_ptr<Node> returnType;  
    std::shared_ptr<Node> body;
};

#endif // ROUTINEDECLARATION_H
