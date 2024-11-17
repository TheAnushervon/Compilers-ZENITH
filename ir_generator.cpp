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
#include <algorithm> // Для std::remove_if
#include <set>

// Ваши заголовочные файлы
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
                llvm::errs() << "Error: routine is not a RoutineDeclaration.\n";
            }
        }
    }

    void generateRoutine(const RoutineDeclaration* routine) {
        // 1. Извлекаем из функции возвращаемый тип
        llvm::Type* returnType = builder.getVoidTy();
        auto type = dynamic_cast<Type*>(routine->returnType.get());
        if (auto primitiveType = dynamic_cast<PrimitiveType*>(type->child.get())) {
            returnType = mapType(type->child.get());
        }
        std::cout << "1 этап: ок\n";

        // 2. Извлекаем параметры
        std::vector<llvm::Type*> paramTypes;
        std::set<std::string> param_names; // Для использования в parseParameters
        parseParameters(routine, paramTypes, param_names);
        std::cout << "2 этап: ок\n";
        module->print(llvm::outs(), nullptr);

        // 3. Создание типа функции
        llvm::FunctionType* funcType = llvm::FunctionType::get(returnType, paramTypes, false);
        std::cout << "3 этап: ок\n";
        module->print(llvm::outs(), nullptr);

        // 4. Создание функции
        auto identifier = dynamic_cast<Identifier*>(routine->identifier.get());
        if (!identifier) {
            llvm::errs() << "Error: Routine has no Identifier.\n";
            return;
        }

        // Очистка имени идентификатора функции
        std::string funcName = identifier->name;
        funcName.erase(std::remove_if(funcName.begin(), funcName.end(), [](char c) { return !std::isalnum(c); }), funcName.end());

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
                        std::string name = paramName->name;
                        name.erase(std::remove_if(name.begin(), name.end(), [](char c) { return !std::isalnum(c); }), name.end());

                        arg.setName(name); // Устанавливаем имя параметра
                        param_names.insert(name);
                    }
                }
                ++idx;
            }
        }
        module->print(llvm::outs(), nullptr);

        // 5. Создаём базовый блок
        llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(context, "entry", function);
        builder.SetInsertPoint(entryBlock);
        std::cout << "5 этап: ок\n";
        module->print(llvm::outs(), nullptr);

        // 6. Обработка переменных и выражений
        // Создание символической таблицы для локальных переменных
        std::unordered_map<std::string, llvm::Value*> localVars; // Объявление локальных переменных

        if (routine->body) {
            auto body = dynamic_cast<Body*>(routine->body.get());
            if (!body) {
                llvm::errs() << "Error: Body не является типом Body\n";
                return;
            }
            for (const auto& stmt : body->statements) {
                // Генерация выражения для переменной
                if (auto simDec = dynamic_cast<SimpleDeclaration*>(stmt.get())) {
                    if (auto varDecl = dynamic_cast<VariableDeclaration*>(simDec->child.get())) {
                        auto identifier = dynamic_cast<Identifier*>(varDecl->identifier.get());
                        if (!identifier) {
                            llvm::errs() << "Error: VariableDeclaration без Identifier\n";
                            continue;
                        }

                        // Очистка имени идентификатора переменной
                        std::string varName = identifier->name;
                        varName.erase(std::remove_if(varName.begin(), varName.end(), [](char c) { return !std::isalnum(c); }), varName.end());

                        auto type = dynamic_cast<Type*>(varDecl->type.get());
                        if (!type) {
                            llvm::errs() << "Error: VariableDeclaration без Type\n";
                            continue;
                        }

                        llvm::Type* varType = mapType(type->child.get());
                        if (!varType) {
                            llvm::errs() << "Error: Невозможно сопоставить тип переменной " << varName << "\n";
                            continue;
                        }

                        llvm::Value* alloca = builder.CreateAlloca(varType, nullptr, varName);
                        localVars[varName] = alloca;

                        if (auto expression = dynamic_cast<Expression*>(varDecl->expression.get())) {
                            if (!expression->relations.empty()) {
                                llvm::Value* exprValue = generateArithmeticExpression(dynamic_cast<Relation*>(expression->relations.front().get()), localVars, param_names);
                                if (exprValue) {
                                    builder.CreateStore(exprValue, alloca);
                                } else {
                                    llvm::errs() << "Error: Failed to generate expression for variable " << varName << "\n";
                                }
                            } else {
                                llvm::errs() << "Error: VariableDeclaration expression не содержит relations\n";
                            }
                        }
                    }
                }

                // Генерация return
                if (auto returnStmt = dynamic_cast<ReturnType*>(stmt.get())) {
                    auto expression = dynamic_cast<Expression*>(returnStmt->type.get());
                    if (expression && !expression->relations.empty()) {
                        auto relation = dynamic_cast<Relation*>(expression->relations.front().get());
                        if (!relation) {
                            llvm::errs() << "Error: Relation в return выражении не обнаружен\n";
                        }
                        llvm::Value* returnValue = generateArithmeticExpression(relation, localVars, param_names);
                        if (returnValue) {
                            builder.CreateRet(returnValue); // Возвращаем вычисленное значение
                        } else {
                            llvm::errs() << "Error: Failed to generate return value\n";
                        }
                    } else {
                        llvm::errs() << "Error: Invalid or empty return expression\n";
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

    // Функция для генерации арифметических выражений (например, a + b)
    llvm::Value* generateArithmeticExpression(
        const Relation* relation,
        std::unordered_map<std::string, llvm::Value*>& localVars,
        std::set<std::string>& param_names)
    {
        if (relation->simples.empty()) {
            llvm::errs() << "Error: Relation содержит пустые Simples.\n";
            return nullptr;
        }

        // Генерация значения первого Simple
        llvm::Value* result = generateSimple(dynamic_cast<Simple*>(relation->simples[0].get()), localVars, param_names);
        if (!result) {
            llvm::errs() << "Error: Failed to generate first Simple in Relation.\n";
            return nullptr;
        }

        // Итерируемся по оставшимся Simples и операторам Relation (например, логические операторы)
        for (size_t i = 1; i < relation->simples.size(); ++i) {
            if (i-1 >= relation->operators.size()) {
                llvm::errs() << "Error: Недостаточно операторов в Relation.\n";
                break;
            }

            std::string op = relation->operators[i-1];
            llvm::Value* rhs = generateSimple(dynamic_cast<Simple*>(relation->simples[i].get()), localVars, param_names);
            if (!rhs) {
                llvm::errs() << "Error: Failed to generate Simple at index " << i << " in Relation.\n";
                return nullptr;
            }

            std::cout << "Processing Relation operator: " << op << "\n";

            // Обработка арифметических операторов
            if (op == "+") {
                std::cout << "Processing '+' operator\n";
                result = builder.CreateAdd(result, rhs, "addtmp_relation");
            }
            else if (op == "-") {
                std::cout << "Processing '-' operator\n";
                result = builder.CreateSub(result, rhs, "subtmp_relation");
            }
            // Пример обработки логических операторов (если есть)
            else if (op == "and") {
                std::cout << "Processing 'and' operator\n";
                result = builder.CreateAnd(result, rhs, "andtmp_relation");
            }
            else if (op == "or") {
                std::cout << "Processing 'or' operator\n";
                result = builder.CreateOr(result, rhs, "ortmp_relation");
            }
            else {
                llvm::errs() << "Error: Unsupported Relation operator '" << op << "'.\n";
                return nullptr;
            }
        }

        return result;
    }

    // Функция для генерации узла Simple (например, a + b)
    llvm::Value* generateSimple(
        const Simple* simple,
        std::unordered_map<std::string, llvm::Value*>& localVars,
        std::set<std::string>& param_names)
    {
        if (simple->factors.empty()) {
            llvm::errs() << "Error: Simple содержит пустые Factors.\n";
            return nullptr;
        }

        // Генерация значения первого Factor
        llvm::Value* result = generateFactor(dynamic_cast<Factor*>(simple->factors[0].get()), localVars, param_names);
        if (!result) {
            llvm::errs() << "Error: Failed to generate first Factor in Simple.\n";
            return nullptr;
        }

        // Итерируемся по оставшимся Factors и операторам
        for (size_t i = 1; i < simple->factors.size(); ++i) {
            if (i-1 >= simple->operators.size()) {
                llvm::errs() << "Error: Недостаточно операторов в Simple.\n";
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
                std::cout << "Processing '+' operator\n";
                result = builder.CreateAdd(result, rhs, "addtmp_simple");
            }
            else if (op == "-") {
                std::cout << "Processing '-' operator\n";
                result = builder.CreateSub(result, rhs, "subtmp_simple");
            }
            else {
                llvm::errs() << "Error: Unsupported Simple operator '" << op << "'.\n";
                return nullptr;
            }
        }

        return result;
    }

    // Функция для генерации узла Factor
    llvm::Value* generateFactor(
        const Factor* factor,
        std::unordered_map<std::string, llvm::Value*>& localVars,
        std::set<std::string>& param_names)
    {
        llvm::Value* result = nullptr;

        // Генерация значения первого Summand
        auto summandIt = factor->summands.begin();
        if (summandIt != factor->summands.end()) {
            result = generateSummand(dynamic_cast<Summand*>(summandIt->get()), localVars, param_names);
        }

        // Итерируемся по оставшимся Summands и операторам
        for (size_t i = 0; i < factor->operators.size(); ++i) {
            if (i + 1 >= factor->summands.size()) {
                llvm::errs() << "Error: Недостаточно summands для оператора " << factor->operators[i] << "\n";
                break;
            }
            auto op = factor->operators[i];
            auto rhs = generateSummand(dynamic_cast<Summand*>(factor->summands[i + 1].get()), localVars, param_names);

            if (op == "*") {
                result = builder.CreateMul(result, rhs, "multmp");
            }
            else if (op == "/") {
                result = builder.CreateSDiv(result, rhs, "divtmp");
            }
        }

        return result;
    }

    // Функция для генерации узла Summand
    llvm::Value* generateSummand(
        const Summand* summand,
        std::unordered_map<std::string, llvm::Value*>& localVars,
        std::set<std::string>& param_names)
    {
        if (auto primary = dynamic_cast<Primary*>(summand->child.get())) {
            return generatePrimary(primary, localVars, param_names);
        }
        llvm::errs() << "Error: Summand не содержит Primary\n";
        return nullptr;
    }

    // Функция для генерации узла Primary
    llvm::Value* generatePrimary(
        const Primary* primary,
        std::unordered_map<std::string, llvm::Value*>& localVars,
        std::set<std::string>& param_names)
    {
        if (auto modifiable = dynamic_cast<ModifiablePrimary*>(primary->child.get())) {
            auto identifier = dynamic_cast<Identifier*>(modifiable->identifier.get());
            if (identifier) {
                // Удаляем лишние символы из имени идентификатора
                std::string name = identifier->name;
                name.erase(std::remove_if(name.begin(), name.end(), [](char c) { return !std::isalnum(c); }), name.end());

                llvm::Value* var = nullptr;
                // Проверяем, является ли переменная локальной
                auto it = localVars.find(name);
                if (it != localVars.end()) {
                    var = it->second;
                    if (!var) {
                        llvm::errs() << "Error: Variable " << name << " in localVars is nullptr\n";
                        return nullptr;
                    }

                    if (auto allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
                        llvm::Type* varType = allocaInst->getAllocatedType();
                        return builder.CreateLoad(varType, var, name);
                    }
                    else {
                        llvm::errs() << "Error: Variable " << name << " is not an AllocaInst\n";
                        return nullptr;
                    }
                }
                // Проверяем, является ли переменная параметром функции
                else if (param_names.count(name) != 0) {
                    llvm::Function* function = builder.GetInsertBlock()->getParent();
                    for (auto& arg : function->args()) {
                        if (arg.getName() == name) {
                            return &arg; // Возвращаем аргумент напрямую
                        }
                    }
                    llvm::errs() << "Error: Parameter " << name << " not found in function arguments\n";
                    return nullptr;
                }
                else {
                    llvm::errs() << "Error: Variable " << name << " not found in localVars or parameters\n";
                    return nullptr;
                }
            }
        }
        llvm::errs() << "Error: Primary node processing failed\n";
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
                        llvm::errs() << "Error: ParameterDeclaration без Type или child\n";
                        continue;
                    }
                    llvm::Type* paramType = mapType(t->child.get());
                    if (!paramType) {
                        llvm::errs() << "Error: Невозможно сопоставить тип параметра\n";
                        continue;
                    }
                    paramTypes.push_back(paramType);
                    auto paramName = dynamic_cast<Identifier*>(paramDecl->identifier.get());
                    if (paramName) {
                        // Удаляем лишние символы из имени параметра
                        std::string name = paramName->name;
                        name.erase(std::remove_if(name.begin(), name.end(), [](char c) { return !std::isalnum(c); }), name.end());

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
            if (primitive->name == "real") return builder.getDoubleTy();
            if (primitive->name == "boolean") return builder.getInt1Ty();
        }
        return builder.getVoidTy(); // На случай, если тип не распознан
    }
};
