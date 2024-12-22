#ifndef FIELD_ASSIGNMENT_H
#define FIELD_ASSIGNMENT_H

#include <string>
#include <memory>
#include "node.h"


class FieldAssignment : public Node {
public:
    std::shared_ptr<Node> fieldName;  // Имя поля записи
    std::shared_ptr<Node> value;      // Выражение для инициализации поля

    FieldAssignment(std::shared_ptr<Node> name, std::shared_ptr<Node> expr)
        : fieldName(name), value(expr) {}

    std::string ToString(int count) const override {
        std::string ots(count * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "FieldAssignment:\n";
        result += fieldName->ToString(count + 2);
        result += value->ToString(count + 2); // Для выражения, которое инициализирует поле
        return result;
    }
};

#endif // FIELD_ASSIGNMENT_H
