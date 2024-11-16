# Компилятор
CXX = clang++

# Пути к заголовкам и библиотекам
LLVM_INCLUDE = C:/LLVM/clang+llvm-19.1.0-x86_64-pc-windows-msvc/include
LLVM_LIB = C:/LLVM/clang+llvm-19.1.0-x86_64-pc-windows-msvc/lib

# Добавляем include paths для MinGW
MINGW_INCLUDE = C:/MinGW/include
MINGW_INCLUDE_CXX = C:/MinGW/lib/gcc/mingw32/8.1.0/include/c++

# Флаги компиляции
CXXFLAGS = -std=c++17 -I$(LLVM_INCLUDE) -I$(MINGW_INCLUDE) -I$(MINGW_INCLUDE_CXX) -g -Wall -Werror -D_GLIBCXX_USE_CXX11_ABI=0

# Флаги линковки
LDFLAGS = -L$(LLVM_LIB) -lLLVMCore -lLLVMSupport -lLLVMIRReader -ladvapi32

# Исходные файлы
SRCS = main.cpp ir_generator.cpp

# Целевой файл
TARGET = main

# Сборка
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) $(LDFLAGS) -o $(TARGET)

# Очистка
clean:
	rm -f $(TARGET)
