#ifndef HANDLER_H
#define HANDLER_H

#include "../enums/token_type.h"
#include "../structs/token.h"
#include <vector>
#include <string>
#include <utility>

class Handler {
public:
    static std::pair<int, std::vector<Token>> var_handler(const std::string& ptr_file, int i);
    static TokenType determine_tk(const std::string& tk);
    static std::pair<int, std::vector<Token>>while_handler(const std::string& ptr_file, int i); 
};

inline std::pair<int, std::vector<Token>>while_handler(const std::string& ptr_file, int i){
    std::vector<Token> all_tokens;
    while (i < ptr_file.size() && ptr_file[i] == ' ') i++;
    std::string expression = ""; 
    // considering 1st tc
    while (i < ptr_file.size() && ptr_file[i] != ' '){
        expression+= ptr_file[i];
    }
    expression = ""; 
    // insert bool_val 
    all_tokens.push_back({TokenType::tk_identifier, expression}); 

    // get rid off spaces  
    while (i < ptr_file.size() && ptr_file[i] == ' ') i++;
    
    while(i < ptr_file.size() && ptr_file[i] != ' '){
        expression += ptr_file[i];
    }
    // insert loop
    all_tokens.push_back({TokenType::tk_loop, expression});
    expression = ""; 
    // get rid off spaces  
    while (i < ptr_file.size() && ptr_file[i] == ' ') i++;

    while(i < ptr_file.size() && ptr_file[i]!=' '){
        expression += ptr_file[i];
    }
    return {};
}
inline TokenType Handler::determine_tk(const std::string& tk) {
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
#endif // HANDLER_H
