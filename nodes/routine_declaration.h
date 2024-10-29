#ifndef ROUTINEDECLARATION_H
#define ROUTINEDECLARATION_H

#include <memory>
#include <vector>
#include "node.h"

class RoutineDeclaration : public Node {
public:
    std::shared_ptr<Node> identifier;
    std::shared_ptr<Node> parameters;
    std::shared_ptr<Node> returnType;  
    std::shared_ptr<Node> body;

    RoutineDeclaration(std::shared_ptr<Node> identifier, 
                       std::shared_ptr<Node> parameters, 
                       std::shared_ptr<Node> returnType = nullptr, 
                       std::shared_ptr<Node> body = nullptr)
        : identifier(identifier), parameters(parameters), returnType(returnType), body(body) {}

    std::string ToString(int counter) const override {
        std::string ots(counter * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "RoutineDeclaration:\n";

        result += identifier->ToString(counter + 2) + "\n";
        result += parameters->ToString(counter + 2) + "\n";

        if (returnType) {
            result += ots + "Expected return type: "+returnType->ToString(counter + 2) + "\n";
        } else {
            result += ots + "Expected return type:  void\n";
        }

        if (body) {
            result += body->ToString(counter + 2);
        } else {
            result += ots + "  No body\n";
        }

        return result;
    }
};

#endif // ROUTINEDECLARATION_H
