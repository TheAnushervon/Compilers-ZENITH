#ifndef Primary_H
#define Primary_H

#include <vector>
#include <memory>
#include "node.h"

class Primary : public Node {
public:
     Primary(std::shared_ptr<Node> childNode) : child(childNode) {}

    std::string ToString(int counter) const override {
        std::string result = "Primary: " + child->ToString(counter);
        return result;
    }

private:
// : IntegralLiteral 
//| RealLiteral 
//| true | false 
//| ModifiablePrimary 
    std::shared_ptr<Node> child; 
};

#endif // Primary_H
