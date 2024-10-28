#ifndef SIMPLE_H
#define SIMPLE_H

#include <vector>
#include <memory>
#include "node.h"

class Simple : public Node {
public:
    // Factor { ( * | / | % ) Factor } 
    std::vector<std::shared_ptr<Node>> factors; 

    Simple(const std::vector<std::shared_ptr<Node>>& factorsList) : factors(factorsList) {}

    std::string ToString(int counter) const override {
        std::string ots(counter * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "Simple:\n";

        for (const auto& factor : factors) {
            result += factor->ToString(counter + 1) + "\n";
        }

        return result;
    }
};

#endif // SIMPLE_H
