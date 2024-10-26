#ifndef Summand_H
#define Summand_H

#include <vector>
#include <memory>
#include "node.h"

class Summand : public Node {
public:
Summand(std::shared_ptr<Node> childNode) : child(childNode) {}


    std::string ToString(int counter) const override {
        std::string result = "Summand: " + child->ToString(counter);
        return result;
    }

private:
// Primary | ( Expression ) 
    std::shared_ptr<Node> child; 
};

#endif // Summand_H


