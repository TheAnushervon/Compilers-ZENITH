#ifndef FORLOOP_H
#define FORLOOP_H

#include "node.h"
#include <memory>

class ForLoop : public Node {
public:
    std::shared_ptr<Node> identifier;
    std::shared_ptr<Node> range;
    std::shared_ptr<Node> body;
    bool isReverse; 

    ForLoop(std::shared_ptr<Node> id, std::shared_ptr<Node> rng, std::shared_ptr<Node> bdy, bool reverse = false)
        : identifier(id), range(rng), body(bdy), isReverse(reverse) {}

    void setReverse(bool reverse) {
        isReverse = reverse;
    }
    bool getReverse() const {
        return isReverse;
    }

    std::string ToString(int counter) const override {
        std::string indent(counter, ' ');
        std::string result = indent + "ForLoop: ";

        if (isReverse) {
            result += "(reverse) ";
        }

        result += identifier->ToString(counter) + "\n";
        result += range->ToString(counter) + "\n";
        result += body->ToString(counter);

        return result;
    }
};

#endif // FORLOOP_H
