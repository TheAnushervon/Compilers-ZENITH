#ifndef IRGENERATOR_H
#define IRGENERATOR_H

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/Instructions.h>
#include <unordered_map>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <set>

// Включите ваши заголовочные файлы узлов AST
#include "nodes/node.h"
#include "nodes/program.h"
#include "nodes/routine_declaration.h"
#include "nodes/primitive_type.h"
#include "nodes/type.h"
#include "nodes/parametrs.h"
#include "nodes/parameter_declaration.h"
#include "nodes/body.h"
#include "nodes/variable_declaration.h"
#include "nodes/relation.h"
#include "nodes/return_type.h"
#include "nodes/simple.h"
#include "nodes/primary.h"
#include "nodes/summand.h"
#include "nodes/factor.h"
#include "nodes/modifiable_primary.h"

class IRGenerator {
public:
    llvm::LLVMContext context;                  // Контекст для IR
    llvm::IRBuilder<> builder;                  // Утилита для построения инструкций
    std::unique_ptr<llvm::Module> module;       // Хранилище IR

    IRGenerator() : builder(context) {
        module = std::make_unique<llvm::Module>("MyProgram", context);
    }

    // Основная функция генерации программы
    void generateProgram(Node* node) {
        auto program = dynamic_cast<Program*>(node);
        if (!program) {
            llvm::errs() << "Error: Node is not a Program.\n";
            return;
        }
        for (const auto& routine : program->routineDeclarations) {
            auto routineDecl = dynamic_cast<RoutineDeclaration*>(routine.get());
            if (routineDecl) {
                generateRoutine(routineDecl);
            } else {
                llvm::errs() << "Error: Routine is not a RoutineDeclaration.\n";
            }
        }
    }

private:
    // Генерация функции (рутины)
    void generateRoutine(const RoutineDeclaration* routine) {
        // 1. Извлечение возвращаемого типа
        llvm::Type* returnType = builder.getVoidTy();
        auto type = dynamic_cast<Type*>(routine->returnType.get());
        if (auto primitiveType = dynamic_cast<PrimitiveType*>(type->child.get())) {
            returnType = mapType(type->child.get());
        }
        std::cout << "1 этап: ок\n";

        // 2. Извлечение параметров
        std::vector<llvm::Type*> paramTypes;
        std::set<std::string> param_names; // Для использования в generateExpression и generateRelation
        parseParameters(routine, paramTypes, param_names);
        std::cout << "2 этап: ок\n";

        // 3. Создание типа функции
        llvm::FunctionType* funcType = llvm::FunctionType::get(returnType, paramTypes, false);
        std::cout << "3 этап: ок\n";

        // 4. Создание функции
        auto identifier = dynamic_cast<Identifier*>(routine->identifier.get());
        if (!identifier) {
            llvm::errs() << "Error: Routine has no Identifier.\n";
            return;
        }

        // Очистка имени идентификатора функции
        std::string funcName = sanitizeName(identifier->name);

        llvm::Function* function = llvm::Function::Create(
            funcType,
            llvm::Function::ExternalLinkage,
            funcName,
            module.get()
        );
        std::cout << "4 этап: ок\n";

        // Установка имён для параметров
        unsigned idx = 0;
        if (routine->parameters) {
            auto parameters = dynamic_cast<Parameters*>(routine->parameters.get());
            for (auto& arg : function->args()) {
                if (idx >= parameters->children.size()) {
                    llvm::errs() << "Error: More arguments than parameters.\n";
                    break;
                }
                auto paramDecl = dynamic_cast<ParameterDeclaration*>(parameters->children[idx].get());
                if (paramDecl) {
                    auto paramName = dynamic_cast<Identifier*>(paramDecl->identifier.get());
                    if (paramName) {
                        // Очистка имени параметра
                        std::string name = sanitizeName(paramName->name);
                        arg.setName(name); // Устанавливаем имя параметра
                        param_names.insert(name);
                    }
                }
                ++idx;
            }
        }

        // 5. Создание базового блока
        llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(context, "entry", function);
        builder.SetInsertPoint(entryBlock);
        std::cout << "5 этап: ок\n";

        // 6. Обработка переменных и выражений
        // Создание символической таблицы для локальных переменных
        std::unordered_map<std::string, llvm::Value*> localVars; // Объявление локальных переменных

        if (routine->body) {
            auto body = dynamic_cast<Body*>(routine->body.get());
            if (!body) {
                llvm::errs() << "Error: Body is not of type Body.\n";
                return;
            }
            for (const auto& stmt : body->statements) {
                // Генерация выражения для переменной
                if (auto simDec = dynamic_cast<SimpleDeclaration*>(stmt.get())) {
                    if (auto varDecl = dynamic_cast<VariableDeclaration*>(simDec->child.get())) {
                        auto identifier = dynamic_cast<Identifier*>(varDecl->identifier.get());
                        if (!identifier) {
                            llvm::errs() << "Error: VariableDeclaration without Identifier.\n";
                            continue;
                        }

                        // Очистка имени идентификатора переменной
                        std::string varName = sanitizeName(identifier->name);

                        auto type = dynamic_cast<Type*>(varDecl->type.get());
                        if (!type) {
                            llvm::errs() << "Error: VariableDeclaration without Type.\n";
                            continue;
                        }

                        llvm::Type* varType = mapType(type->child.get());
                        if (!varType) {
                            llvm::errs() << "Error: Cannot map type for variable " << varName << "\n";
                            continue;
                        }

                        llvm::Value* alloca = builder.CreateAlloca(varType, nullptr, varName);
                        localVars[varName] = alloca;

                        if (auto expression = dynamic_cast<Expression*>(varDecl->expression.get())) {
                            if (!expression->relations.empty()) {
                                llvm::Value* exprValue = generateExpression(expression, localVars, param_names);
                                if (exprValue) {
                                    builder.CreateStore(exprValue, alloca);
                                } else {
                                    llvm::errs() << "Error: Failed to generate expression for variable " << varName << "\n";
                                }
                            } else {
                                llvm::errs() << "Error: VariableDeclaration expression does not contain relations.\n";
                            }
                        }
                    }
                }

                // Генерация return
                if (auto returnStmt = dynamic_cast<ReturnType*>(stmt.get())) {
                    auto expression = dynamic_cast<Expression*>(returnStmt->type.get());
                    if (expression && !expression->relations.empty()) {
                        llvm::Value* returnValue = generateExpression(expression, localVars, param_names);
                        if (returnValue) {
                            builder.CreateRet(returnValue); // Возвращаем вычисленное значение
                        } else {
                            llvm::errs() << "Error: Failed to generate return value.\n";
                        }
                    } else {
                        llvm::errs() << "Error: Invalid or empty return expression.\n";
                    }
                }
            }
        }

        std::cout << "6 этап: ок\n";

        // 7. Завершение функции
        if (!llvm::verifyFunction(*function, &llvm::errs())) {
            std::cout << "Function verified successfully.\n";
        } else {
            llvm::errs() << "Function verification failed!\n";
        }

        module->print(llvm::outs(), nullptr);
    }

