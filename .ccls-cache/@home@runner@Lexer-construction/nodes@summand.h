#ifndef SUMMAND_H
#define SUMMAND_H

#include <vector>
#include <memory>
#include "node.h"

class Summand : public Node {
public:
    // Primary | ( Expression ) 
    std::shared_ptr<Node> child; 

    Summand(std::shared_ptr<Node> childNode) : child(childNode) {}

    std::string ToString(int counter) const override {
        std::string ots(counter * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "Summand:\n" + child->ToString(counter + 1);
        return result;
    }
};

#endif // SUMMAND_H
