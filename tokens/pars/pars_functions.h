#ifndef HANDLER_H
#define HANDLER_H

#include "../enums/token_type.h"
#include "../structs/token.h"
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

class Handler {
  public:
    static std::pair<int, std::vector<Token>>
    var_handler(const std::string &ptr_file, int i);

    static TokenType determine_tk(const std::string &tk);

    static std::pair<int, std::vector<Token>>
    while_handler(const std::string &ptr_file, int i);

    static std::vector<Token> get_tokens_id_exist(const std::string &tk);
    static std::vector<Token> parse_tokens(const std::string &fileContents);
    static bool check_for_range(const std::string &tk);
};

inline std::vector<Token> Handler::get_tokens_id_exist(const std::string &tk) {
    std::set<char> mySet = {':', '=', '[', ']', '(', ')'};
    std::vector<Token> tokens;

    for (char ch : tk) {
        if (mySet.find(ch) != mySet.end()) {
            TokenType t;
            switch (ch) {
            case ':':
                t = TokenType::tk_colon;
                break;
            case '=':
                t = TokenType::tk_equal;
                break;
            case '[':
                t = TokenType::tk_open_bracket;
                break;
            case ']':
                t = TokenType::tk_close_bracket;
                break;
            case '(':
                t = TokenType::tk_open_parenthesis;
                break;
            case ')':
                t = TokenType::tk_close_parenthesis;
                break;
            default:
                continue;
            }
            tokens.push_back(Token(t, std::string(1, ch)));
        }
    }
    return tokens;
}

inline bool Handler::check_for_range(const std::string &tk) {
    int cnt = 0;
    for (int i = 0; i < tk.size(); i++) {
        if (tk[i] == '.' && cnt == 0) {
            cnt++;
        } else if (tk[i] == '.' && cnt == 1) {
            return true;
        } else
            cnt = 0;
    }
    return false;
}

inline std::vector<Token>
Handler::parse_tokens(const std::string &fileContents) {
    std::vector<Token> output;
    std::string potential;
    int i = 0;

    while (i < fileContents.size()) {
        char currentChar = fileContents[i];

        if (currentChar == ' ' || currentChar == '\n') {
            if (!potential.empty()) {
                auto n = get_tokens_id_exist(potential);
                output.insert(output.end(), n.begin(), n.end());

                auto t = Handler::determine_tk(potential);
                output.push_back(Token(t, potential));
                potential = "";
            }
            i++;
        } else {
            potential += currentChar;
            i++;
        }
    }

    if (!potential.empty()) {
        auto t = Handler::determine_tk(potential);
        output.push_back(Token(t, potential));
    }

    return output;
}

inline TokenType Handler::determine_tk(const std::string &tk) {
    if (tk == "routine")
        return TokenType::tk_routine;
    if (tk == "type")
        return TokenType::tk_type;
    if (tk == "is")
        return TokenType::tk_is;
    if (tk == "var")
        return TokenType::tk_var;
    if (tk == "end")
        return TokenType::tk_end;
    if (tk == "for")
        return TokenType::tk_for;
    if (tk == "loop")
        return TokenType::tk_loop;
    if (tk == "in")
        return TokenType::tk_in;
    if (tk == "while")
        return TokenType::tk_while;
    if (tk == "if")
        return TokenType::tk_if;
    if (tk == "then")
        return TokenType::tk_then;
    if (tk == "else")
        return TokenType::tk_else;
    if (tk == "integer")
        return TokenType::tk_integer;
    if (tk == "boolean")
        return TokenType::tk_boolean;
    if (tk == "real")
        return TokenType::tk_real;
    if (tk == "record")
        return TokenType::tk_record;
    if (tk == "array")
        return TokenType::tk_array;
    if (tk == "true")
        return TokenType::tk_true;
    if (tk == "false")
        return TokenType::tk_false;
    if (tk == "reverse")
        return TokenType::tk_reverse;

    if (tk == "+")
        return TokenType::tk_add;
    if (tk == "-")
        return TokenType::tk_subtract;
    if (tk == "*")
        return TokenType::tk_multiply;
    if (tk == "/")
        return TokenType::tk_divide;
    if (tk == "and")
        return TokenType::tk_and;
    if (tk == "or")
        return TokenType::tk_or;
    if (tk == "xor")
        return TokenType::tk_xor;
    if (tk == "%")
        return TokenType::tk_mod;
    if (tk == "=")
        return TokenType::tk_equal;
    if (tk == "!=")
        return TokenType::tk_not_equal;
    if (tk == "<")
        return TokenType::tk_less_than;
    if (tk == ">")
        return TokenType::tk_greater_than;
    if (tk == "<=")
        return TokenType::tk_less_than_equal;
    if (tk == ">=")
        return TokenType::tk_greater_than_equal;
    if (tk == ":=")
        return TokenType::tk_assign;

    if (tk == "(")
        return TokenType::tk_open_parenthesis;
    if (tk == ")")
        return TokenType::tk_close_parenthesis;
    if (tk == "[")
        return TokenType::tk_open_bracket;
    if (tk == "]")
        return TokenType::tk_close_bracket;
    if (tk == ":")
        return TokenType::tk_colon;
    if (tk == ",")
        return TokenType::tk_comma;
    if (tk == ".")
        return TokenType::tk_dot;
    if (tk == "..")
        return TokenType::tk_range;

    if (tk == "\n")
        return TokenType::tk_newline;
    try {
        stoi(tk);
        return TokenType::tk_num;
    } catch (std::invalid_argument) {
        return TokenType::tk_identifier;
    }

    return TokenType::tk_identifier;
}

#endif // HANDLER_H
