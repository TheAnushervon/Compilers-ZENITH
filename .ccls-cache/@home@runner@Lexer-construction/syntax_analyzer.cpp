#include "nodes/array_type.h"
#include "nodes/assignment.h"
#include "nodes/body_declaration.h"
#include "nodes/expression.h"
#include "nodes/for_loop.h"
#include "nodes/identifier.h"
#include "nodes/if_statement.h"
#include "nodes/literal_expression.h"
#include "nodes/parameter_declaration.h"
#include "nodes/primitive_type.h"
#include "nodes/program.h"
#include "nodes/range_statment.h"
#include "nodes/record_type.h"
#include "nodes/routine_call.h"
#include "nodes/routine_declaration.h"
#include "nodes/statement.h"
#include "nodes/type_declaration.h"
#include "nodes/variable_declaration.h"
#include "nodes/while_loop.h"
#include "tokens/enums/token_type.h"
#include "tokens/structs/token.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class SyntaxAnalyzer {
  public:
    SyntaxAnalyzer(const std::vector<Token> &tokens)
        : tokens(tokens), currentIndex(0), errorOccurred(false) {}

    std::unique_ptr<Program> Analyze() {
        auto program = std::make_unique<Program>();
        bool flag_terminated = false;
        while (currentIndex < tokens.size() && !errorOccurred) {
            if (flag_terminated)
                break;
            const Token &token = GetCurrentToken();
            switch (token.type) {
            case TokenType::tk_routine:
                program->AddRoutineDeclaration(ParseRoutine());
                break;
            case TokenType::tk_var:
            case TokenType::tk_type:
                program->AddSimpleDeclaration(ParseSimpleDeclaration());
                break;
            case TokenType::tk_terminate:
                flag_terminated = true;
                break;
            default:
                std::cerr << "Syntax error: Unexpected token '" << token.value
                          << "'\n";
                errorOccurred = true;
                break;
            }
        }
        return program;
    }

    std::string toStrin(TokenType token) {
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
        case TokenType::tk_identifier:
            return "tk_identifier";
        default:
            return "Unknown token";
        }
    };

  private:
    const std::vector<Token> &tokens;
    size_t currentIndex;
    bool errorOccurred;

    const Token &GetCurrentToken() const { return tokens[currentIndex]; }

    const Token &AdvanceToken() {
        if (currentIndex < tokens.size() - 1) {
            currentIndex++;
        }
        return tokens[currentIndex];
    }

    bool ExpectToken(TokenType expectedType) {
        if (GetCurrentToken().type == expectedType) {
            // AdvanceToken();
            return true;
        } else {
            std::cerr << "Syntax error: Expected token type "
                      << static_cast<int>(expectedType) << ", but got "
                      << static_cast<int>(GetCurrentToken().type) << std::endl;
            errorOccurred = true;
            return false;
        }
    }

    std::shared_ptr<RoutineDeclaration> ParseRoutine() {
        if (errorOccurred)
            return nullptr;

        AdvanceToken(); // Пропускаем 'routine'

        if (!ExpectToken(TokenType::tk_identifier)) {
            return nullptr;
        }
        auto routineName =
            std::make_shared<Identifier>(GetCurrentToken().value);
        AdvanceToken();

        std::vector<std::shared_ptr<ParameterDeclaration>> parameters;
        if (ExpectToken(TokenType::tk_open_parenthesis)) {
            AdvanceToken();
            parameters = ParseParameters();
            ExpectToken(TokenType::tk_close_parenthesis);
            AdvanceToken();
        }

        std::shared_ptr<Type> returnType = nullptr;
        if (GetCurrentToken().type == TokenType::tk_colon) {
            AdvanceToken();
            returnType = ParseType();
        }

        std::shared_ptr<Body> body = nullptr;
        if (ExpectToken(TokenType::tk_is)) {
            AdvanceToken();
            body = ParseBody();
        }

        ExpectToken(TokenType::tk_end);
        AdvanceToken();
        return std::make_shared<RoutineDeclaration>(routineName, parameters,
                                                    returnType, body);
    }

    std::shared_ptr<SimpleDeclaration> ParseSimpleDeclaration() {
        if (errorOccurred)
            return nullptr;

        if (GetCurrentToken().type == TokenType::tk_var) {
            return ParseVariableDeclaration();
        } else if (GetCurrentToken().type == TokenType::tk_type) {
            return ParseTypeDeclaration();
        }
        return nullptr;
    }

    std::shared_ptr<VariableDeclaration> ParseVariableDeclaration() {
        if (errorOccurred)
            return nullptr;

        AdvanceToken(); // Пропускаем 'var'

        if (!ExpectToken(TokenType::tk_identifier)) {
            return nullptr;
        }
        auto varName = std::make_shared<Identifier>(GetCurrentToken().value);
        AdvanceToken();

        std::shared_ptr<Type> varType = nullptr;
        if (ExpectToken(TokenType::tk_colon)) {
            AdvanceToken();
            varType = ParseType();
        }

        std::shared_ptr<Expression> initialValue = nullptr;
        if (GetCurrentToken().type == TokenType::tk_is) {
            AdvanceToken();

            initialValue = ParseExpression();
        }

        return std::make_shared<VariableDeclaration>(varName, varType,
                                                     initialValue);
    }

    std::shared_ptr<TypeDeclaration> ParseTypeDeclaration() {
        if (errorOccurred)
            return nullptr;

        AdvanceToken(); // Пропускаем 'type'

        if (!ExpectToken(TokenType::tk_identifier)) {
            return nullptr;
        }
        auto typeName = std::make_shared<Identifier>(GetCurrentToken().value);
        AdvanceToken();

        auto type = ParseType();
        return std::make_shared<TypeDeclaration>(typeName, type);
    }

    std::shared_ptr<IfStatement> ParseIfStatement() {
        if (errorOccurred)
            return nullptr;

        AdvanceToken(); // Пропускаем 'if'

        auto condition = ParseExpression();
        std::shared_ptr<Body> thenBody = nullptr, elseBody = nullptr;

        if (ExpectToken(TokenType::tk_then)) {
            thenBody = ParseBody();
        }

        if (GetCurrentToken().type == TokenType::tk_else) {
            AdvanceToken();
            elseBody = ParseBody();
        }

        ExpectToken(TokenType::tk_end);
        return std::make_shared<IfStatement>(condition, thenBody, elseBody);
    }

    std::shared_ptr<ForLoop> ParseForLoop() {
        if (errorOccurred)
            return nullptr;

        AdvanceToken(); // Пропускаем 'for'
        std::cout << toStrin(GetCurrentToken().type) << " ";

        if (!ExpectToken(TokenType::tk_identifier)) {
            return nullptr;
        }
        std::cout << toStrin(GetCurrentToken().type) << " ";
        std::cout << GetCurrentToken().value << " ";
        auto loopVar = std::make_shared<Identifier>(GetCurrentToken().value);
        AdvanceToken();

        auto range = ParseRange();
        if (!range) {
            return nullptr;
        }

        auto startExpr = range->GetStart();
        auto endExpr = range->GetEnd();
        auto body = ParseBody();

        ExpectToken(TokenType::tk_end);
        return std::make_shared<ForLoop>(loopVar, startExpr, endExpr, body);
    }

    std::shared_ptr<WhileLoop> ParseWhileLoop() {
        if (errorOccurred)
            return nullptr;

        AdvanceToken(); // Пропускаем 'while'

        auto condition = ParseExpression();
        auto body = ParseBody();

        ExpectToken(TokenType::tk_end);
        return std::make_shared<WhileLoop>(condition, body);
    }

    std::vector<std::shared_ptr<ParameterDeclaration>> ParseParameters() {
        if (errorOccurred)
            return {};

        std::vector<std::shared_ptr<ParameterDeclaration>> parameters;

        while (GetCurrentToken().type != TokenType::tk_close_parenthesis) {
            if (!ExpectToken(TokenType::tk_identifier)) {
                break;
            }
            auto paramName =
                std::make_shared<Identifier>(GetCurrentToken().value);
            AdvanceToken();

            if (!ExpectToken(TokenType::tk_colon)) {
                break;
            }
            AdvanceToken();
            auto paramType = ParseType();
            parameters.push_back(
                std::make_shared<ParameterDeclaration>(paramName, paramType));

            if (GetCurrentToken().type == TokenType::tk_comma) {
                AdvanceToken();
            } else {
                break;
            }
        }
        return parameters;
    }

    std::shared_ptr<Type> ParseType() {
        if (errorOccurred)
            return nullptr;

        std::string typeStr = GetCurrentToken().value;
        PrimitiveType::Kind kind = StringToKind(typeStr);
        auto typeNode = std::make_shared<PrimitiveType>(kind);
        AdvanceToken();
        return typeNode;
    }

    std::shared_ptr<Expression> ParseExpression() {
        if (errorOccurred)
            return nullptr;

        auto exprNode =
            std::make_shared<LiteralExpression>(GetCurrentToken().value);
        AdvanceToken();
        AdvanceToken();
        return exprNode;
    }

    std::shared_ptr<Range> ParseRange() {
        if (errorOccurred)
            return nullptr;

        auto startExpr = ParseExpression();

        if (GetCurrentToken().type == TokenType::tk_range) {
            AdvanceToken();
            auto endExpr = ParseExpression();
            return std::make_shared<Range>(startExpr, endExpr);
        }

        return nullptr;
    }

    std::shared_ptr<Body> ParseBody() {
        if (errorOccurred)
            return nullptr;

        std::vector<std::shared_ptr<Statement>> statements;
        std::vector<std::shared_ptr<SimpleDeclaration>> declarations;

        while (GetCurrentToken().type != TokenType::tk_end &&
               currentIndex < tokens.size()) {
            switch (GetCurrentToken().type) {
            case TokenType::tk_var:
                statements.push_back(ParseVariableDeclaration());
                break;
            case TokenType::tk_if:
                statements.push_back(ParseIfStatement());
                break;
            case TokenType::tk_for:
                statements.push_back(ParseForLoop());
                break;
            case TokenType::tk_while:
                statements.push_back(ParseWhileLoop());
                break;
            case TokenType::tk_identifier:
                statements.push_back(ParseRoutineCall());
                break;
            default:
                AdvanceToken();
                break;
            }
        }
        return std::make_shared<Body>(declarations, statements);
    }

    std::shared_ptr<RoutineCall> ParseRoutineCall() {
        if (errorOccurred)
            return nullptr;

        auto routineName =
            std::make_shared<Identifier>(GetCurrentToken().value);
        AdvanceToken();

        std::vector<std::shared_ptr<Expression>> arguments;
        if (ExpectToken(TokenType::tk_open_parenthesis)) {
            while (GetCurrentToken().type != TokenType::tk_close_parenthesis) {
                arguments.push_back(ParseExpression());
                if (GetCurrentToken().type == TokenType::tk_comma) {
                    AdvanceToken();
                }
            }
            ExpectToken(TokenType::tk_close_parenthesis);
        }
        return std::make_shared<RoutineCall>(routineName, arguments);
    }
};
