#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
#include <memory>
#include <set>
#include <stack>
#include "nodes/node.h"
#include "nodes/body.h"
#include "nodes/program.h"
#include "nodes/simple_declaration.h"
#include "nodes/variable_declaration.h"
#include "nodes/routine_declaration.h"
#include "nodes/identifier.h"
#include "nodes/primitive_type.h"
#include "nodes/parametrs.h"
#include "nodes/parameter_declaration.h"

class TypeChecker {
public:
    void GlobalAndRoutineScopeTypeCheck(const std::unique_ptr<Node>& root) {
        if (auto program = dynamic_cast<Program*>(root.get())) {
            std::queue<Node*> queue; 
            std::set<std::string> globalVariables; 
            std::stack<std::set<std::string>> scopeStack; 

            queue.push(program); 

            while (!queue.empty()) {
                Node* current = queue.front();
                queue.pop();

              
                if (auto prog = dynamic_cast<Program*>(current)) {
                    for (const auto& decl : prog->simpleDeclarations) {
                        queue.push(decl.get());
                    }
                    for (const auto& routine : prog->routineDeclarations) {
                        queue.push(routine.get());
                    }
                }
              
                else if (auto simpleDecl = dynamic_cast<SimpleDeclaration*>(current)) {
                    if (auto varDecl = dynamic_cast<VariableDeclaration*>(simpleDecl->child.get())) {
                        auto identifier = dynamic_cast<Identifier*>(varDecl->identifier.get());
                        if (!identifier) {
                            std::cerr << "Error: Unable to retrieve variable name." << std::endl;
                            continue;
                        }

                        std::string varName = identifier->name;

                        if (globalVariables.count(varName) > 0) {
                            std::cerr << "Error: Global variable '" << varName 
                                      << "' is redeclared in the global scope." << std::endl;
                        } else {
                            globalVariables.insert(varName);
                        }
                    }
                }
                else if (auto routineDecl = dynamic_cast<RoutineDeclaration*>(current)) {
                    scopeStack.push({});

                    if (routineDecl->body) {
                        queue.push(routineDecl->body.get());
                    }

                    if (routineDecl->parameters) {
                        for (const auto& param : dynamic_cast<Parameters*>(routineDecl->parameters.get())->children) {
                            auto paramDecl = dynamic_cast<ParameterDeclaration*>(param.get());
                            if (paramDecl) {
                                std::string paramName = dynamic_cast<Identifier*>(paramDecl->identifier.get())->name;
                                if (scopeStack.top().count(paramName) > 0) {
                                    std::cerr << "Error: Parameter '" << paramName 
                                              << "' is redeclared in routine scope." << std::endl;
                                } else {
                                    scopeStack.top().insert(paramName);
                                }
                            }
                        }
                    }
                }
      else if (auto body = dynamic_cast<Body*>(current)) {
                    for (const auto& stmt : body->statements) {
                        queue.push(stmt.get());
                    }
                }
               
                else if (auto varDecl = dynamic_cast<VariableDeclaration*>(current)) {
                    auto identifier = dynamic_cast<Identifier*>(varDecl->identifier.get());
                    if (!identifier) {
                        std::cerr << "Error: Unable to retrieve variable name." << std::endl;
                        continue;
                    }

                    std::string varName = identifier->name;

                    if (scopeStack.top().count(varName) > 0) {
                        std::cerr << "Error: Variable '" << varName 
                                  << "' is redeclared in the current routine scope." << std::endl;
                    } else {
                        scopeStack.top().insert(varName);
                    }
                }

                if (auto routineDecl = dynamic_cast<RoutineDeclaration*>(current)) {
                    if (!scopeStack.empty()) {
                        scopeStack.pop();
                    }
                }
            }
        } else {
            std::cerr << "Error: The provided node is not of type Program.\n";
        }
    }
};
