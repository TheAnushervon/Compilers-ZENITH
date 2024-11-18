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
#include <fstream>

// Включите ваши заголовочные файлы узлов AST
#include "nodes/node.h"
#include "nodes/program.h"
#include "utility/utils.cpp"
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
#include "nodes/if_statement.h"
#include "nodes/assignment.h"
#include "nodes/routine_call.h"


class IRGenerator {
public:
    llvm::LLVMContext context;                  // Контекст для IR
    llvm::IRBuilder<> builder;                  // Утилита для построения инструкций
    std::unique_ptr<llvm::Module> module;       // Хранилище IR
    const std::string logFilePath = "logs/ir_generator.txt";

    IRGenerator() : builder(context) {
        module = std::make_unique<llvm::Module>("MyProgram", context);
        declarePrintFunctions(); // Объявляем функции печати
    }


    // Основная функция генерации программы
    void generateProgram(Node* node) {
        auto program = dynamic_cast<Program*>(node);
        if (!program) {
            llvm::errs() << "Error: Node is not a Program.\n";
            return;
        }

        // Обрабатываем глобальные переменные
        for (const auto& simpleDecl : program->simpleDeclarations) {
            if (auto curr = dynamic_cast<SimpleDeclaration*>(simpleDecl.get())) {
                if (auto varDecl = dynamic_cast<VariableDeclaration*>(curr->child.get())) {
                    generateVariableDeclaration(varDecl);
                }
            }
        }

        // Парсим рутины
        for (const auto& routine : program->routineDeclarations) {
            auto routineDecl = dynamic_cast<RoutineDeclaration*>(routine.get());
            if (routineDecl) {
                generateRoutine(routineDecl);
            } else {
                llvm::errs() << "Error: Routine is not a RoutineDeclaration.\n";
            }
        }

        // Генерация main для обработки глобальных переменных и вызовов
        if (!program->simpleDeclarations.empty()) {
            generateMain(program);
        }

        // Сохраняем IR в файл
        std::string moduleStr;
        llvm::raw_string_ostream rso(moduleStr);
        module->print(rso, nullptr);

        std::ofstream astFile("llvm_code.txt");
        if (astFile.is_open()) {
            astFile << rso.str();
            astFile.close();
        } else {
            std::cerr << "Failed to open file llvm_code.txt for writing." << std::endl;
        }
        std::ofstream outFile("generated_code.ll");
        outFile << rso.str();
        outFile.close();
    }

private:
    //генерация симплов
    std::unordered_map<std::string, llvm::Value*> globalVars;

