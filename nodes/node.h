#ifndef NODE_H
#define NODE_H

#include <string>

class Node {
public:
    virtual std::string ToString(int counter) const = 0;
    virtual ~Node() = default;
};

#endif // NODE_H
