#ifndef BODY_H
#define BODY_H

#include "node.h"
#include <memory>
#include <vector>

class Body : public Node {
public:
    std::vector<std::shared_ptr<Node>> statements;
    std::shared_ptr<Node> returnType;

    Body(const std::vector<std::shared_ptr<Node>>& statements)
        : statements(statements), returnType(nullptr) {}

    std::string ToString(int counter) const override {
        std::string ots(counter * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "Body:\n";

        for (const auto& stmt : statements) {
            result += stmt->ToString(counter + 2) + "\n";
        }

        return result;
    }
};

#endif // BODY_H