   void generateMain(const Program* program) {
    llvm::FunctionType* mainType = llvm::FunctionType::get(builder.getInt32Ty(), false);
    llvm::Function* mainFunc = llvm::Function::Create(
        mainType, llvm::Function::ExternalLinkage, "main", module.get());

    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(context, "entry", mainFunc);
    builder.SetInsertPoint(entryBlock);

    // Создаём локальный кэш для загруженных значений глобальных переменных
    std::unordered_map<std::string, llvm::Value*> localVars;

    // Загружаем глобальные переменные
    for (const auto& simpleDecl : program->simpleDeclarations) {
        if (auto curr = dynamic_cast<SimpleDeclaration*>(simpleDecl.get())) {
            if (auto varDecl = dynamic_cast<VariableDeclaration*>(curr->child.get())) {
                std::string varName = dynamic_cast<Identifier*>(varDecl->identifier.get())->name;
                if (globalVars.find(varName) != globalVars.end()) {
                    auto globalVar = llvm::dyn_cast<llvm::GlobalVariable>(globalVars[varName]);
                    if (!globalVar) continue;

                    // Загружаем значение глобальной переменной и сохраняем в локальный кэш
                    llvm::Value* loadedValue = builder.CreateLoad(globalVar->getValueType(), globalVar, varName);
                    localVars[varName] = loadedValue;
                }
            }
        }
    }

    // Генерация вызовов функций и обработка Print
    for (const auto& simpleDecl : program->simpleDeclarations) {
        if (auto routineCall = dynamic_cast<RoutineCall*>(simpleDecl.get())) {
                generateRoutineCall(routineCall, localVars);
            }

        else if (auto printNode = dynamic_cast<Print*>(simpleDecl.get())) {
            generatePrint(printNode, localVars);
        }
    }

    // Завершаем main возвратом 0
    builder.CreateRet(llvm::ConstantInt::get(builder.getInt32Ty(), 0));
    llvm::outs() << "Main function generated.\n";
}

void generatePrint(const Print* printNode, std::unordered_map<std::string, llvm::Value*>& localVars) {
    llvm::Value* valueToPrint = nullptr;

    // Если внутри Print находится RoutineCall
    if (auto routineCall = dynamic_cast<RoutineCall*>(printNode->child.get())) {
        llvm::outs() << "Generating Print for RoutineCall...\n";
        valueToPrint = generateRoutineCall(routineCall, localVars);
    }
    // Если внутри Print находится Identifier
    else if (auto identifier = dynamic_cast<Identifier*>(printNode->child.get())) {
        std::string varName = sanitizeName(identifier->name);
        llvm::outs() << "Generating Print for Identifier: " << varName << "\n";

        // Проверяем локальные переменные
        if (localVars.find(varName) != localVars.end()) {
            valueToPrint = localVars[varName];
        }
        // Проверяем глобальные переменные
        else if (globalVars.find(varName) != globalVars.end()) {
            auto globalVar = llvm::dyn_cast<llvm::GlobalVariable>(globalVars[varName]);
            if (!globalVar) {
                llvm::errs() << "Error: '" << varName << "' is not a GlobalVariable.\n";
                return;
            }
            valueToPrint = builder.CreateLoad(globalVar->getValueType(), globalVar, varName);
        } else {
            llvm::errs() << "Error: Variable '" << varName << "' not found.\n";
            return;
        }
    } else {
        llvm::errs() << "Error: Print node contains unsupported child type.\n";
        return;
    }

    if (!valueToPrint) {
        llvm::errs() << "Error: Failed to generate value for Print.\n";
        return;
    }

    // Генерация вызова функции Print в зависимости от типа
    if (valueToPrint->getType()->isIntegerTy()) {
        llvm::Function* printFunc = module->getFunction("PrintInt");
        if (!printFunc) {
            llvm::errs() << "Error: PrintInt function not found in module.\n";
            return;
        }
        builder.CreateCall(printFunc, {valueToPrint});
    } else if (valueToPrint->getType()->isDoubleTy()) {
        llvm::Function* printFunc = module->getFunction("PrintReal");
        if (!printFunc) {
            llvm::errs() << "Error: PrintReal function not found in module.\n";
            return;
        }
        builder.CreateCall(printFunc, {valueToPrint});
    } else if (valueToPrint->getType()->isIntegerTy(1)) { // Boolean
        llvm::Function* printFunc = module->getFunction("PrintBoolean");
        if (!printFunc) {
            llvm::errs() << "Error: PrintBoolean function not found in module.\n";
            return;
        }
        builder.CreateCall(printFunc, {valueToPrint});
    } else {
        llvm::errs() << "Error: Unsupported type for Print.\n";
    }
}




llvm::Value* generateRoutineCall(const RoutineCall* routineCall, const std::unordered_map<std::string, llvm::Value*>& localVars) {
    auto identifier = dynamic_cast<Identifier*>(routineCall->identifier.get());
    if (!identifier) {
        llvm::errs() << "Error: RoutineCall identifier is not valid.\n";
        return nullptr;
    }

    std::string functionName = sanitizeName(identifier->name);

    llvm::Function* function = module->getFunction(functionName);
    if (!function) {
        llvm::errs() << "Error: Function '" << functionName << "' not found in module.\n";
        return nullptr;
    }

    std::vector<llvm::Value*> args;
    auto paramIt = function->args().begin();

    for (const auto& expr : routineCall->expressions) {
        auto expression = dynamic_cast<Expression*>(expr.get());
        if (!expression || expression->relations.empty()) {
            llvm::errs() << "Error: RoutineCall contains an invalid argument.\n";
            return nullptr;
        }

        auto relation = dynamic_cast<Relation*>(expression->relations[0].get());
        auto simple = dynamic_cast<Simple*>(relation->simples[0].get());
        auto factor = dynamic_cast<Factor*>(simple->factors[0].get());
        auto summand = dynamic_cast<Summand*>(factor->summands[0].get());
        auto primary = dynamic_cast<Primary*>(summand->child.get());
        auto modifiablePrimary = dynamic_cast<ModifiablePrimary*>(primary->child.get());
        auto argIdentifier = dynamic_cast<Identifier*>(modifiablePrimary->identifier.get());
        std::string argName = sanitizeName(argIdentifier->name);

        // Используем локальный кэш, если значение переменной уже загружено
        if (localVars.find(argName) != localVars.end()) {
            args.push_back(localVars.at(argName));
        } else {
            llvm::errs() << "Error: Variable '" << argName << "' not found in localVars.\n";
            return nullptr;
        }

        ++paramIt;
    }

    return builder.CreateCall(function, args, "calltmp_routine");
}


