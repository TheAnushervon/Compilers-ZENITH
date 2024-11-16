# Минимальная версия CMake
cmake_minimum_required(VERSION 3.16)

# Название проекта
project(Compilers-ZENITH)

# Установить стандарт C++
set(CMAKE_CXX_STANDARD 17)

# Указать путь к LLVM (в папке lib/cmake/llvm находится файл LLVMConfig.cmake)
set(LLVM_DIR "C:/LLVM/clang+llvm-19.1.0-x86_64-pc-windows-msvc/lib/cmake/llvm")

# Найти LLVM
find_package(LLVM REQUIRED CONFIG)

# Вывод сообщений о найденной LLVM (помогает в отладке)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Подключить директории с заголовками LLVM
include_directories(${LLVM_INCLUDE_DIRS})

# Подключить директории с библиотеками LLVM
link_directories(${LLVM_LIBRARY_DIRS})

# Установить флаги компиляции (добавляет определения, необходимые LLVM)
add_definitions(${LLVM_DEFINITIONS})

# Указать ваши исходные файлы (замените на ваши файлы)
add_executable(my_project main.cpp ir_generator.cpp)

# Связать LLVM с вашим проектом (подключение библиотек)
target_link_libraries(my_project PRIVATE LLVM)
