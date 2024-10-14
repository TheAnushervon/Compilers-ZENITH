#ifndef PRIMITIVETYPE_H
#define PRIMITIVETYPE_H

#include "type.h"

class PrimitiveType : public Type {
  public:
    enum class Kind { Integer, Real, Boolean };

    explicit PrimitiveType(Kind kind) : kind(kind) {}

    std::string ToString() const override {
        switch (kind) {
        case Kind::Integer:
            return "integer";
        case Kind::Real:
            return "real";
        case Kind::Boolean:
            return "boolean";
        default:
            return "unknown";
        }
    }

  private:
    Kind kind;
};

inline PrimitiveType::Kind StringToKind(const std::string &str) {
    if (str == "integer") {
        return PrimitiveType::Kind::Integer;
    } else if (str == "real") {
        return PrimitiveType::Kind::Real;
    } else if (str == "boolean") {
        return PrimitiveType::Kind::Boolean;
    } else {
        throw std::invalid_argument("Unknown primitive type: " + str);
    }
}

#endif // PRIMITIVETYPE_H
