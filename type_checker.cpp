#include "nodes/assignment.h"
#include "nodes/body.h"
#include "nodes/identifier.h"
#include "nodes/if_statement.h"
#include "nodes/modifiable_primary.h"
#include "nodes/node.h"
#include "nodes/parameter_declaration.h"
#include "nodes/parametrs.h"
#include "nodes/primitive_type.h"
#include "nodes/program.h"
#include "nodes/return_type.h"
#include "nodes/routine_declaration.h"
#include "nodes/simple_declaration.h"
#include "nodes/statement.h"
#include "nodes/type.h"
#include "nodes/variable_declaration.h"
#include "nodes/while_loop.h"
#include <iostream>
#include <memory>
#include <queue>
#include <set>
#include <stack>

class TypeChecker {
  public:
    void GlobalAndRoutineScopeTypeCheck(const std::unique_ptr<Node> &root) {
        if (auto program = dynamic_cast<Program *>(root.get())) {
            std::queue<Node *> queue;      // Очередь для обхода программы
            std::set<std::string> globals; // Глобальные переменные
            std::stack<std::set<std::string>> scopes;

            queue.push(program);

            while (!queue.empty()) {
                Node *current = queue.front();
                queue.pop();

                if (auto prog = dynamic_cast<Program *>(current)) {
                    for (const auto &decl : prog->simpleDeclarations)
                        queue.push(decl.get());
                    for (const auto &routine : prog->routineDeclarations)
                        queue.push(routine.get());
                }

                else if (auto simpleDecl =
                             dynamic_cast<SimpleDeclaration *>(current)) {
                    if (auto varDecl = dynamic_cast<VariableDeclaration *>(
                            simpleDecl->child.get())) {
                        auto varName = dynamic_cast<Identifier *>(
                                           varDecl->identifier.get())
                                           ->name;
                        if (!globals.insert(varName).second) {
                            std::cerr << "Error: Variable '" << varName
                                      << "' is redeclared.\n";
                        }
                    }
                }

                else if (auto routineDecl =
                             dynamic_cast<RoutineDeclaration *>(current)) {
                    scopes.push({});
                    queue.push(routineDecl->body.get());
                    auto returnType =
                        dynamic_cast<Type *>(routineDecl->returnType.get());
                    std::string typeName;
                    if (returnType == nullptr)
                        typeName = "void";
                    else {
                        if (auto pt = dynamic_cast<PrimitiveType *>(
                                returnType->child.get())) {
                            typeName = pt->name;
                        }
                    }

                    auto body = dynamic_cast<Body *>(routineDecl->body.get());
                    if (body->returnType) {
                        auto declaredReturnType =
                            dynamic_cast<ReturnType *>(body->returnType.get());
                        auto modifiablePrimary =
                            dynamic_cast<ModifiablePrimary *>(
                                declaredReturnType->type.get());
                        auto identifier = dynamic_cast<Identifier *>(
                            modifiablePrimary->identifier.get());
                        for (const auto &stmt : body->statements) {
                            if (auto simplDec =
                                    dynamic_cast<SimpleDeclaration *>(
                                        stmt.get())) {
                                auto varDecl =
                                    dynamic_cast<VariableDeclaration *>(
                                        simplDec->child.get());
                                auto varName = dynamic_cast<Identifier *>(
                                                   varDecl->identifier.get())
                                                   ->name;
                                if (identifier->name == varName) {
                                    auto type = dynamic_cast<Type *>(
                                        varDecl->type.get());
                                    if (auto primitive =
                                            dynamic_cast<PrimitiveType *>(
                                                type->child.get())) {
                                        auto t = primitive->name;
                                        if (typeName != t) {
                                            std::cerr
                                                << "Error: Wrong return type '"
                                                << t << "', expected: '"
                                                << typeName << "'\n";
                                        }
                                    }
                                }
                            }
                        }
                    } else if (returnType != nullptr) {
                        std::cerr << "Error: Wrong return type '" << "void"
                                  << "', expected: '" << typeName << "'\n";
                    }
                    if (routineDecl->parameters) {
                        for (const auto &param :
                             dynamic_cast<Parameters *>(
                                 routineDecl->parameters.get())
                                 ->children) {
                            auto paramName =
                                dynamic_cast<Identifier *>(
                                    dynamic_cast<ParameterDeclaration *>(
                                        param.get())
                                        ->identifier.get())
                                    ->name;
                            if (!scopes.top().insert(paramName).second) {
                                std::cerr << "Error: Parameter '" << paramName
                                          << "' is redeclared in routine.\n";
                            }
                        }
                    }
                }

                else if (auto body = dynamic_cast<Body *>(current)) {
                    for (const auto &stmt : body->statements)
                        queue.push(stmt.get());
                }

                else if (auto varDecl =
                             dynamic_cast<VariableDeclaration *>(current)) {
                    auto varName =
                        dynamic_cast<Identifier *>(varDecl->identifier.get())
                            ->name;
                    if (!scopes.top().insert(varName).second) {
                        std::cerr << "Error: Variable '" << varName
                                  << "' redeclared in current scope.\n";
                    }
                }

                if (dynamic_cast<RoutineDeclaration *>(current) &&
                    !scopes.empty())
                    scopes.pop();
            }
        } else {
            std::cerr << "Error: Provided node is not of type Program.\n";
        }
    }