    // Функция для генерации Expression (Relation { ( and | or | xor ) Relation })
    llvm::Value* generateExpression(
        const Expression* expr,
        std::unordered_map<std::string, llvm::Value*>& localVars,
        const std::set<std::string>& param_names)
    {
        if (expr->relations.empty()) {
            llvm::errs() << "Error: Expression contains no Relations.\n";
            return nullptr;
        }

        // Генерация первого Relation
        llvm::Value* result = generateRelation(dynamic_cast<Relation*>(expr->relations[0].get()), localVars, param_names);
        if (!result) {
            llvm::errs() << "Error: Failed to generate first Relation in Expression.\n";
            return nullptr;
        }

        // Итерируемся по оставшимся Relations и операторам
        for (size_t i = 1; i < expr->relations.size(); ++i) {
            if (i-1 >= expr->operators.size()) {
                llvm::errs() << "Error: Not enough operators in Expression.\n";
                break;
            }

            std::string op = expr->operators[i-1];
            llvm::Value* rhs = generateRelation(dynamic_cast<Relation*>(expr->relations[i].get()), localVars, param_names);
            if (!rhs) {
                llvm::errs() << "Error: Failed to generate Relation at index " << i << " in Expression.\n";
                return nullptr;
            }

            std::cout << "Processing Expression operator: " << op << "\n";

            if (op == "and") {
                result = builder.CreateAnd(result, rhs, "andtmp_expression");
            }
            else if (op == "or") {
                result = builder.CreateOr(result, rhs, "ortmp_expression");
            }
            else if (op == "xor") {
                result = builder.CreateXor(result, rhs, "xortmp_expression");
            }
            else {
                llvm::errs() << "Error: Unsupported Expression operator '" << op << "'.\n";
                return nullptr;
            }
        }

        return result;
    }

