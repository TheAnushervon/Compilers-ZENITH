#ifndef BODY_H
#define BODY_H

#include "node.h"
#include <memory>
#include <vector>

class Body : public Node {
public:
    std::vector<std::shared_ptr<Node>> statements;
    std::shared_ptr<Node> returnType;
    int returnTypeIndex;

    Body(const std::vector<std::shared_ptr<Node>>& statements)
        : statements(statements), returnType(nullptr), returnTypeIndex(-1) {}

    std::string ToString(int counter) const override {
        std::string ots(counter * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "Body:\n";

        for (const auto& stmt : statements) {
            if (stmt->ToString(counter + 2) =="") result += "";
            else result += stmt->ToString(counter + 2) + "\n";
        }
        if (result == "Body:\n") return "";
        return result;
    }
};

#endif // BODY_H
