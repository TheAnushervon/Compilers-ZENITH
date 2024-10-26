#ifndef Relation_H
#define Relation_H

#include <vector>
#include <memory>
#include "node.h"

class Relation : public Node {
public:
     Relation(const std::vector<std::shared_ptr<Node>>& simplesList) : simples(simplesList) {}

    std::string ToString(int counter) const override {
        std::string result;
        for (const auto& simple : simples) {
            if (!result.empty()) {
                result += " "; 
            }
            result += simple->ToString(counter); 
        }
        return result;
    }

private:
//Simple [ ( < | <= | > | >= | = | /= ) Simple ]
    std::vector<std::shared_ptr<Node>> simples; 
};

#endif // Relation_H
