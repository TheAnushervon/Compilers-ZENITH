#include <cctype>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>
#include "tokens/enums/token_type.h"
#include "tokens/pars/pars_functions.h"
#include "tokens/structs/token.h"


std::string toString(TokenType token) {
    switch (token) {
        case TokenType::tk_routine: return "tk_routine";
        case TokenType::tk_type: return "tk_type";
        case TokenType::tk_is: return "tk_is";
        case TokenType::tk_var: return "tk_var";
        case TokenType::tk_end: return "tk_end";
        case TokenType::tk_for: return "tk_for";
        case TokenType::tk_loop: return "tk_loop";
        case TokenType::tk_in: return "tk_in";
        case TokenType::tk_while: return "tk_while";
        case TokenType::tk_if: return "tk_if";
        case TokenType::tk_then: return "tk_then";
        case TokenType::tk_else: return "tk_else";
        case TokenType::tk_integer: return "tk_integer";
        case TokenType::tk_boolean: return "tk_boolean";
        case TokenType::tk_real: return "tk_real";
        case TokenType::tk_record: return "tk_record";
        case TokenType::tk_array: return "tk_array";
        case TokenType::tk_true: return "tk_true";
        case TokenType::tk_false: return "tk_false";
        case TokenType::tk_reverse: return "tk_reverse";
       
        case TokenType::tk_add: return "tk_add";
        case TokenType::tk_subtract: return "tk_subtract";
        case TokenType::tk_multiply: return "tk_multiply";
        case TokenType::tk_divide: return "tk_divide";
        case TokenType::tk_and: return "tk_and";
        case TokenType::tk_or: return "tk_or";
        case TokenType::tk_xor: return "tk_xor";
        case TokenType::tk_mod: return "tk_mod";
        case TokenType::tk_equal: return "tk_equal";
        case TokenType::tk_not_equal: return "tk_not_equal";
        case TokenType::tk_less_than: return "tk_less_than";
        case TokenType::tk_greater_than: return "tk_greater_than";
        case TokenType::tk_less_than_equal: return "tk_less_than_equal";
        case TokenType::tk_greater_than_equal: return "tk_greater_than_equal";
        case TokenType::tk_assign: return "tk_assign";
       
        case TokenType::tk_open_parenthesis: return "tk_open_parenthesis";
        case TokenType::tk_close_parenthesis: return "tk_close_parenthesis";
        case TokenType::tk_open_bracket: return "tk_open_bracket";
        case TokenType::tk_close_bracket: return "tk_close_bracket";
        case TokenType::tk_colon: return "tk_colon";
        case TokenType::tk_comma: return "tk_comma";
        case TokenType::tk_dot: return "tk_dot";
        case TokenType::tk_range: return "tk_range";
        case TokenType::tk_newline: return "tk_newline";
        case TokenType::tk_identifier: return "tk_identifier";
        default: return "Unknown token";
    }
};


bool is_simple_sign(char t) {
   // removed t == '/', cause we have /=
   // removed  t == '=', cause we have :=
   // could be solved using stack when meet them
   if (t == '+' || t == '-' || t == '*') {
      return true;
   }
   return false;
}
bool is_factor(char t) {
   // removed t == '/', cause we have /=
   if (t == '*' || t == '%') {
      return true;
   }
   return false;
}

// var x : integer
// trigers : ' ', ':'

// output.push_back(Token{determine_tk(string potential), potential});
// std::pair<int,vector<TokenType>> var_hadler(&ptr_file, i){
//   auto name ="";
// while(ptr_file[i] != ' '){
// name += ptr_file[i];
//i++;
//}
// i+=3
// auto type ="";
//  while(ptr_file[i] != ' '){
// type += ptr_file[i];
//i++;
//}
void Insert_to_output(std::vector<Token> &output, std::vector<Token>& result){
   for (int i = 0 ; i < result.size(); i++){
       output.emplace_back(result[i].type, result[i].value);
    }
}
int main() {
   std::ifstream inputfile("input.txt");
   std::string fileContents((std::istreambuf_iterator<char>(inputfile)),
                            std::istreambuf_iterator<char>());
   std::vector<Token> output;
   output.emplace_back(TokenType::tk_if, "if");
   std::cout << output[0].value;
   std::cout << toString(output[0].type); 
   // auto index = 0;
   

   // int space_counter = 0;
   std::cout << std::endl<< fileContents << std::endl;
   std::string potential = "";
   int i = 0 ; 
      while(i < fileContents.size()){
          if (fileContents[i] == ' '){
            auto t =  Handler::determine_tk(potential);
            std::pair<int,std::vector<Token>> result;
             switch (t){
                case TokenType::tk_var: 
                     result = Handler::var_handler(fileContents, i);
                     Insert_to_output(output, result.second);
                     break;
                default:
                break;
      

              // case TokenType::tk_while: 
                //  pair(int, vector<Token>) result = while_handler(fileContents, i);
                  //i = result.first ; 
                  // output.insert(output.end(), result.second.begin(), result.second.end());
               // Insert_to_output(output, result.second); 
                   
             }
             i = result.first;

          }else{
             potential += fileContents[i];
             i++;
          }  
        
      
         // if (isalpha(ch)) {
      //    potential += ch;
      // } else if (ch == ' ') {
      //    space_counter++;
      //    //determine_tk(potential);
      // }
      // // check for \n
      // if (space_counter == 2) {
      //    output.emplace_back(TokenType::tk_if, "if");
      // }
      // if (ch == ' ') {
      //    space_counter++;
      // }
   
   }
   for (auto & i : output){
      std::cout << i.value;
      std::cout << toString(i.type); 
   }
}
