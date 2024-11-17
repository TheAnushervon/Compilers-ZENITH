#ifndef RELATION_H
#define RELATION_H

#include <vector>
#include <memory>
#include <string> // Добавили include для std::string
#include "node.h"

class Relation : public Node {
public:
    // Simple [ ( < | <= | > | >= | = | /= ) Simple ]
    std::vector<std::shared_ptr<Node>> simples;
    std::vector<std::string> operators; // Добавили вектор операторов

    // Обновленный конструктор, принимающий вектор операторов
    Relation(const std::vector<std::shared_ptr<Node>>& simplesList, const std::vector<std::string>& ops)
        : simples(simplesList), operators(ops) {}

    std::string ToString(int counter) const override {
        std::string ots(counter * 2, ' '); // Отступы с учетом уровня вложенности
        std::string result = ots + "Relation:\n";

        // Итерируемся по `simples` и `operators` одновременно
        for (size_t i = 0; i < simples.size(); ++i) {
            result += simples[i]->ToString(counter + 2);

            // Добавляем оператор, если он есть
            if (i < operators.size()) {
                result += "\n" + std::string((counter + 2) * 2, ' ') + "Operator: " + operators[i] + "\n";
            }
        }

        return result;
    }
};

#endif // RELATION_H
