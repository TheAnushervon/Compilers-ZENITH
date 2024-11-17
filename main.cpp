#include "json.hpp"
#include "syntax_analyzer.cpp"
#include "tokens/enums/token_type.h"
#include "tokens/pars/pars_functions.h"
#include "type_checker.cpp"
#include <cctype>
#include <fstream>
#include <vector>

namespace nh = nlohmann;

std::string toString(TokenType token) {
    switch (token) {
    case TokenType::tk_routine:
        return "tk_routine";
    case TokenType::tk_type:
        return "tk_type";
    case TokenType::tk_is:
        return "tk_is";
    case TokenType::tk_var:
        return "tk_var";
    case TokenType::tk_end:
        return "tk_end";
    case TokenType::tk_for:
        return "tk_for";
    case TokenType::tk_loop:
        return "tk_loop";
    case TokenType::tk_in:
        return "tk_in";
    case TokenType::tk_while:
        return "tk_while";
    case TokenType::tk_if:
        return "tk_if";
    case TokenType::tk_then:
        return "tk_then";
    case TokenType::tk_else:
        return "tk_else";
    case TokenType::tk_integer:
        return "tk_integer";
    case TokenType::tk_boolean:
        return "tk_boolean";
    case TokenType::tk_real:
        return "tk_real";
    case TokenType::tk_record:
        return "tk_record";
    case TokenType::tk_array:
        return "tk_array";
    case TokenType::tk_true:
        return "tk_true";
    case TokenType::tk_false:
        return "tk_false";
    case TokenType::tk_reverse:
        return "tk_reverse";

    case TokenType::tk_add:
        return "tk_add";
    case TokenType::tk_subtract:
        return "tk_subtract";
    case TokenType::tk_multiply:
        return "tk_multiply";
    case TokenType::tk_divide:
        return "tk_divide";
    case TokenType::tk_and:
        return "tk_and";
    case TokenType::tk_or:
        return "tk_or";
    case TokenType::tk_xor:
        return "tk_xor";
    case TokenType::tk_mod:
        return "tk_mod";
    case TokenType::tk_equal:
        return "tk_equal";
    case TokenType::tk_not_equal:
        return "tk_not_equal";
    case TokenType::tk_less_than:
        return "tk_less_than";
    case TokenType::tk_greater_than:
        return "tk_greater_than";
    case TokenType::tk_less_than_equal:
        return "tk_less_than_equal";
    case TokenType::tk_greater_than_equal:
        return "tk_greater_than_equal";
    case TokenType::tk_assign:
        return "tk_assign";

    case TokenType::tk_open_parenthesis:
        return "tk_open_parenthesis";
    case TokenType::tk_close_parenthesis:
        return "tk_close_parenthesis";
    case TokenType::tk_open_bracket:
        return "tk_open_bracket";
    case TokenType::tk_close_bracket:
        return "tk_close_bracket";
    case TokenType::tk_colon:
        return "tk_colon";
    case TokenType::tk_comma:
        return "tk_comma";
    case TokenType::tk_dot:
        return "tk_dot";
    case TokenType::tk_range:
        return "tk_range";
    case TokenType::tk_newline:
        return "tk_newline";
    case TokenType::tk_num:
        return "tk_num";
    case TokenType::tk_identifier:
        return "tk_identifier";
    case TokenType::tk_terminate:
        return "tk_terminate";
        case TokenType::tk_return:
        return "tk_return";
    default:
        return "Unknown token";
    }
};

int main() {

    std::ifstream inputfile("input_for.txt");
    std::string fileContents((std::istreambuf_iterator<char>(inputfile)),
                             std::istreambuf_iterator<char>());
    std::string potential = "";

    auto output = Handler::parse_tokens(fileContents);

    for (int i = 0; i < output.size(); i++) {
        if (output[i].type == TokenType::tk_newline) {
            output.erase(output.begin() + i);
            i--;
        }
    }

    nh::json json_output;

    for (int i = 0; i < output.size(); i++) {
        json_output.push_back(
            {{"type", toString(output[i].type)}, {"value", output[i].value}});
    }

    std::ofstream output_file("output.json");
    output_file << json_output.dump(4);
    output_file.close();

    SyntaxAnalyzer syntaxAnalyzer(output);
    const std::unique_ptr<Node> ast = syntaxAnalyzer.Analyze();
    std::cout << ast->ToString(2) << std::endl;

}
