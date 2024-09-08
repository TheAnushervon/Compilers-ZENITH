#include <cctype>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>

enum class TokenType {
   // Keywords
   tk_routine,
   tk_type,
   tk_is,
   tk_var,
   tk_end,
   tk_for,
   tk_loop,
   tk_in,
   tk_while,
   tk_if,
   tk_then,
   tk_else,
   tk_integer,
   tk_boolean,
   tk_real,
   tk_record,
   tk_array,
   tk_true,
   tk_false,
   tk_reverse,

   // Operators
   tk_add,
   tk_subtract,
   tk_multiply,
   tk_divide,
   tk_and,
   tk_or,
   tk_xor,
   tk_mod,
   tk_equal,
   tk_not_equal,
   tk_less_than,
   tk_greater_than,
   tk_less_than_equal,
   tk_greater_than_equal,
   tk_assign,

   // Symbols
   tk_open_parenthesis,
   tk_close_parenthesis,
   tk_open_bracket,
   tk_close_bracket,
   tk_colon,
   tk_comma,
   tk_dot,
   tk_range, // two dots, case: for 2..5

   tk_newline,
   tk_identifier,
};
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

struct Token {
   TokenType type;
   std::string value;
 Token(TokenType t, const std::string& v) : type(t), value(v) {}
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

enum TokenType determine_tk(std::string &tk) {
       if (tk == "routine") return TokenType::tk_routine;
       if (tk == "type") return TokenType::tk_type;
       if (tk == "is") return TokenType::tk_is;
       if (tk == "var") return TokenType::tk_var;
       if (tk == "end") return TokenType::tk_end;
       if (tk == "for") return TokenType::tk_for;
       if (tk == "loop") return TokenType::tk_loop;
       if (tk == "in") return TokenType::tk_in;
       if (tk == "while") return TokenType::tk_while;
       if (tk == "if") return TokenType::tk_if;
       if (tk == "then") return TokenType::tk_then;
       if (tk == "else") return TokenType::tk_else;
       if (tk == "integer") return TokenType::tk_integer;
       if (tk == "boolean") return TokenType::tk_boolean;
       if (tk == "real") return TokenType::tk_real;
       if (tk == "record") return TokenType::tk_record;
       if (tk == "array") return TokenType::tk_array;
       if (tk == "true") return TokenType::tk_true;
       if (tk == "false") return TokenType::tk_false;
       if (tk == "reverse") return TokenType::tk_reverse;

       if (tk == "+") return TokenType::tk_add;
       if (tk == "-") return TokenType::tk_subtract;
       if (tk == "*") return TokenType::tk_multiply;
       if (tk == "/") return TokenType::tk_divide;
       if (tk == "and") return TokenType::tk_and;
       if (tk == "or") return TokenType::tk_or;
       if (tk == "xor") return TokenType::tk_xor;
       if (tk == "%") return TokenType::tk_mod;
       if (tk == "==") return TokenType::tk_equal;
       if (tk == "!=") return TokenType::tk_not_equal;
       if (tk == "<") return TokenType::tk_less_than;
       if (tk == ">") return TokenType::tk_greater_than;
       if (tk == "<=") return TokenType::tk_less_than_equal;
       if (tk == ">=") return TokenType::tk_greater_than_equal;
       if (tk == "=") return TokenType::tk_assign;

       if (tk == "(") return TokenType::tk_open_parenthesis;
       if (tk == ")") return TokenType::tk_close_parenthesis;
       if (tk == "[") return TokenType::tk_open_bracket;
       if (tk == "]") return TokenType::tk_close_bracket;
       if (tk == ":") return TokenType::tk_colon;
       if (tk == ",") return TokenType::tk_comma;
       if (tk == ".") return TokenType::tk_dot;
       if (tk == "..") return TokenType::tk_range;

       if (tk == "\n") return TokenType::tk_newline;
       if (tk == "identifier") return TokenType::tk_identifier;

       // Default case if none of the above match
       return TokenType::tk_identifier;
}
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
            auto t =  determine_tk(potential);
            std::pair<int,std::vector<TokenType>> result;
             switch (t){
                case TokenType::tk_var: 
                     result = 

               case TokenType::tk_while: 
                  pair(int, vector<TokenType>) result = while_handler(fileContents, i);
                  i = result.first ; 
                  // output.insert(output.end(), result.second.begin(), result.second.end());
                for (int i = 0 ; i < result.second.size(); i++){
                   output.emplace_back(result.second[i], toString(result.second[i]));
                }
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
}
