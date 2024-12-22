#ifndef RELATION_H
#define RELATION_H

#include <vector>
#include <memory>
#include "node.h"

class Relation : public Node {
public:
    // Simple [ ( < | <= | > | >= | = | /= ) Simple ]
    std::vector<std::shared_ptr<Node>> simples; 

    Relation(const std::vector<std::shared_ptr<Node>>& simplesList) : simples(simplesList) {}

    std::string ToString(int counter) const override {
        std::string ots(counter * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "Relation:\n";

        for (const auto& simple : simples) {
            result += simple->ToString(counter + 2) + " ";
        }

        return result;
    }
};

#endif // RELATION_H
