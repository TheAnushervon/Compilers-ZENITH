#ifndef FACTOR_H
#define FACTOR_H

#include <vector>
#include <memory>
#include "node.h"

class Factor : public Node {
public:
    // Summand { ( + | - ) Summand } 
    std::vector<std::shared_ptr<Node>> summands; 

    Factor(const std::vector<std::shared_ptr<Node>>& summandsList) : summands(summandsList) {}

    std::string ToString(int counter) const override {
        std::string ots(counter * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "Factor:\n";

        for (const auto& simple : summands) {
            result += simple->ToString(counter + 2) + "\n"; // Перенос строки для каждого summand
        }

        return result;
    }
};

#endif // FACTOR_H
