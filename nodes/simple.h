#ifndef Simple_H
#define Simple_H

#include <vector>
#include <memory>
#include "node.h"

class Simple : public Node {
public:
    // Factor { ( * | / | % ) Factor } 
    std::vector<std::shared_ptr<Node>> factors; 
    
    Simple(const std::vector<std::shared_ptr<Node>>& factorsList) : factors(factorsList) {}

    std::string ToString(int counter) const override {
        std::string result;
        for (const auto& simple : factors) {
            if (!result.empty()) {
                result += " "; 
            }
            result += simple->ToString(counter); 
        }
        return result;
    }
};

#endif // Simple_H