    void generateVariableDeclaration(const VariableDeclaration* varDecl) {
        auto identifier = dynamic_cast<Identifier*>(varDecl->identifier.get());
        if (!identifier) return;

        std::string varName = identifier->name;

        // Определение типа переменной
        auto typeNode = dynamic_cast<Type*>(varDecl->type.get());
        if (!typeNode || !typeNode->child) {
            llvm::errs() << "Error: VariableDeclaration without Type or child.\n";
            return;
        }

        llvm::Type* llvmType = mapType(typeNode->child.get());
        if (!llvmType) {
            llvm::errs() << "Error: Failed to map type for variable: " << varName << "\n";
            return;
        }

        // Создание начального значения
        llvm::Constant* initializer = llvm::Constant::getNullValue(llvmType);
        if (varDecl->expression) {
            auto expr = dynamic_cast<Expression*>(varDecl->expression.get());
            if (expr) {
                llvm::Value* value = generateExpression(expr, globalVars, {});
                initializer = llvm::dyn_cast<llvm::Constant>(value);
                if (!initializer) {
                    llvm::errs() << "Error: Failed to generate initializer for variable: " << varName << "\n";
                    return;
                }
            }
        }

        // Создание глобальной переменной
        llvm::GlobalVariable* globalVar = new llvm::GlobalVariable(
            *module, llvmType, false, llvm::GlobalValue::ExternalLinkage,
            initializer, varName);

        globalVars[varName] = globalVar;
    }



