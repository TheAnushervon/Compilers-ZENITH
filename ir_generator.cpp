#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

#include"nodes/node.h"

class IRGenerator {
public:
    llvm::LLVMContext context;         // Контекст для IR
    llvm::IRBuilder<> builder;         // Утилита для построения инструкций
    std::unique_ptr<llvm::Module> module; // Хранилище IR

    IRGenerator() : builder(context) {
        module = std::make_unique<llvm::Module>("MyProgram", context);
    }
/**
    void generateProgram(const Program* program) {
        for (const auto& routine : program->routineDeclarations) {
            generateRoutine(dynamic_cast<RoutineDeclaration*>(routine.get()));
        }
    }

    void generateRoutine(const RoutineDeclaration* routine) {
        // Создаем тип функции
        llvm::FunctionType* funcType = createFunctionType(routine);
        llvm::Function* function = llvm::Function::Create(
            funcType, llvm::Function::ExternalLinkage,
            dynamic_cast<Identifier*>(routine->identifier.get())->name,
            module.get()
        );

        // Создаем базовый блок
        llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(context, "entry", function);
        builder.SetInsertPoint(entryBlock);

        // Генерация тела
        if (routine->body) {
            generateBody(dynamic_cast<Body*>(routine->body.get()), function);
        }

        builder.CreateRetVoid();
        llvm::verifyFunction(*function);
    }

    void generateBody(const Body* body, llvm::Function* function) {
        for (const auto& stmt : body->statements) {
            // Генерируем IR для каждого оператора
            // Например, поддержка переменных, условных операторов
        }
    }

    llvm::FunctionType* createFunctionType(const RoutineDeclaration* routine) {
        std::vector<llvm::Type*> paramTypes;
        if (routine->parameters) {
            for (const auto& param : dynamic_cast<Parameters*>(routine->parameters.get())->children) {
                auto paramDecl = dynamic_cast<ParameterDeclaration*>(param.get());
                auto paramType = mapType(paramDecl->type.get());
                paramTypes.push_back(paramType);
            }
        }

        llvm::Type* returnType = routine->returnType
            ? mapType(dynamic_cast<Type*>(routine->returnType.get()))
            : builder.getVoidTy();
        return llvm::FunctionType::get(returnType, paramTypes, false);
    }

    llvm::Type* mapType(const Node* typeNode) {
        if (auto primitive = dynamic_cast<PrimitiveType*>(typeNode)) {
            if (primitive->name == "integer") return builder.getInt32Ty();
            if (primitive->name == "real") return builder.getDoubleTy();
            if (primitive->name == "boolean") return builder.getInt1Ty();
        }
        return builder.getVoidTy();
    }

    void printIR() {
        module->print(llvm::outs(), nullptr);
    }*/

};
