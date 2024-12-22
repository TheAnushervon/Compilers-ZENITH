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
        while (currentIndex < tokens.size() && !errorOccurred) {
            const Token &token = GetCurrentToken();
            switch (token.type) {
            case TokenType::tk_routine:
                program->AddRoutineDeclaration(ParseRoutine());
                break;
            case TokenType::tk_var:
            case TokenType::tk_type:
                program->AddSimpleDeclaration(ParseSimpleDeclaration());
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
            AdvanceToken();
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
            parameters = ParseParameters();
            ExpectToken(TokenType::tk_close_parenthesis);
        }

        std::shared_ptr<Type> returnType = nullptr;
        if (GetCurrentToken().type == TokenType::tk_colon) {
            AdvanceToken();
            returnType = ParseType();
        }

        std::shared_ptr<Body> body = nullptr;
        if (ExpectToken(TokenType::tk_is)) {
            body = ParseBody();
        }

        ExpectToken(TokenType::tk_end);
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

        if (!ExpectToken(TokenType::tk_identifier)) {
            return nullptr;
        }
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
