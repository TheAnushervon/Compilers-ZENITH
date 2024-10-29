#include <iostream>
#include <queue>
#include <set>
#include <stack>
#include <memory>
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
#include "nodes/return_type.h"
#include "nodes/modifiable_primary.h"

class TypeChecker {
public:
    void GlobalAndRoutineScopeTypeCheck(const std::unique_ptr<Node>& root) {
        if (auto program = dynamic_cast<Program*>(root.get())) {
            std::queue<Node*> queue;       // Очередь для обхода программы
            std::set<std::string> globals; // Глобальные переменные
            std::stack<std::set<std::string>> scopes;


            queue.push(program);

            while (!queue.empty()) {
                Node* current = queue.front();
                queue.pop();

                if (auto prog = dynamic_cast<Program*>(current)) {
                    for (const auto& decl : prog->simpleDeclarations) queue.push(decl.get());
                    for (const auto& routine : prog->routineDeclarations) queue.push(routine.get());
                }

                else if (auto simpleDecl = dynamic_cast<SimpleDeclaration*>(current)) {
                    if (auto varDecl = dynamic_cast<VariableDeclaration*>(simpleDecl->child.get())) {
                        auto varName = dynamic_cast<Identifier*>(varDecl->identifier.get())->name;
                        if (!globals.insert(varName).second) {
                            std::cerr << "Error: Global variable '" << varName << "' is redeclared.\n";
                        }
                    }
                }

                else if (auto routineDecl = dynamic_cast<RoutineDeclaration*>(current)) {
                    scopes.push({});
                    queue.push(routineDecl->body.get());
                    auto returnType = dynamic_cast<Type*>(routineDecl->returnType.get());
                    std::string typeName;
                    if (returnType == nullptr) typeName = "void";

                    auto body = dynamic_cast<Body*>(routineDecl->body.get());
                    if (body->returnType) {
                        auto declaredReturnType = dynamic_cast<ReturnType*>(body->returnType.get());
                        auto modifiablePrimary = dynamic_cast<ModifiablePrimary*>(declaredReturnType->type.get());
                        auto identifier = dynamic_cast<Identifier*>(modifiablePrimary->identifier.get());
                        for (const auto& stmt : body->statements) {
                            if (auto simplDec = dynamic_cast<SimpleDeclaration*>(stmt.get())) {
                                auto varDecl = dynamic_cast<VariableDeclaration*>(simplDec->child.get());
                                auto varName = dynamic_cast<Identifier*>(varDecl->identifier.get())->name;
                                if (identifier->name == varName) {
                                    auto type = dynamic_cast<Type*>(varDecl->type.get());
                                    if (auto primitive = dynamic_cast<PrimitiveType*>(type->child.get())) {
                                        auto t = primitive->name;
                                        if (typeName != t) {
                                            std::cerr << "Error: Wrong return type '" << t << "', expected: '"<< typeName<<"'\n";
                                        }
                                    }
                                }
                            }
                        }
                    } else if (returnType != nullptr) {
                        std::cerr << "Error: Wrong return type '" << "void" << "', expected: '"<< typeName<<"'\n";
                    }
                    if (routineDecl->parameters) {
                        for (const auto& param : dynamic_cast<Parameters*>(routineDecl->parameters.get())->children) {
                            auto paramName = dynamic_cast<Identifier*>(dynamic_cast<ParameterDeclaration*>(param.get())->identifier.get())->name;
                            if (!scopes.top().insert(paramName).second) {
                                std::cerr << "Error: Parameter '" << paramName << "' is redeclared in routine.\n";
                            }
                        }
                    }
                }

                else if (auto body = dynamic_cast<Body*>(current)) {
                    for (const auto& stmt : body->statements) queue.push(stmt.get());
                }

                else if (auto varDecl = dynamic_cast<VariableDeclaration*>(current)) {
                    auto varName = dynamic_cast<Identifier*>(varDecl->identifier.get())->name;
                    if (!scopes.top().insert(varName).second) {
                        std::cerr << "Error: Variable '" << varName << "' redeclared in current scope.\n";
                    }
                }

                if (dynamic_cast<RoutineDeclaration*>(current) && !scopes.empty()) scopes.pop();
            }
        } else {
            std::cerr << "Error: Provided node is not of type Program.\n";
        }
    }

    void OptimizeAST(const std::unique_ptr<Node>& root) {
        if (auto program = dynamic_cast<Program*>(root.get())) {
            std::queue<Node*> queue;
            queue.push(program);


            while (!queue.empty()) {
                Node* current = queue.front();
                queue.pop();

                if (auto prog = dynamic_cast<Program*>(current)) {
                    for (const auto& routine : prog->routineDeclarations) queue.push(routine.get());
                }
                else if (auto routineDecl = dynamic_cast<RoutineDeclaration*>(current)) {
                    auto body = dynamic_cast<Body*>(routineDecl->body.get());
                    int amount = static_cast<int>(body->statements.size());
                    int oderOfReturn;

                    if (body->returnType) {
                        oderOfReturn = body->returnTypeIndex;
                    }
                    if (body->returnType && static_cast<int>(amount) > oderOfReturn) {
                        auto name = dynamic_cast<Identifier*>(routineDecl->identifier.get())->name;
                        std::cout << "Warning:\nIn the routine '" << name<< " ' exists unreachable code and will be removed at compile time";
                        int am = static_cast<int>(amount) - oderOfReturn;
                        while (am > 0) {
                           body->statements.erase(body->statements.begin() +oderOfReturn);
                            am--;
                        }
                    }

                }

            }
        } else {
            std::cerr << "Error: Provided node is not of type Program.\n";
        }
    }
};
