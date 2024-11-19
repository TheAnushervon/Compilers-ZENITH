#ifndef LITERAL_PRIMARY_H
#define LITERAL_PRIMARY_H

#include "node.h"
#include <string>

class LiteralPrimary : public Node {
public:
    enum class LiteralType { Integer, Real, Boolean };

    LiteralType type;
    int intValue;
    double realValue;
    bool boolValue;

    LiteralPrimary(int intValue) 
        : type(LiteralType::Integer), intValue(intValue), realValue(0), boolValue(false) {}

    LiteralPrimary(double realValue) 
        : type(LiteralType::Real), intValue(0), realValue(realValue), boolValue(false) {}

    LiteralPrimary(bool boolValue) 
        : type(LiteralType::Boolean), intValue(0), realValue(0), boolValue(boolValue) {}

    std::string ToString(int counter) const override {
        std::string ots(counter * 2, ' '); // Отступы с учетом уровня вложенности
        std::string ots1(counter * 2+4, ' '); // Отступы с учетом уровня вложенности


        switch (type) {
            case LiteralType::Integer:
                return ots + "LiteralPrimary\n" + ots1 + "Integer: " + std::to_string(intValue)+ "\n";
            case LiteralType::Real:
                return ots + "LiteralPrimary\n"+ ots1 + "Real: " + std::to_string(realValue)+ "\n";
            case LiteralType::Boolean:
                return ots + "LiteralPrimary\n" + ots1 +"Boolean: " + (boolValue ? "true" : "false") + "\n";
        }
        return ""; 
    }
};

#endif // LITERAL_PRIMARY_H