    // Функция для генерации Relation (Simple [ ( < | <= | > | >= | = | /= ) Simple ])
    llvm::Value* generateRelation(
        const Relation* rel,
        std::unordered_map<std::string, llvm::Value*>& localVars,
        const std::set<std::string>& param_names)
    {
        if (rel->simples.empty()) {
            llvm::errs() << "Error: Relation contains no Simples.\n";
            return nullptr;
        }

        // Генерация первого Simple
        llvm::Value* result = generateSimple(dynamic_cast<Simple*>(rel->simples[0].get()), localVars, param_names);
        if (!result) {
            llvm::errs() << "Error: Failed to generate first Simple in Relation.\n";
            return nullptr;
        }

        // Итерируемся по оставшимся Simples и операторам
        for (size_t i = 1; i < rel->simples.size(); ++i) {
            if (i-1 >= rel->operators.size()) {
                llvm::errs() << "Error: Not enough operators in Relation.\n";
                break;
            }

            std::string op = rel->operators[i-1];
            llvm::Value* rhs = generateSimple(dynamic_cast<Simple*>(rel->simples[i].get()), localVars, param_names);
            if (!rhs) {
                llvm::errs() << "Error: Failed to generate Simple at index " << i << " in Relation.\n";
                return nullptr;
            }

            std::cout << "Processing Relation operator: " << op << "\n";

            // Обработка операторов сравнения
            if (op == "<") {
                result = builder.CreateICmpSLT(result, rhs, "cmplt_relation");
            }
            else if (op == "<=") {
                result = builder.CreateICmpSLE(result, rhs, "cmple_relation");
            }
            else if (op == ">") {
                result = builder.CreateICmpSGT(result, rhs, "cmpgt_relation");
            }
            else if (op == ">=") {
                result = builder.CreateICmpSGE(result, rhs, "cmpge_relation");
            }
            else if (op == "=") {
                result = builder.CreateICmpEQ(result, rhs, "cmpeq_relation");
            }
            else if (op == "/=") {
                result = builder.CreateICmpNE(result, rhs, "cmpne_relation");
            }
            else {
                llvm::errs() << "Error: Unsupported Relation operator '" << op << "'.\n";
                return nullptr;
            }
        }

        return result;
    }

    // Функция для генерации Simple (Factor { ( + | - ) Factor })
    llvm::Value* generateSimple(
        const Simple* simple,
        std::unordered_map<std::string, llvm::Value*>& localVars,
        const std::set<std::string>& param_names)
    {
        if (simple->factors.empty()) {
            llvm::errs() << "Error: Simple contains no Factors.\n";
            return nullptr;
        }

        // Генерация первого Factor
        llvm::Value* result = generateFactor(dynamic_cast<Factor*>(simple->factors[0].get()), localVars, param_names);
        if (!result) {
            llvm::errs() << "Error: Failed to generate first Factor in Simple.\n";
            return nullptr;
        }

        // Итерируемся по оставшимся Factors и операторам
        for (size_t i = 1; i < simple->factors.size(); ++i) {
            if (i-1 >= simple->operators.size()) {
                llvm::errs() << "Error: Not enough operators in Simple.\n";
                break;
            }

            std::string op = simple->operators[i-1];
            llvm::Value* rhs = generateFactor(dynamic_cast<Factor*>(simple->factors[i].get()), localVars, param_names);
            if (!rhs) {
                llvm::errs() << "Error: Failed to generate Factor at index " << i << " in Simple.\n";
                return nullptr;
            }

            std::cout << "Processing Simple operator: " << op << "\n";

            if (op == "+") {
                result = builder.CreateAdd(result, rhs, "addtmp_simple");
            }
            else if (op == "-") {
                result = builder.CreateSub(result, rhs, "subtmp_simple");
            }
            else {
                llvm::errs() << "Error: Unsupported Simple operator '" << op << "'.\n";
                return nullptr;
            }
        }

        return result;
    }

    // Функция для генерации Factor (Summand { ( * | / | % ) Summand })
    llvm::Value* generateFactor(
        const Factor* factor,
        std::unordered_map<std::string, llvm::Value*>& localVars,
        const std::set<std::string>& param_names)
    {
        if (factor->summands.empty()) {
            llvm::errs() << "Error: Factor contains no Summands.\n";
            return nullptr;
        }

        // Генерация первого Summand
        llvm::Value* result = generateSummand(dynamic_cast<Summand*>(factor->summands[0].get()), localVars, param_names);
        if (!result) {
            llvm::errs() << "Error: Failed to generate first Summand in Factor.\n";
            return nullptr;
        }

        // Итерируемся по оставшимся Summands и операторам
        for (size_t i = 1; i < factor->summands.size(); ++i) {
            if (i-1 >= factor->operators.size()) {
                llvm::errs() << "Error: Not enough operators in Factor.\n";
                break;
            }

            std::string op = factor->operators[i-1];
            llvm::Value* rhs = generateSummand(dynamic_cast<Summand*>(factor->summands[i].get()), localVars, param_names);
            if (!rhs) {
                llvm::errs() << "Error: Failed to generate Summand at index " << i << " in Factor.\n";
                return nullptr;
            }

            std::cout << "Processing Factor operator: " << op << "\n";

            if (op == "*") {
                result = builder.CreateMul(result, rhs, "multmp_factor");
            }
            else if (op == "/") {
                result = builder.CreateSDiv(result, rhs, "divtmp_factor");
            }
            else if (op == "%") {
                result = builder.CreateSRem(result, rhs, "remtmp_factor");
            }
            else {
                llvm::errs() << "Error: Unsupported Factor operator '" << op << "'.\n";
                return nullptr;
            }
        }

        return result;
    }

