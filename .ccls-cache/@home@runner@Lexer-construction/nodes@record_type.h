#ifndef RECORDTYPE_H
#define RECORDTYPE_H

#include <vector>
#include <memory>
#include "type.h"
#include "variable_declaration.h"

class RecordType : public Type {
public:
    explicit RecordType(const std::vector<std::shared_ptr<VariableDeclaration>>& members)
        : members(members) {}

    std::string ToString(int counter) const override {
        std::string result = "record { ";
        for (const auto& member : members) {
            result += member->ToString(2) + "; ";
        }
        result += "} end";
        return result;
    }

private:
    std::vector<std::shared_ptr<VariableDeclaration>> members; // Члены записи
};

#endif // RECORDTYPE_H
