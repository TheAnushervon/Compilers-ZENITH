#ifndef BODY_H
#define BODY_H

#include "node.h"
#include <memory>
#include <vector>

class Body : public Node {
public:
    std::vector<std::shared_ptr<Node>> statements;

    Body(const std::vector<std::shared_ptr<Node>>& statements)
        : statements(statements) {}

    std::string ToString(int counter) const override {
        std::string result = "Body: ";
        for (const auto& stmt : statements) {
            result += stmt->ToString(counter) + "\n";
        }
        return result;
    }
};


#endif // BODY_H