    void OptimizeAST(const std::unique_ptr<Node> &root) {
        if (auto program = dynamic_cast<Program *>(root.get())) {
            // check return
            std::queue<Node *> queue;
            queue.push(program);

            while (!queue.empty()) {
                Node *current = queue.front();
                queue.pop();

                if (auto prog = dynamic_cast<Program *>(current)) {
                    for (const auto &routine : prog->routineDeclarations)
                        queue.push(routine.get());
                } else if (auto routineDecl =
                               dynamic_cast<RoutineDeclaration *>(current)) {
                    auto body = dynamic_cast<Body *>(routineDecl->body.get());
                    int amount = static_cast<int>(body->statements.size());
                    int oderOfReturn;

                    if (body->returnType) {
                        oderOfReturn = body->returnTypeIndex;
                    }
                    if (body->returnType &&
                        static_cast<int>(amount) > oderOfReturn) {
                        auto name = dynamic_cast<Identifier *>(
                                        routineDecl->identifier.get())
                                        ->name;
                        std::cout << "Warning:\nIn the routine '" << name
                                  << " ' exists unreachable code and will be "
                                     "removed at compile time\n";
                        int am = static_cast<int>(amount) - oderOfReturn;
                        while (am > 0) {
                            body->statements.erase(body->statements.begin() +
                                                   oderOfReturn);
                            am--;
                        }
                    }
                }
            }

            /// check if variables are declared, but not used
            if (auto prog = dynamic_cast<Program *>(root.get())) {

                for (const auto &routine : prog->routineDeclarations) {
                    std::queue<Node *> q;
                    q.push(routine.get());
                    std::set<std::string> willBeRemoved;
                    while (!q.empty()) {
                        std::set<std::string> declaredVariables;
                        std::set<std::string> modifiedVariables;
                        auto curr = q.front();
                        q.pop();

                        // thenBody, elseBody
                        if (auto st = dynamic_cast<Statement *>(curr)) {

                            if (auto whileDecl = dynamic_cast<WhileLoop *>(
                                    st->child.get())) {
                                auto whileBody =
                                    dynamic_cast<Body *>(whileDecl->body.get());
                                for (const auto &item : whileBody->statements) {
                                    if (auto sd =
                                            dynamic_cast<SimpleDeclaration *>(
                                                item.get())) {

                                        auto v =
                                            dynamic_cast<VariableDeclaration *>(
                                                sd->child.get());

                                        auto name = dynamic_cast<Identifier *>(
                                                        v->identifier.get())
                                                        ->name;
                                        declaredVariables.insert(name);
                                    }
                                    if (auto st = dynamic_cast<Statement *>(
                                            item.get())) {
                                        if (auto assig =
                                                dynamic_cast<Assignment *>(
                                                    st->child.get())) {
                                            auto v = dynamic_cast<
                                                ModifiablePrimary *>(
                                                assig->modifiablePrimary.get());
                                            auto name =
                                                dynamic_cast<Identifier *>(
                                                    v->identifier.get())
                                                    ->name;
                                            modifiedVariables.insert(name);
                                        } else {
                                            q.push(st->child.get());
                                        }
                                    }
                                }
                                for (const auto &vd : declaredVariables) {
                                    if (modifiedVariables.count(vd) == 0) {
                                        std::cout
                                            << "Warning:\nThe variable '" << vd
                                            << "' is declared but not used\n";
                                        willBeRemoved.insert(vd);
                                    }
                                }

                                for (const auto &vd : modifiedVariables) {
                                    if (declaredVariables.count(vd) == 0) {
                                        std::cerr << "Error:\nThe variable '"
                                                  << vd
                                                  << "' is not declared in the "
                                                     "scope\n";
                                    }
                                }
                                int index = 0;
                                int amount =
                                    static_cast<int>(willBeRemoved.size());
                                while (amount > 0) {
                                    const auto &item =
                                        whileBody->statements[index];
                                    if (auto sd =
                                            dynamic_cast<SimpleDeclaration *>(
                                                item.get())) {

                                        auto v =
                                            dynamic_cast<VariableDeclaration *>(
                                                sd->child.get());

                                        auto name = dynamic_cast<Identifier *>(
                                                        v->identifier.get())
                                                        ->name;
                                        if (willBeRemoved.count(name) != 0) {
                                            whileBody->statements.erase(
                                                whileBody->statements.begin() +
                                                index);
                                            index--;
                                            amount--;
                                        }
                                    }
                                    if (auto st = dynamic_cast<Statement *>(
                                            item.get())) {
                                        if (auto assig =
                                                dynamic_cast<Assignment *>(
                                                    st->child.get())) {
                                            auto v = dynamic_cast<
                                                ModifiablePrimary *>(
                                                assig->modifiablePrimary.get());
                                            auto name =
                                                dynamic_cast<Identifier *>(
                                                    v->identifier.get())
                                                    ->name;
                                            if (willBeRemoved.count(name) !=
                                                0) {
                                                whileBody->statements.erase(
                                                    whileBody->statements
                                                        .begin() +
                                                    index);
                                                index--;
                                                amount--;
                                            }
                                        }
                                    }
                                    index++;
                                }
                            }

                            if (auto ifDecl = dynamic_cast<IfStatement *>(
                                    st->child.get())) {
                                willBeRemoved.clear();
                                declaredVariables.clear();
                                modifiedVariables.clear();
                                auto thenBody = dynamic_cast<Body *>(
                                    ifDecl->thenBody.get());
                                for (const auto &item : thenBody->statements) {
                                    if (auto sd =
                                            dynamic_cast<SimpleDeclaration *>(
                                                item.get())) {

                                        auto v =
                                            dynamic_cast<VariableDeclaration *>(
                                                sd->child.get());

                                        auto name = dynamic_cast<Identifier *>(
                                                        v->identifier.get())
                                                        ->name;
                                        declaredVariables.insert(name);
                                    }
                                    if (auto st = dynamic_cast<Statement *>(
                                            item.get())) {
                                        if (auto assig =
                                                dynamic_cast<Assignment *>(
                                                    st->child.get())) {
                                            auto v = dynamic_cast<
                                                ModifiablePrimary *>(
                                                assig->modifiablePrimary.get());
                                            auto name =
                                                dynamic_cast<Identifier *>(
                                                    v->identifier.get())
                                                    ->name;
                                            modifiedVariables.insert(name);
                                        } else {
                                            q.push(st->child.get());
                                        }
                                    }
                                }
                                for (const auto &vd : declaredVariables) {
                                    if (modifiedVariables.count(vd) == 0) {
                                        std::cout
                                            << "Warning:\nThe variable '" << vd
                                            << "' is declared but not used\n";
                                        willBeRemoved.insert(vd);
                                    }
                                }
                                for (const auto &vd : modifiedVariables) {
                                    if (declaredVariables.count(vd) == 0) {
                                        std::cerr << "Error:\nThe variable '"
                                                  << vd
                                                  << "' is not declared in the "
                                                     "scope\n";
                                    }
                                }
                                int index = 0;
                                int amount =
                                    static_cast<int>(willBeRemoved.size());
                                while (amount > 0) {
                                    const auto &item =
                                        thenBody->statements[index];
                                    if (auto sd =
                                            dynamic_cast<SimpleDeclaration *>(
                                                item.get())) {

                                        auto v =
                                            dynamic_cast<VariableDeclaration *>(
                                                sd->child.get());

                                        auto name = dynamic_cast<Identifier *>(
                                                        v->identifier.get())
                                                        ->name;
                                        if (willBeRemoved.count(name) != 0) {
                                            thenBody->statements.erase(
                                                thenBody->statements.begin() +
                                                index);
                                            index--;
                                            amount--;
                                        }
                                    }
                                    if (auto st = dynamic_cast<Statement *>(
                                            item.get())) {
                                        if (auto assig =
                                                dynamic_cast<Assignment *>(
                                                    st->child.get())) {
                                            auto v = dynamic_cast<
                                                ModifiablePrimary *>(
                                                assig->modifiablePrimary.get());
                                            auto name =
                                                dynamic_cast<Identifier *>(
                                                    v->identifier.get())
                                                    ->name;
                                            if (willBeRemoved.count(name) !=
                                                0) {
                                                thenBody->statements.erase(
                                                    thenBody->statements
                                                        .begin() +
                                                    index);
                                                index--;
                                                amount--;
                                            }
                                        }
                                    }
                                    index++;
                                }
                                if (auto elseBody = dynamic_cast<Body *>(
                                        ifDecl->elseBody.get())) {
                                    declaredVariables.clear();
                                    modifiedVariables.clear();
                                    willBeRemoved.clear();
                                    for (const auto &item :
                                         elseBody->statements) {
                                        if (auto sd = dynamic_cast<
                                                SimpleDeclaration *>(
                                                item.get())) {

                                            auto v = dynamic_cast<
                                                VariableDeclaration *>(
                                                sd->child.get());

                                            auto name =
                                                dynamic_cast<Identifier *>(
                                                    v->identifier.get())
                                                    ->name;

                                            declaredVariables.insert(name);
                                        }
                                        if (auto st = dynamic_cast<Statement *>(
                                                item.get())) {
                                            if (auto assig =
                                                    dynamic_cast<Assignment *>(
                                                        st->child.get())) {
                                                auto v = dynamic_cast<
                                                    ModifiablePrimary *>(
                                                    assig->modifiablePrimary
                                                        .get());
                                                auto name =
                                                    dynamic_cast<Identifier *>(
                                                        v->identifier.get())
                                                        ->name;
                                                modifiedVariables.insert(name);

                                            } else {
                                                q.push(st->child.get());
                                            }
                                        }
                                    }
                                    for (const auto &vd : declaredVariables) {
                                        if (modifiedVariables.count(vd) == 0) {
                                            std::cout
                                                << "Warning:\nThe variable '"
                                                << vd
                                                << "' is declared but not "
                                                   "used\n";
                                            willBeRemoved.insert(vd);
                                        }
                                    }
                                    for (const auto &vd : modifiedVariables) {
                                        if (declaredVariables.count(vd) == 0) {
                                            std::cerr
                                                << "Error:\nThe variable '"
                                                << vd
                                                << "' is not declared in the "
                                                   "scope\n";
                                        }
                                    }
                                    index = 0;
                                    amount =
                                        static_cast<int>(willBeRemoved.size());

                                    while (amount > 0) {

                                        const auto &item =
                                            elseBody->statements[index];
                                        if (auto sd = dynamic_cast<
                                                SimpleDeclaration *>(
                                                item.get())) {

                                            auto v = dynamic_cast<
                                                VariableDeclaration *>(
                                                sd->child.get());

                                            auto name =
                                                dynamic_cast<Identifier *>(
                                                    v->identifier.get())
                                                    ->name;
                                            if (willBeRemoved.count(name) !=
                                                0) {
                                                elseBody->statements.erase(
                                                    elseBody->statements
                                                        .begin() +
                                                    index);
                                                index--;
                                                amount--;
                                            }
                                        }
                                        if (auto st = dynamic_cast<Statement *>(
                                                item.get())) {
                                            if (auto assig =
                                                    dynamic_cast<Assignment *>(
                                                        st->child.get())) {
                                                auto v = dynamic_cast<
                                                    ModifiablePrimary *>(
                                                    assig->modifiablePrimary
                                                        .get());
                                                auto name =
                                                    dynamic_cast<Identifier *>(
                                                        v->identifier.get())
                                                        ->name;
                                                if (willBeRemoved.count(name) !=
                                                    0) {
                                                    elseBody->statements.erase(
                                                        elseBody->statements
                                                            .begin() +
                                                        index);
                                                    index--;
                                                    amount--;
                                                }
                                            }
                                        }
                                        index++;
                                    }
                                }
                            }

                        }
                        // returnType / Body
                        else if (auto routuneDecl =
                                     dynamic_cast<RoutineDeclaration *>(curr)) {
                            declaredVariables.clear();
                            modifiedVariables.clear();
                            willBeRemoved.clear();

                            if (routuneDecl->returnType) {
                                if (auto var =
                                        dynamic_cast<ModifiablePrimary *>(
                                            routuneDecl->returnType.get())) {
                                    if (auto primVar =
                                            dynamic_cast<Identifier *>(
                                                var->identifier.get())) {
                                        modifiedVariables.insert(primVar->name);
                                    }
                                }
                            }

                            if (routuneDecl->body) {

                                auto bodyDecl = dynamic_cast<Body *>(
                                    routuneDecl->body.get());
                                for (const auto &item : bodyDecl->statements) {
                                    if (auto sd =
                                            dynamic_cast<SimpleDeclaration *>(
                                                item.get())) {

                                        auto v =
                                            dynamic_cast<VariableDeclaration *>(
                                                sd->child.get());

                                        auto name = dynamic_cast<Identifier *>(
                                                        v->identifier.get())
                                                        ->name;
                                        declaredVariables.insert(name);
                                    }
                                    if (auto st = dynamic_cast<Statement *>(
                                            item.get())) {
                                        if (auto assig =
                                                dynamic_cast<Assignment *>(
                                                    st->child.get())) {
                                            auto v = dynamic_cast<
                                                ModifiablePrimary *>(
                                                assig->modifiablePrimary.get());
                                            auto name =
                                                dynamic_cast<Identifier *>(
                                                    v->identifier.get())
                                                    ->name;
                                            modifiedVariables.insert(name);
                                        } else {
                                            q.push(item.get());
                                        }
                                    }
                                }
                                for (const auto &vd : declaredVariables) {
                                    if (modifiedVariables.count(vd) == 0) {
                                        std::cout
                                            << "Warning:\nThe variable '" << vd
                                            << "' is declared but not used\n";
                                        willBeRemoved.insert(vd);
                                    }
                                }
                                for (const auto &vd : modifiedVariables) {
                                    if (declaredVariables.count(vd) == 0) {
                                        std::cerr << "Error:\nThe variable '"
                                                  << vd
                                                  << "' is not declared in the "
                                                     "scope\n";
                                    }
                                }

                                int index = 0;
                                int amount =
                                    static_cast<int>(willBeRemoved.size());

                                while (amount > 0) {
                                    const auto &item =
                                        bodyDecl->statements[index];
                                    if (auto sd =
                                            dynamic_cast<SimpleDeclaration *>(
                                                item.get())) {

                                        auto v =
                                            dynamic_cast<VariableDeclaration *>(
                                                sd->child.get());

                                        auto name = dynamic_cast<Identifier *>(
                                                        v->identifier.get())
                                                        ->name;
                                        if (willBeRemoved.count(name) != 0) {
                                            bodyDecl->statements.erase(
                                                bodyDecl->statements.begin() +
                                                index);
                                            index--;
                                            amount--;
                                        }
                                    }
                                    if (auto st = dynamic_cast<Statement *>(
                                            item.get())) {
                                        if (auto assig =
                                                dynamic_cast<Assignment *>(
                                                    st->child.get())) {
                                            auto v = dynamic_cast<
                                                ModifiablePrimary *>(
                                                assig->modifiablePrimary.get());
                                            auto name =
                                                dynamic_cast<Identifier *>(
                                                    v->identifier.get())
                                                    ->name;
                                            if (willBeRemoved.count(name) !=
                                                0) {
                                                bodyDecl->statements.erase(
                                                    bodyDecl->statements
                                                        .begin() +
                                                    index);
                                                index--;
                                                amount--;
                                            }
                                        }
                                    }
                                    index++;
                                }
                            }
                        }
                    }
                }
            }
        } else {
            std::cerr << "Error: Provided node is not of type Program.\n";
        }
    }
};
