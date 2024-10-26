#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "node.h"
#include <string>
#include <memory>
#include <vector>

class Parameters : public Node {
public:
    explicit Parameters(const std::vector<std::shared_ptr<Node>>& initialChildren = {})
        : children(initialChildren) {}
    
    void AddParameter(const std::shared_ptr<Node>& parameter) {
        children.push_back(parameter);
    }

    std::string ToString(int counter) const override {
        if (children.empty()) {
            return "Parameters: none";
        }

        std::string result = "Parameters: ";
        for (const auto& child : children) {
            result += child->ToString(counter) + ", ";
        }

        if (!result.empty()) {
            result = result.substr(0, result.size() - 2);
        }

        return result;
    }

private:
// ParameterDeclaration { ',' ParameterDeclaration } 
    std::vector<std::shared_ptr<Node>> children;
};

#endif // PARAMETERS_H
