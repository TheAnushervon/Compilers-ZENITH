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

    std::string ToString(int counter) const override {
        std::string result = "";
        for (int i = 0; i < counter; i++){
            result += " ";
        }
        result += "Routine: " + routineName->ToString(counter) + "\n";

        if (returnType) {
            for (int i = 0; i < counter + 2; i++){
                result += " ";
            }
            result += "Type: " + returnType->ToString(counter) + "\n";
        }

        
        for (size_t i = 0; i < parameters.size(); ++i) {
            result+= parameters[i]->ToString(counter + 2);            
        }
        

        if (body) {
            result += body->ToString(counter + 2);
        }

        return result;
    }

    void Print() const {
        std::cout << "RoutineDeclaration: " << ToString(0) << std::endl;
    }

private:
    std::shared_ptr<Identifier> routineName;
    std::vector<std::shared_ptr<ParameterDeclaration>> parameters;
    std::shared_ptr<Type> returnType;
    std::shared_ptr<Body> body; 
};

#endif // ROUTINEDECLARATION_H
