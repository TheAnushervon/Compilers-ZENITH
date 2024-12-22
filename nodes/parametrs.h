#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "node.h"
#include <string>
#include <memory>
#include <vector>

class Parameters : public Node {
public:
    // ParameterDeclaration { ',' ParameterDeclaration } 
    std::vector<std::shared_ptr<Node>> children;

    explicit Parameters(const std::vector<std::shared_ptr<Node>>& initialChildren = {})
        : children(initialChildren) {}

    void AddParameter(const std::shared_ptr<Node>& parameter) {
        children.push_back(parameter);
    }

    std::string ToString(int counter) const override {
        if (children.empty()) {
            return std::string(counter * 2, ' ') + "Parameters: none";
        }

        std::string ots(counter * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "Parameters:\n";

        for (const auto& child : children) {
            result += child->ToString(counter + 2);
        }

        // Удаление последней запятой и пробела
        if (!children.empty()) {
            result = result.substr(0, result.size() - 2) + "\n";
        }

        return result;
    }
};

#endif // PARAMETERS_H
