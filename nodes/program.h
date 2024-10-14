#ifndef PROGRAM_H
#define PROGRAM_H

#include <vector>
#include <memory>
#include "node.h"
#include "simple_declaration.h"
#include "routine_declaration.h"

class Program : public Node {
public:
  
    Program(const std::vector<std::shared_ptr<SimpleDeclaration>>& simpleDeclarations = {},
            const std::vector<std::shared_ptr<RoutineDeclaration>>& routineDeclarations = {})
        : simpleDeclarations(simpleDeclarations), routineDeclarations(routineDeclarations) {}

    void AddSimpleDeclaration(std::shared_ptr<SimpleDeclaration> declaration) {
        simpleDeclarations.push_back(declaration);
    }

    void AddRoutineDeclaration(std::shared_ptr<RoutineDeclaration> declaration) {
        routineDeclarations.push_back(declaration);
    }

    std::string ToString() const override {
        std::string result = "Program:\n";

       
        for (const auto& decl : simpleDeclarations) {
            result += "  " + decl->ToString() + ";\n";
        }

     
        for (const auto& routine : routineDeclarations) {
            result += "  " + routine->ToString() + "\n";
        }

        return result;
    }

   
    void Print()  {
        std::cout << ToString() << std::endl;
    }

private:
    std::vector<std::shared_ptr<SimpleDeclaration>> simpleDeclarations; 
    std::vector<std::shared_ptr<RoutineDeclaration>> routineDeclarations; 
};

#endif // PROGRAM_H
