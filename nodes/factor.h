#ifndef FACTOR_H
#define FACTOR_H

#include <vector>
#include <memory>
#include "node.h"

class Factor : public Node {
public:
Factor(const std::vector<std::shared_ptr<Node>>& summandsList) : summands(summandsList) {}

    std::string ToString(int counter) const override {
        std::string result;
        for (const auto& simple : summands) {
            if (!result.empty()) {
                result += " "; 
            }
            result += simple->ToString(counter); 
        }
        return result;
    }

private:
// Summand { ( + | - ) Summand } 
    std::vector<std::shared_ptr<Node>> summands; 
};

#endif // FACTOR_H
