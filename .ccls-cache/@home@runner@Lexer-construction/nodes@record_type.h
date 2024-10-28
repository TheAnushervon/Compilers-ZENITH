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
        std::string ots(counter * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "RecordType:\n";

        for (const auto& member : members) {
            result += member->ToString(counter + 2) + "\n";
        }

        return result;
    }
};

#endif // RECORDTYPE_H
