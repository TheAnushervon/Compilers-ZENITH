#ifndef RECORDTYPE_H
#define RECORDTYPE_H

#include <vector>
#include <memory>
#include "node.h"

class RecordType : public Node {
public:
    std::vector<std::shared_ptr<Node>> members;

    RecordType(const std::vector<std::shared_ptr<Node>>& members) 
        : members(members) {}

    std::string ToString(int counter) const override {
        std::string result = "RecordType: ";
        for (const auto& member : members) {
            result += member->ToString(counter) + "\n";
        }
        return result;
    }
};


#endif // RECORDTYPE_H
