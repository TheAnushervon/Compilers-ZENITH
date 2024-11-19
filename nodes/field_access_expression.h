#ifndef FIELD_ACCESS_EXPRESSION_H
#define FIELD_ACCESS_EXPRESSION_H

#include "node.h"
#include "identifier.h"
#include <memory>

class FieldAccessExpression : public Node {
public:
    std::shared_ptr<Node> object;
    std::shared_ptr<Node> field;

    FieldAccessExpression(std::shared_ptr<Node> obj, std::shared_ptr<Node> fld)
        : object(obj), field(fld) {}

    std::string ToString(int count) const override {
        std::string ots(count * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "FieldAccessExpression:\n";
        result += object->ToString(count + 2); // Выводим объект
        result += field->ToString(count + 2);  // Выводим поле
        return result;
    }
};

#endif // FIELD_ACCESS_EXPRESSION_H