    // Генерация функции (рутины)
    void generateRoutine(const RoutineDeclaration* routine) {
        // 1. Извлечение возвращаемого типа
        llvm::Type* returnType = builder.getVoidTy();
        auto type = dynamic_cast<Type*>(routine->returnType.get());
        if (type && type->child) {
            if (auto primitiveType = dynamic_cast<PrimitiveType*>(type->child.get())) {
                returnType = mapType(type->child.get());
            }
        }
        utils::log(logFilePath, "INFO", "1 этап: ок");
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
        llvm::outs() << "Creating function: " << funcName << "\n";

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
                        llvm::outs() << "Parameter " << idx << ": " << name << "\n";
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
                        llvm::outs() << "Declaring variable: " << varName << "\n";

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
                        llvm::outs() << "Allocated space for " << varName << "\n";

                        if (auto expression = dynamic_cast<Expression*>(varDecl->expression.get())) {
                            if (!expression->relations.empty()) {
                                llvm::Value* exprValue = generateExpression(expression, localVars, param_names);
                                if (exprValue) {
                                    builder.CreateStore(exprValue, alloca);
                                    llvm::outs() << "Stored value in " << varName << "\n";
                                } else {
                                    llvm::errs() << "Error: Failed to generate expression for variable " << varName << "\n";
                                }
                            } else {
                                llvm::errs() << "Error: VariableDeclaration expression does not contain relations.\n";
                            }
                        }
                    }
                }

                if (auto st = dynamic_cast<Statement*>(stmt.get())) {
                    // Обработка IfStatement
                    if (auto ifStmt = dynamic_cast<IfStatement*>(st->child.get())) {
                        generateIfStatement(ifStmt, localVars, param_names);
                    }
                }

                // Обработка Assignment
                if (auto assignStmt = dynamic_cast<Assignment*>(stmt.get())) {
                    generateAssignment(assignStmt, localVars, param_names);
                }

                // Генерация return
                if (auto returnStmt = dynamic_cast<ReturnType*>(stmt.get())) {
                    // Проверяем, существует ли член 'expression' в ReturnType
                    // Предполагается, что 'expression' является публичным членом класса ReturnType
                    auto expression = dynamic_cast<Expression*>(returnStmt->type.get());
                    if (expression && !expression->relations.empty()) {
                        llvm::Value* returnValue = generateExpression(expression, localVars, param_names);
                        if (returnValue) {
                            builder.CreateRet(returnValue); // Возвращаем вычисленное значение
                            llvm::outs() << "Return statement generated.\n";
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

    // Метод для генерации IfStatement
  void generateIfStatement(
    const IfStatement* ifStmt,
    std::unordered_map<std::string, llvm::Value*>& localVars,
    const std::set<std::string>& param_names
) {
    llvm::outs() << "Generating IfStatement...\n";

    // Генерация условия
    llvm::Value* condValue = generateExpression(dynamic_cast<Expression*>(ifStmt->ifExpression.get()), localVars, param_names);
    if (!condValue) {
        llvm::errs() << "Error: Failed to generate condition for if statement.\n";
        return;
    }
    llvm::outs() << "Condition generated successfully.\n";

    // Приведение условия к типу i1 (булев)
    if (!condValue->getType()->isIntegerTy(1)) {
        condValue = builder.CreateICmpNE(condValue, llvm::ConstantInt::get(condValue->getType(), 0), "ifcond");
        llvm::outs() << "Condition converted to i1.\n";
    } else {
        llvm::outs() << "Condition is already i1.\n";
    }

    // Получение текущей функции
    llvm::Function* function = builder.GetInsertBlock()->getParent();

    // Создание базовых блоков
    llvm::BasicBlock* thenBB = llvm::BasicBlock::Create(context, "then", function);
    llvm::BasicBlock* elseBB = llvm::BasicBlock::Create(context, "else", function);
    llvm::BasicBlock* mergeBB = llvm::BasicBlock::Create(context, "ifcont", function);

    // Создание перехода на then или else
    builder.CreateCondBr(condValue, thenBB, elseBB);
    llvm::outs() << "Conditional branch created.\n";

    // Генерация блока then
    builder.SetInsertPoint(thenBB);
    llvm::outs() << "Generating then block...\n";

    if (ifStmt->thenBody) {
        auto body = dynamic_cast<Body*>(ifStmt->thenBody.get());
        if (body) {
            for (const auto& stmt : body->statements) {
                // Обработка присваивания
                if (auto curr = dynamic_cast<Statement*>(stmt.get())) {
                    if (auto assignStmt = dynamic_cast<Assignment*>(curr->child.get())) {
                        generateAssignment(assignStmt, localVars, param_names);
                    }
                }
            }
        }
    }

    // Если блок не содержит терминатора, добавляем переход на merge
    if (!thenBB->getTerminator()) {
        builder.CreateBr(mergeBB);
        llvm::outs() << "Added branch to merge in then block.\n";
    }

    // Генерация блока else
    builder.SetInsertPoint(elseBB);
    llvm::outs() << "Generating else block...\n";

    if (ifStmt->elseBody) {
        auto body = dynamic_cast<Body*>(ifStmt->elseBody.get());
        if (body) {
            for (const auto& stmt : body->statements) {
                // Обработка присваивания
                if (auto assignStmt = dynamic_cast<Assignment*>(stmt.get())) {
                    generateAssignment(assignStmt, localVars, param_names);
                }
            }
        }
    }

    // Если блок не содержит терминатора, добавляем переход на merge
    if (!elseBB->getTerminator()) {
        builder.CreateBr(mergeBB);
        llvm::outs() << "Added branch to merge in else block.\n";
    }

    // Установка точки вставки в блок merge
    builder.SetInsertPoint(mergeBB);
    llvm::outs() << "Set insert point to merge block.\n";
}


    // Метод для генерации Assignment (Присваивания)
    void generateAssignment(
        const Assignment* assignStmt,
        std::unordered_map<std::string, llvm::Value*>& localVars,
        const std::set<std::string>& param_names
    ) {
        llvm::outs() << "Generating Assignment...\n";

        // Получаем имя переменной
        auto modifiable = dynamic_cast<ModifiablePrimary*>(assignStmt->modifiablePrimary.get());
        if (!modifiable) {
            llvm::errs() << "Error: Assignment does not contain ModifiablePrimary.\n";
            return;
        }
        auto identifier = dynamic_cast<Identifier*>(modifiable->identifier.get());
        if (!identifier) {
            llvm::errs() << "Error: ModifiablePrimary does not contain Identifier.\n";
            return;
        }

        std::string varName = sanitizeName(identifier->name);
        llvm::outs() << "Assigning to variable: " << varName << "\n";

        // Поиск переменной в локальных
        auto it = localVars.find(varName);
        if (it == localVars.end()) {
            llvm::errs() << "Error: Variable " << varName << " not found.\n";
            return;
        }

        llvm::Value* varAlloca = it->second;

        // Генерация выражения для присваивания
        auto exprNode = assignStmt->expression.get();
        auto expression = dynamic_cast<Expression*>(exprNode);
        if (!expression) {
            llvm::errs() << "Error: Assignment expression is not of type Expression.\n";
            return;
        }

        llvm::Value* exprValue = generateExpression(expression, localVars, param_names);
        if (!exprValue) {
            llvm::errs() << "Error: Failed to generate expression for assignment.\n";
            return;
        }

        // Создаём инструкцию store
        builder.CreateStore(exprValue, varAlloca);
        llvm::outs() << "Stored value in " << varName << "\n";
    }





    // Вспомогательный метод для генерации тела (Body) в блоках if
    void generateBody(
     const Node* bodyNode,
     std::unordered_map<std::string, llvm::Value*>& localVars,
     const std::set<std::string>& param_names
 ) {

        auto body = dynamic_cast<const Body*>(bodyNode);
        if (!body) {
            llvm::errs() << "Error: Body is not of type Body.\n";
            return;
        }


        for (const auto& stmt : body->statements) {
            if (auto assignStmt = dynamic_cast<const Assignment*>(stmt.get())) {
                generateAssignment(assignStmt, localVars, param_names);
            }

            if (auto ifStmt = dynamic_cast<const IfStatement*>(stmt.get())) {
                generateIfStatement(ifStmt, localVars, param_names);
            }
        }
    }


    // Функция для генерации Expression (Relation { ( and | or | xor ) Relation })
   llvm::Value* generateExpression(
    const Expression* expr,
    std::unordered_map<std::string, llvm::Value*>& localVars,
    const std::set<std::string>& param_names
) {
    if (expr->relations.empty()) {
        llvm::errs() << "Error: Expression contains no Relations.\n";
        return nullptr;
    }

    llvm::Value* result = generateRelation(dynamic_cast<Relation*>(expr->relations[0].get()), localVars, param_names);
    if (!result) {
        llvm::errs() << "Error: Failed to generate first Relation in Expression.\n";
        return nullptr;
    }

    // Итерируемся по оставшимся Relations и операторам
    for (size_t i = 1; i < expr->relations.size(); ++i) {
        if (i - 1 >= expr->operators.size()) {
            llvm::errs() << "Error: Not enough operators in Expression.\n";
            break;
        }

        std::string op = expr->operators[i - 1];
        llvm::Value* rhs = generateRelation(dynamic_cast<Relation*>(expr->relations[i].get()), localVars, param_names);
        if (!rhs) {
            llvm::errs() << "Error: Failed to generate Relation at index " << i << " in Expression.\n";
            return nullptr;
        }

        // Приведение к типу i1
        if (!result->getType()->isIntegerTy(1)) {
            result = builder.CreateICmpNE(result, llvm::ConstantInt::get(result->getType(), 0), "cond");
        }
        if (!rhs->getType()->isIntegerTy(1)) {
            rhs = builder.CreateICmpNE(rhs, llvm::ConstantInt::get(rhs->getType(), 0), "cond");
        }

        // Применяем операторы
        if (op == "and") {
            result = builder.CreateAnd(result, rhs, "andtmp_expression");
        } else if (op == "or") {
            result = builder.CreateOr(result, rhs, "ortmp_expression");
        } else if (op == "xor") {
            result = builder.CreateXor(result, rhs, "xortmp_expression");
        } else {
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
    // Обработка узла литерала
    if (auto literal = dynamic_cast<LiteralPrimary*>(primary->child.get())) {
        switch (literal->type) {
            case LiteralPrimary::LiteralType::Integer: {
                llvm::outs() << "Generating integer literal: " << literal->intValue << "\n";
                return llvm::ConstantInt::get(builder.getInt32Ty(), literal->intValue, true);
            }
            case LiteralPrimary::LiteralType::Real: {
                llvm::outs() << "Generating real literal: " << literal->realValue << "\n";
                return llvm::ConstantFP::get(builder.getDoubleTy(), literal->realValue);
            }
            case LiteralPrimary::LiteralType::Boolean: {
                llvm::outs() << "Generating boolean literal: " << (literal->boolValue ? "true" : "false") << "\n";
                return llvm::ConstantInt::get(builder.getInt1Ty(), literal->boolValue);
            }
            default:
                llvm::errs() << "Error: Unsupported literal type.\n";
                return nullptr;
        }
    }

    // Обработка модифицируемого узла
    if (auto modifiable = dynamic_cast<ModifiablePrimary*>(primary->child.get())) {
        auto identifier = dynamic_cast<Identifier*>(modifiable->identifier.get());
        if (identifier) {
            // Очистка имени идентификатора
            std::string name = sanitizeName(identifier->name);
            llvm::outs() << "Generating Primary for identifier: " << name << "\n";

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
                    llvm::Value* loadedVal = builder.CreateLoad(varType, var, name);
                    llvm::outs() << "Loaded value from variable: " << name << "\n";
                    return loadedVal;
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
                        llvm::outs() << "Accessing parameter: " << name << "\n";
                        return &arg; // Возвращаем аргумент напрямую
                    }
                }
                llvm::errs() << "Error: Parameter " << name << " not found in function arguments.\n";
                return nullptr;
            }
            else {
                llvm::errs() << "Error: Undefined variable or identifier '" << name << "'.\n";
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
    void declarePrintFunctions() {
       // PrintInt
       llvm::FunctionType* printIntType = llvm::FunctionType::get(
           builder.getVoidTy(), {builder.getInt32Ty()}, false);
       llvm::Function::Create(printIntType, llvm::Function::ExternalLinkage, "PrintInt", module.get());

       // PrintReal
       llvm::FunctionType* printRealType = llvm::FunctionType::get(
           builder.getVoidTy(), {builder.getDoubleTy()}, false);
       llvm::Function::Create(printRealType, llvm::Function::ExternalLinkage, "PrintReal", module.get());

       // PrintBoolean
       llvm::FunctionType* printBooleanType = llvm::FunctionType::get(
           builder.getVoidTy(), {builder.getInt1Ty()}, false);
       llvm::Function::Create(printBooleanType, llvm::Function::ExternalLinkage, "PrintBoolean", module.get());
   }

};

#endif // IRGENERATOR_H
