#ifndef PARS_FUNCTIONS_H
#define PARS_FUNCTIONS_H

#include "tokens/enums/token_type.h"
#include "tokens/structs/token.h"
#include <vector>

class Handler{ 
//Handler::var_handler()
static std::pair<int, std::vector<Token>> var_handler(const std::string& ptr_file, int i) {
 std::vector<Token> arr;
 std::string name = "";


 while (i < ptr_file.size() && ptr_file[i] != ' ') {
  name += ptr_file[i];
  i++;
 }

 auto tk_name = determine_tk(name);
 arr.push_back(tk_name);
 while (i < ptr_file.size() && ptr_file[i] == ' ') i++;

 std::string next = "";
 while (i < ptr_file.size() && ptr_file[i] != ' ') {
  next += ptr_file[i];
  i++;
 }

 auto next_tk = determine_tk(next);
 arr.push_back(next_tk);
 while (i < ptr_file.size() && ptr_file[i] == ' ') i++;

 std::string type = "";
 while (i < ptr_file.size() && ptr_file[i] != ' ') {
  type += ptr_file[i];
  i++;
 }

 auto tk_type = determine_tk(type);
 arr.push_back(tk_type);
 while (i < ptr_file.size() && ptr_file[i] == ' ') i++;

 std::string next_symbol = "";
 auto index = i;
 while (i < ptr_file.size() && ptr_file[i] != ' ') {
  next_symbol += ptr_file[i];
  i++;
 }

 if (next_symbol != "is") {
  return {index, arr};
 } else {  
  arr.push_back(determine_tk(next_symbol));
  while (i < ptr_file.size() && ptr_file[i] == ' ') i++;

  std::string value = "";
  while (i < ptr_file.size() && ptr_file[i] != ' ') {
   value += ptr_file[i];
   i++;
  }
  arr.push_back(determine_tk(value));
  return {i, arr};
 }
};
#endif //PARS_FUNCTIONS_H