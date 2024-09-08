// enums/token_type.h
#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

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
#endif // TOKEN_TYPE_H
