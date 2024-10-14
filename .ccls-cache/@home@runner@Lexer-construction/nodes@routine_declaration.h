#ifndef ROUTINEDECLARATION_H
#define ROUTINEDECLARATION_H

#include <vector>
#include <memory>
#include "node.h"
#include "identifier.h"
#include "type.h"
#include "parameter_declaration.h"
#include "statement.h"
#include "body_declaration.h"

class RoutineDeclaration : public Node {
public:
RoutineDeclaration(std::shared_ptr<Identifier> routineName,
               const std::vector<std::shared_ptr<ParameterDeclaration>>& parameters,
               std::shared_ptr<Type> returnType = nullptr,
               std::shared_ptr<Body> body = nullptr)
: routineName(routineName), parameters(parameters), returnType(returnType), body(body) {}

    std::string ToString() const override {
        std::string result = "routine " + routineName->ToString() + "(";

        for (size_t i = 0; i < parameters.size(); ++i) {
            result += parameters[i]->ToString();
            if (i < parameters.size() - 1) result += ", ";
        }
        result += ")";

        if (returnType) {
            result += " : " + returnType->ToString();
        }

        result += " is\n";

        if (body) {
            result += "  " + body->ToString();
        }

        result += "\nend";
        return result;
    }

    void Print() const {
        std::cout << "RoutineDeclaration: " << ToString() << std::endl;
    }

private:
    std::shared_ptr<Identifier> routineName;
    std::vector<std::shared_ptr<ParameterDeclaration>> parameters;
    std::shared_ptr<Type> returnType;
    std::shared_ptr<Body> body; 
};

#endif // ROUTINEDECLARATION_H
