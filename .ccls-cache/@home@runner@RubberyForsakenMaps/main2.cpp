#include <iostream>
using namespace std;
enum class Operators { tk_ADD, tk_Min };

enum Tokens { Operators, A };

void main2() {
  Tokens mytoken;


  mytoken = Tokens::Operators;
  cout << mytoken << endl;
}
