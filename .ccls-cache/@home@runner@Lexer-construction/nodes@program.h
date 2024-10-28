#ifndef PROGRAM_H
#define PROGRAM_H

#include "node.h"

#include <memory>
#include <vector>

class Program : public Node {
  public:
    //Program : { SimpleDeclaration | RoutineDeclaration }
    std::vector<std::shared_ptr<Node>> simpleDeclarations;
    std::vector<std::shared_ptr<Node>> routineDeclarations;
    Program(const std::vector<std::shared_ptr<Node>>
                &simpleDeclarations = {},
            const std::vector<std::shared_ptr<Node>>
                &routineDeclarations = {})
        : simpleDeclarations(simpleDeclarations),
          routineDeclarations(routineDeclarations) {}

    void AddSimpleDeclaration(std::shared_ptr<Node> declaration) {
        simpleDeclarations.push_back(declaration);
    }

    void
    AddRoutineDeclaration(std::shared_ptr<Node> declaration) {
        routineDeclarations.push_back(declaration);
    }

    std::string ToString(int counter) const override {
        std::string result = "Program:\n";

        for (const auto &decl : simpleDeclarations) {
            result += decl->ToString(counter) + "\n";
        }

        for (const auto &routine : routineDeclarations) {
            result += routine->ToString(counter) + "\n";
        }

        return result;
    }

};

#endif // PROGRAM_H
