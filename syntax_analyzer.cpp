#include "nodes/array_type.h"
#include "nodes/assignment.h"
#include "nodes/body.h"
#include "nodes/expression.h"
#include "nodes/factor.h"
#include "nodes/for_loop.h"
#include "nodes/identifier.h"
#include "nodes/if_statement.h"
#include "nodes/literal_primary.h"
#include "nodes/modifiable_primary.h"
#include "nodes/node.h"
#include "nodes/parameter_declaration.h"
#include "nodes/parametrs.h"
#include "nodes/primary.h"
#include "nodes/primitive_type.h"
#include "nodes/program.h"
#include "nodes/range.h"
#include "nodes/record_type.h"
#include "nodes/relation.h"
#include "nodes/routine_call.h"
#include "nodes/routine_declaration.h"
#include "nodes/simple.h"
#include "nodes/simple_declaration.h"
#include "nodes/statement.h"
#include "nodes/summand.h"
#include "nodes/type.h"
#include "nodes/type_declaration.h"
#include "nodes/user_type.h"
#include "nodes/variable_declaration.h"
#include "nodes/while_loop.h"
#include "nodes/return_type.h"

#include "tokens/enums/token_type.h"
#include "tokens/structs/token.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

class SyntaxAnalyzer {
  public:
    SyntaxAnalyzer(const std::vector<Token> &tokens)
        : tokens(tokens), currentIndex(0) {}

