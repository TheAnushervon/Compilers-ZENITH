#ifndef RECORD_EXPRESSION_H
#define RECORD_EXPRESSION_H

#include <string>
#include <memory>
#include <vector>
#include "node.h"
#include "field_assignment.h"

class RecordExpression : public Node {
public:
    std::vector<std::shared_ptr<Node>> fieldAssignments;

    RecordExpression(const std::vector<std::shared_ptr<Node>>& assignments)
        : fieldAssignments(assignments) {}

    std::string ToString(int count) const override {
        std::string ots(count * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "RecordExpression:\n";
        
        // Итерируемся по каждому полю и выводим информацию
        for (const auto& field : fieldAssignments) {
            result += field->ToString(count + 2);  // Вызываем ToString для каждого поля
        }

        return result;
    }
};

#endif // RECORD_EXPRESSION_H
