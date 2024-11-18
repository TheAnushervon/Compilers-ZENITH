#!/bin/bash

# Убедитесь, что исходные файлы и генерированный IR существуют
if [[ ! -f "print_functions.c" ]]; then
  echo "Error: print_functions.c not found!"
  exit 1
fi

if [[ ! -f "generated_code.ll" ]]; then
  echo "Error: generated_code.ll not found!"
  exit 1
fi

# Шаг 1: Компиляция print_functions.c в объектный файл
echo "Compiling print_functions.c..."
clang -c print_functions.c -o print_functions.o
if [ $? -ne 0 ]; then
  echo "Error: Compilation of print_functions.c failed!"
  exit 1
fi

# Шаг 2: Преобразование LLVM IR в объектный файл
echo "Assembling LLVM IR..."
llvm-as generated_code.ll -o generated_code.bc
if [ $? -ne 0 ]; then
  echo "Error: llvm-as failed!"
  exit 1
fi

# Шаг 3: Преобразование .bc в .o объектный файл
echo "Generating object file from LLVM BC..."
llc -filetype=obj generated_code.bc -o generated_code.o
if [ $? -ne 0 ]; then
  echo "Error: llc failed!"
  exit 1
fi

# Шаг 4: Линковка с объектным файлом print_functions.o
echo "Linking object files..."
clang generated_code.o print_functions.o -o my_program
if [ $? -ne 0 ]; then
  echo "Error: Linking failed!"
  exit 1
fi

# Шаг 5: Запуск программы
echo "Running the program..."
./my_program
if [ $? -ne 0 ]; then
  echo "Error: Execution failed!"
  exit 1
fi

echo "Program executed successfully!"