    // new_version
    std::unique_ptr<Program> Analyze() {
        auto program = std::make_unique<Program>();
        bool flag_terminated = false;

        while (currentIndex < tokens.size()) {
            if (flag_terminated)
                break;

            const Token &token = GetCurrentToken();
           // std::cout << "cur_token : " << toStrin(token.type) << std::endl;

            switch (token.type) {
            case TokenType::tk_routine: {
                auto routine = ParseRoutineDeclaration();
                if (!routine) {
                    std::cerr
                        << "Error: Failed to parse routine declaration.\n" << toStrin(GetCurrentToken().type);
                    return nullptr;
                }
                program->AddRoutineDeclaration(routine);
                break;
            }

            case TokenType::tk_identifier:
            case TokenType::tk_var:
            case TokenType::tk_type: {
                auto simpleDecl = ParseSimpleDeclaration();
                if (!simpleDecl) {
                    std::cerr << "Error: Failed to parse simple declaration.\n" << toStrin(GetCurrentToken().type) << " " <<GetCurrentToken().value;
                    return nullptr;
                }
                program->AddSimpleDeclaration(simpleDecl);
                break;
            }

            case TokenType::tk_terminate:
                flag_terminated = true;
                break;

            default:
                std::cerr << "Syntax error: Unexpected token '" << token.value
                          << "'\n";
                return nullptr;
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

  private:
    const std::vector<Token> &tokens;
    size_t currentIndex;

    const Token &GetCurrentToken() const { return tokens[currentIndex]; }

    const Token &AdvanceToken() {
        if (currentIndex < tokens.size() - 1) {
            currentIndex++;
        }
        return tokens[currentIndex];
    }

    // new_version
    std::shared_ptr<Node> ParseSimpleDeclaration() {
       
        std::shared_ptr<Node> declarationNode;

        auto &currentToken = GetCurrentToken().type;
        
        if (currentToken == TokenType::tk_var) {
            declarationNode = ParseVariableDeclaration();
        } else if (currentToken == TokenType::tk_type) {
            declarationNode = ParseTypeDeclaration();
        } else {
            return nullptr;
        }

        return std::make_shared<SimpleDeclaration>(declarationNode);
    }

    // new_version
    std::shared_ptr<Node> ParseVariableDeclaration() {
      

        if (GetCurrentToken().type != TokenType::tk_var) {
            
            return nullptr;
        }
        AdvanceToken();

        auto identifier = ParseIdentifier();
        if (!identifier) {
            return nullptr;
        }

        std::shared_ptr<Node> type = nullptr;
        std::shared_ptr<Node> expression = nullptr;

        if (GetCurrentToken().type == TokenType::tk_colon) {
            AdvanceToken();

            type = ParseType();
            if (!type) {
                return nullptr;
            }
        }

        if (GetCurrentToken().type == TokenType::tk_is) {
            AdvanceToken();

            expression = ParseExpression();
            if (!expression) {
                return nullptr;
            }
        }

        return std::make_shared<VariableDeclaration>(identifier, type,
                                                     expression);
    }

    // new_version
    std::shared_ptr<Node> ParseTypeDeclaration() {

        if (GetCurrentToken().type != TokenType::tk_type) {
            return nullptr;
        }
        AdvanceToken();

        auto identifier = ParseIdentifier();
        if (!identifier) {
            return nullptr;
        }

        if (GetCurrentToken().type != TokenType::tk_is) {
            return nullptr;
        }
        AdvanceToken();

        auto type = ParseType();
        if (!type) {
            return nullptr;
        }

        return std::make_shared<TypeDeclaration>(identifier, type);
    }

    // new_version
    std::shared_ptr<Node> ParseRoutineDeclaration() {

        
        //ghjdthz
        if (GetCurrentToken().type != TokenType::tk_routine) {
            return nullptr;
        }
        AdvanceToken();

        auto identifier = ParseIdentifier();
        if (!identifier) {
            return nullptr;
        }

        if (GetCurrentToken().type != TokenType::tk_open_parenthesis) {
            return nullptr;
        }
        AdvanceToken();

        auto parameters = ParseParameters();
        if (!parameters) {
            return nullptr;
        }

        if (GetCurrentToken().type != TokenType::tk_close_parenthesis) {
            return nullptr;
        }
        AdvanceToken();

        std::shared_ptr<Node> returnType = nullptr;
        if (GetCurrentToken().type == TokenType::tk_colon) {
            AdvanceToken(); // Пропускаем ":"
            returnType = ParseType();
            if (!returnType) {
                 
                return nullptr;
            }
        }

        if (GetCurrentToken().type != TokenType::tk_is) {
             
            return nullptr;
        }
        AdvanceToken();
           
        auto body = ParseBody();
        if (!body) {
             
            return nullptr;
        }

        if (GetCurrentToken().type != TokenType::tk_end) {
             
            return nullptr;
        }
        AdvanceToken();

        return std::make_shared<RoutineDeclaration>(identifier, parameters,
                                                    returnType, body);
    }

    // new_version
    std::shared_ptr<Node> ParseParameters() {

        std::vector<std::shared_ptr<Node>> parameters;

        auto param = ParseParameterDeclaration();
        if (!param && GetCurrentToken().type == TokenType::tk_close_parenthesis) {
             return std::make_shared<Parameters>(parameters);
        }
        if (!param) {
            return nullptr;
        }
        parameters.push_back(param);

        while (GetCurrentToken().type == TokenType::tk_comma) {
            AdvanceToken();

            param = ParseParameterDeclaration();
            if (!param) {
                 
                return nullptr;
            }
            parameters.push_back(param);
        }

        return std::make_shared<Parameters>(parameters);
    }

    // new_version
    std::shared_ptr<Node> ParseParameterDeclaration() {

        auto identifier = ParseIdentifier();
        if (!identifier) {
             
            return nullptr;
        }

        if (GetCurrentToken().type != TokenType::tk_colon) {
             
            return nullptr;
        }
        AdvanceToken();

        auto type = ParseType();
        if (!type) {
             
            return nullptr;
        }

        return std::make_shared<ParameterDeclaration>(identifier, type);
    }

    // new_version
    std::shared_ptr<Node> ParseType() {

        std::shared_ptr<Node> typeNode;

        TokenType currentToken = GetCurrentToken().type;

        if (currentToken == TokenType::tk_integer ||
            currentToken == TokenType::tk_real ||
            currentToken == TokenType::tk_boolean) {
            typeNode = ParsePrimitiveType();
        } else if (currentToken == TokenType::tk_array) {
            typeNode = ParseArrayType();
        }

        else if (currentToken == TokenType::tk_record) {
            typeNode = ParseRecordType();
        } else if (currentToken == TokenType::tk_identifier) {
            typeNode = ParseIdentifier();
        } else {
             
            return nullptr;
        }

        return std::make_shared<Type>(typeNode);
    }

    // new_version
    std::shared_ptr<Node> ParsePrimitiveType() {


        TokenType currentToken = GetCurrentToken().type;
        std::string typeName;

        if (currentToken == TokenType::tk_integer) {
            typeName = "integer";
        } else if (currentToken == TokenType::tk_real) {
            typeName = "real";
        } else if (currentToken == TokenType::tk_boolean) {
            typeName = "boolean";
        } else {
             
            return nullptr;
        }

        AdvanceToken();
        return std::make_shared<PrimitiveType>(typeName);
    }

    // new_version
    std::shared_ptr<Node> ParseRecordType() {

        if (GetCurrentToken().type != TokenType::tk_record) {
             
            return nullptr;
        }
        AdvanceToken();

        std::vector<std::shared_ptr<Node>> members;

        while (GetCurrentToken().type != TokenType::tk_end) {
            auto member = ParseVariableDeclaration();
            if (!member) {
                 
                return nullptr;
            }
            members.push_back(member);
        }

        if (GetCurrentToken().type != TokenType::tk_end) {
             
            return nullptr;
        }
        AdvanceToken();

        return std::make_shared<RecordType>(members);
    }

    // new_version
    std::shared_ptr<Node> ParseArrayType() {

        if (GetCurrentToken().type != TokenType::tk_array) {
             
            return nullptr;
        }
        AdvanceToken();

        std::shared_ptr<Node> sizeExpression = nullptr;
        if (GetCurrentToken().type == TokenType::tk_open_bracket) {
            AdvanceToken();

            if (GetCurrentToken().type != TokenType::tk_close_bracket) {
                sizeExpression = ParseExpression();
                if (!sizeExpression) {
                     
                    return nullptr;
                }
            }

            if (GetCurrentToken().type != TokenType::tk_close_bracket) {
                 
                return nullptr;
            }
            AdvanceToken();
        }
        auto elementType = ParseType();
        if (!elementType) {
             
            return nullptr;
        }

        return std::make_shared<ArrayType>(elementType, sizeExpression);
    }

    // new_version
    std::shared_ptr<Node> ParseBody() {
       

        std::vector<std::shared_ptr<Node>> bodyNodes;


        while (true) {
            std::shared_ptr<Node> childNode;
            if (GetCurrentToken().type == TokenType::tk_return){
                childNode = ParseReturnType();
            }
            else if (GetCurrentToken().type == TokenType::tk_var  ||GetCurrentToken().type ==TokenType::tk_type) {
            childNode = ParseSimpleDeclaration();
            }
            else if (!childNode) {
                childNode = ParseStatement();
            }

            if (!childNode) {
                break;
            }

            bodyNodes.push_back(childNode);
        }

        if (bodyNodes.empty()) {
             
            return nullptr;
        }

        return std::make_shared<Body>(bodyNodes);
    }

    // new_version
    std::shared_ptr<Node> ParseStatement() {
        std::shared_ptr<Node> statementNode;

        switch (GetCurrentToken().type) {
        case TokenType::tk_identifier:
            statementNode = ParseAssignment();
            if (!statementNode) {
                statementNode = ParseRoutineCall();
            }
            break;

        case TokenType::tk_while:
            statementNode = ParseWhileLoop();
            break;

        case TokenType::tk_for:
            statementNode = ParseForLoop();
            break;

        case TokenType::tk_if:
            statementNode = ParseIfStatement();
            break;

        default:
             
            return nullptr;
        }

        if (!statementNode) {
             
            return nullptr;
        }

        return std::make_shared<Statement>(statementNode);
    }

    // new_version
    std::shared_ptr<Node> ParseAssignment() {

        auto modifiablePrimary = ParseModifiablePrimary();
        if (!modifiablePrimary) {
             
            return nullptr;
        }

        if (GetCurrentToken().type != TokenType::tk_assign) {
             
            return nullptr;
        }
        AdvanceToken();
        auto expression = ParseExpression();
        if (!expression) {
             
            return nullptr;
        }

        return std::make_shared<Assignment>(modifiablePrimary, expression);
    }

    // new_version
    std::shared_ptr<Node> ParseRoutineCall() {


        auto identifier = ParseIdentifier();
        if (!identifier) {
             
            return nullptr;
        }

        std::vector<std::shared_ptr<Node>> expressions;

        if (GetCurrentToken().type == TokenType::tk_open_parenthesis) {
            AdvanceToken(); // Пропускаем '('

            if (GetCurrentToken().type != TokenType::tk_close_parenthesis) {
                while (true) {
                    auto expr = ParseExpression();
                    if (!expr) {
                         
                        return nullptr;
                    }
                    expressions.push_back(expr);

                    if (GetCurrentToken().type == TokenType::tk_comma) {
                        AdvanceToken();
                    } else {
                        break;
                    }
                }
            }

            if (GetCurrentToken().type != TokenType::tk_close_parenthesis) {
                 
                return nullptr;
            }
            AdvanceToken();
        }

        return std::make_shared<RoutineCall>(identifier, expressions);
    }

    // new_version
    std::shared_ptr<Node> ParseWhileLoop() {

        if (GetCurrentToken().type != TokenType::tk_while) {
             
            return nullptr;
        }
        AdvanceToken();

        auto expression = ParseExpression();
        if (!expression) {
             
            return nullptr;
        }

        if (GetCurrentToken().type != TokenType::tk_loop) {
             
            return nullptr;
        }
        AdvanceToken();

        auto body = ParseBody();
        if (!body) {
             
            return nullptr;
        }

        if (GetCurrentToken().type != TokenType::tk_end) {
             
            return nullptr;
        }
        AdvanceToken();

        return std::make_shared<WhileLoop>(expression, body);
    }

    // new_version
    std::shared_ptr<Node> ParseForLoop() {

        if (GetCurrentToken().type != TokenType::tk_for) {
             
            return nullptr;
        }
        AdvanceToken();

        auto identifier = ParseIdentifier();
        if (!identifier) {
             
            return nullptr;
        }

        if (GetCurrentToken().type != TokenType::tk_in) {
             
            return nullptr;
        }
        AdvanceToken();

        bool isReverse = false;
        if (GetCurrentToken().type == TokenType::tk_reverse) {
            isReverse = true;
            AdvanceToken();
        }

        auto range = ParseRange();
        if (!range) {
             
            return nullptr;
        }

        if (GetCurrentToken().type != TokenType::tk_loop) {
             
            return nullptr;
        }
        AdvanceToken();

        auto body = ParseBody();
        if (!body) {
             
            return nullptr;
        }

        if (GetCurrentToken().type != TokenType::tk_end) {
             
            return nullptr;
        }
        AdvanceToken();

        auto forLoopNode = std::make_shared<ForLoop>(identifier, range, body);
        if (isReverse) {
            forLoopNode->setReverse(true);
        }
        return forLoopNode;
    }

    // new_version
    std::shared_ptr<Node> ParseRange() {
        auto startExpr = ParseExpression();
        if (!startExpr) {
             
            return nullptr;
        }

        if (GetCurrentToken().type != TokenType::tk_range) {
             
            return nullptr;
        }
        AdvanceToken();

        auto endExpr = ParseExpression();
        if (!endExpr) {
             
            return nullptr;
        }

        return std::make_shared<Range>(startExpr, endExpr);
    }

    // new_version
    std::shared_ptr<Node> ParseIfStatement() {
        if (GetCurrentToken().type != TokenType::tk_if) {
            return nullptr;
        }
        AdvanceToken();
        //подумать
        auto ifExpression = ParseExpression();
        if (!ifExpression) {
            return nullptr;
        }
        if (GetCurrentToken().type != TokenType::tk_then) {
            return nullptr;
        }
       
        AdvanceToken();

    
        auto thenBody = ParseBody();
        if (!thenBody) {
             
            return nullptr;
        }

        std::shared_ptr<Node> elseBody = nullptr;
        if (GetCurrentToken().type == TokenType::tk_else) {
            AdvanceToken();
            elseBody = ParseBody();
            if (!elseBody) {
                 
                return nullptr;
            }
        }

        if (GetCurrentToken().type != TokenType::tk_end) {
             
            return nullptr;
        }
        AdvanceToken();
        return std::make_shared<IfStatement>(ifExpression, thenBody, elseBody);
    }

    // new_version
    std::shared_ptr<Node> ParseExpression() {
        std::vector<std::shared_ptr<Node>> relations;
        relations.push_back(ParseRelation()); // Разбираем первый Relation

        while (true) {
            TokenType currentToken = GetCurrentToken().type;

            if (currentToken == TokenType::tk_and ||
                currentToken == TokenType::tk_or ||
                currentToken == TokenType::tk_xor) {

                AdvanceToken(); 

                relations.push_back(ParseRelation());
            } else {
                break; 
            }
        }
        return std::make_shared<Expression>(relations);
    }

    // new_version
    std::shared_ptr<Node> ParseRelation() {
        std::vector<std::shared_ptr<Node>> simples;
        simples.push_back(ParseSimple());

        TokenType currentToken = GetCurrentToken().type;

        if (currentToken == TokenType::tk_less_than ||
            currentToken == TokenType::tk_less_than_equal ||
            currentToken == TokenType::tk_greater_than ||
            currentToken == TokenType::tk_greater_than_equal ||
            currentToken == TokenType::tk_equal ||
            currentToken == TokenType::tk_not_equal) {

            //warning: добавить в ноду
            auto operatorToken = GetCurrentToken();
            AdvanceToken();

            simples.push_back(ParseSimple());
        }

        return std::make_shared<Relation>(simples);
    }

    // new version
    std::shared_ptr<Node> ParseSimple() {
        std::vector<std::shared_ptr<Node>> factors;
        factors.push_back(ParseFactor());

        while (true) {
            TokenType currentToken = GetCurrentToken().type;

            if (currentToken == TokenType::tk_multiply ||
                currentToken == TokenType::tk_divide ||
                currentToken == TokenType::tk_mod) {
                auto operatorToken = GetCurrentToken();
                AdvanceToken();

                factors.push_back(ParseFactor());
            } else {
                break;
            }
        }
        return std::make_shared<Simple>(factors);
    }

    // new_version
    std::shared_ptr<Node> ParseFactor() {
        std::vector<std::shared_ptr<Node>> summands;
        summands.push_back(ParseSummand());

        while (true) {
            TokenType currentToken = GetCurrentToken().type;

            if (currentToken == TokenType::tk_add ||
                currentToken == TokenType::tk_subtract) {
                //warning: добавить в ноды
                auto operatorToken = GetCurrentToken();
                AdvanceToken();

                summands.push_back(ParseSummand());
            } else {
                break;
            }
        }
        return std::make_shared<Factor>(summands);
    }

    // new_version
//cлагаемое 
    std::shared_ptr<Node> ParseSummand() {
        std::shared_ptr<Node> summandNode;

        TokenType currentToken = GetCurrentToken().type;

        // (5 + 6)
        if (currentToken == TokenType::tk_open_parenthesis) {
            AdvanceToken();
            summandNode = ParseExpression();
            if (GetCurrentToken().type != TokenType::tk_close_parenthesis) {
                 
                return nullptr;
            }
            AdvanceToken();
        } else {

            summandNode = ParsePrimary();
        }

        return std::make_shared<Summand>(summandNode);
    }

    // new_version
    std::shared_ptr<Node> ParsePrimary() {
        std::shared_ptr<Node> primaryNode;

        TokenType currentToken = GetCurrentToken().type;

        if (currentToken == TokenType::tk_num) {
            //warning: только int
            if (std::any_of(GetCurrentToken().value.begin(), GetCurrentToken().value.end(), [](char c) { return c == '.'; })) {
                double number = std::stod(GetCurrentToken().value);
                primaryNode = std::make_shared<LiteralPrimary>(number);
            } else {
                int numericValue = std::stoi(GetCurrentToken().value);
                primaryNode = std::make_shared<LiteralPrimary>(numericValue);
            }

            AdvanceToken();
        } else if (currentToken == TokenType::tk_true ||
                   currentToken == TokenType::tk_false) {
            bool boolValue = (currentToken == TokenType::tk_true);
            primaryNode = std::make_shared<LiteralPrimary>(boolValue);
            AdvanceToken();
        } //пока здесь не были
        else if (currentToken == TokenType::tk_identifier) {
            primaryNode = ParseModifiablePrimary();
        } else if (currentToken == TokenType::tk_open_parenthesis) {
            AdvanceToken();
            primaryNode = ParseExpression();
            if (GetCurrentToken().type != TokenType::tk_close_parenthesis) {
                 
                return nullptr;
            }
            AdvanceToken();
        } else {
             
            return nullptr;
        }

        return std::make_shared<Primary>(primaryNode);
    }

    // new_version
    std::shared_ptr<Node> ParseModifiablePrimary() {
        //нужен ли AdvanceToken?
        auto modifiablePrimary = std::make_shared<ModifiablePrimary>();
        modifiablePrimary->identifier = ParseIdentifier();
        if (GetCurrentToken().type == TokenType::tk_dot) {
            AdvanceToken();
        modifiablePrimary->specialIdentifier = ParseIdentifier();
        }
        if (GetCurrentToken().type == TokenType::tk_open_bracket) {
            AdvanceToken();
        modifiablePrimary->expression = ParseExpression();
             AdvanceToken();
        }
        return modifiablePrimary;
    }

    // new_version
    std::shared_ptr<Node> ParseIdentifier() {
        // for case Identifier { . Identifier...}
        //record.p 
        //record[56]
        if (GetCurrentToken().type == TokenType::tk_dot) {
            AdvanceToken();
        }
        if (GetCurrentToken().type != TokenType::tk_identifier) {
            return nullptr;
        }
        auto identifier = std::make_shared<Identifier>(GetCurrentToken().value);
        AdvanceToken();
        return identifier;
    }

    std::shared_ptr<Node> ParseReturnType() {
        if (GetCurrentToken().type != TokenType::tk_return) {
            return nullptr;
        }
        AdvanceToken();
        auto returnType = ParseModifiablePrimary();
        return std::make_shared<ReturnType>(returnType);
    }
};
