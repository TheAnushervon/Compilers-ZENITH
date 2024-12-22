// body_declaration.h
#ifndef BODY_H
#define BODY_H

#include "node.h"
#include "simple_declaration.h"
#include "statement.h"
#include <memory>
#include <vector>

class Body : public Statement {
  public:
    Body(const std::vector<std::shared_ptr<SimpleDeclaration>> &declarations,
         const std::vector<std::shared_ptr<Statement>> &statements)
        : declarations(declarations), statements(statements) {}

    void AddDeclaration(std::shared_ptr<SimpleDeclaration> declaration) {
        declarations.push_back(declaration);
    }

    void AddStatement(std::shared_ptr<Statement> statement) {
        statements.push_back(statement);
    }

    std::string ToString() const override {
        std::string result = "{\n";
        for (const auto &decl : declarations) {
            result += "  " + decl->ToString() + ";\n";
        }
        for (const auto &stmt : statements) {
            result += "  " + stmt->ToString() + ";\n";
        }
        result += "}";
        return result;
    }

    void Print() const override {
        std::cout << "Body: " << ToString() << std::endl;
    }

  private:
    std::vector<std::shared_ptr<SimpleDeclaration>> declarations;
    std::vector<std::shared_ptr<Statement>> statements;
};

#endif // BODY_H
