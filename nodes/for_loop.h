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
        std::string ots(counter * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "ForLoop:\n";

        if (isReverse) {
            result += ots + "  (reverse)\n";
        }

        result += ots + "  Identifier:\n" + identifier->ToString(counter + 2) + "\n";
        result += ots + "  Range:\n" + range->ToString(counter + 2) + "\n";
        result += ots + "  Body:\n" + body->ToString(counter + 2);

        return result;
    }
};

#endif // FORLOOP_H