    // Функция для генерации Summand
    llvm::Value* generateSummand(
        const Summand* summand,
        std::unordered_map<std::string, llvm::Value*>& localVars,
        const std::set<std::string>& param_names)
    {
        if (auto primary = dynamic_cast<Primary*>(summand->child.get())) {
            return generatePrimary(primary, localVars, param_names);
        }
        llvm::errs() << "Error: Summand does not contain Primary.\n";
        return nullptr;
    }

    // Функция для генерации Primary
    llvm::Value* generatePrimary(
        const Primary* primary,
        std::unordered_map<std::string, llvm::Value*>& localVars,
        const std::set<std::string>& param_names)
    {
        if (auto modifiable = dynamic_cast<ModifiablePrimary*>(primary->child.get())) {
            auto identifier = dynamic_cast<Identifier*>(modifiable->identifier.get());
            if (identifier) {
                // Очистка имени идентификатора
                std::string name = sanitizeName(identifier->name);

                llvm::Value* var = nullptr;
                // Проверка локальных переменных
                auto it = localVars.find(name);
                if (it != localVars.end()) {
                    var = it->second;
                    if (!var) {
                        llvm::errs() << "Error: Variable " << name << " in localVars is nullptr.\n";
                        return nullptr;
                    }

                    if (auto allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
                        llvm::Type* varType = allocaInst->getAllocatedType();
                        return builder.CreateLoad(varType, var, name);
                    }
                    else {
                        llvm::errs() << "Error: Variable " << name << " is not an AllocaInst.\n";
                        return nullptr;
                    }
                }
                // Проверка параметров функции
                else if (param_names.find(name) != param_names.end()) {
                    llvm::Function* function = builder.GetInsertBlock()->getParent();
                    for (auto& arg : function->args()) {
                        if (arg.getName() == name) {
                            return &arg; // Возвращаем аргумент напрямую
                        }
                    }
                    llvm::errs() << "Error: Parameter " << name << " not found in function arguments.\n";
                    return nullptr;
                }
                else {
                    llvm::errs() << "Error: Variable " << name << " not found in localVars or parameters.\n";
                    return nullptr;
                }
            }
        }
        llvm::errs() << "Error: Primary node processing failed.\n";
        return nullptr;
    }

    // Функция для парсинга параметров
    void parseParameters(
        const RoutineDeclaration* routine,
        std::vector<llvm::Type*>& paramTypes,
        std::set<std::string>& param_names)
    {
        if (routine->parameters) {
            auto parameters = dynamic_cast<Parameters*>(routine->parameters.get());
            for (const auto& param : parameters->children) {
                auto paramDecl = dynamic_cast<ParameterDeclaration*>(param.get());
                if (paramDecl) {
                    auto t = dynamic_cast<Type*>(paramDecl->type.get());
                    if (!t || !t->child) {
                        llvm::errs() << "Error: ParameterDeclaration without Type or child.\n";
                        continue;
                    }
                    llvm::Type* paramType = mapType(t->child.get());
                    if (!paramType) {
                        llvm::errs() << "Error: Cannot map parameter type.\n";
                        continue;
                    }
                    paramTypes.push_back(paramType);
                    auto paramName = dynamic_cast<Identifier*>(paramDecl->identifier.get());
                    if (paramName) {
                        // Очистка имени параметра
                        std::string name = sanitizeName(paramName->name);
                        llvm::outs() << "Parameter: " << name << "\n";
                        param_names.insert(name);
                    }
                }
            }
        }
    }

    // Функция для сопоставления типов
    llvm::Type* mapType(Node* typeNode) {
        if (auto primitive = dynamic_cast<PrimitiveType*>(typeNode)) {
            if (primitive->name == "integer") {
                return builder.getInt32Ty();
            }
            if (primitive->name == "real") {
                return builder.getDoubleTy();
            }
            if (primitive->name == "boolean") {
                return builder.getInt1Ty();
            }
        }
        return builder.getVoidTy(); // В случае, если тип не распознан
    }

    // Функция для очистки имени (удаление неалфавитно-цифровых символов)
    std::string sanitizeName(const std::string& name) {
        std::string sanitized = name;
        sanitized.erase(std::remove_if(sanitized.begin(), sanitized.end(),
            [](char c) { return !std::isalnum(c); }), sanitized.end());
        return sanitized;
    }
};

#endif // IRGENERATOR_H
