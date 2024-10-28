#ifndef SIGN_H
#define SIGN_H

#include "node.h"
#include <memory>
#include <string>

class Sign : public Node {
  public:
    //child = + | -
   std::string child;

  Sign(const std::string& childNode) : child(childNode) {}

  Sign(std::string childNode) : child(childNode) {}
    std::string ToString(int counter) const override {
      std::string result = " Sign: " + child +'\n';
      return result; 
    }

};

#endif // SIGN_H
