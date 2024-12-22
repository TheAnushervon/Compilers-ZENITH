# ZENITH Compiler

**ZENITH** is a custom compiler written in C++ that leverages the power of the LLVM platform (version 18.1.8). Designed to translate high-level code into optimized machine code, ZENITH provides a robust foundation for compiling various programming constructs and routines.

## Features

- **Arithmetic Operations:** Supports basic arithmetic operations and variable assignments.
- **Control Structures:** Implements loops and conditional statements.
- **Function Definitions:** Allows defining and invoking multiple routines with parameters.
- **Array Handling:** Manages arrays and their manipulations.
- **Integration with LLVM:** Utilizes LLVM for code optimization and compilation.

## Example

Below is an example of a program written for the ZENITH compiler, demonstrating its capabilities:

```plaintext
routine factor(a: integer, b: integer) : integer is

    var m :integer is 1
    for i in a..b loop
         m := m * i
    end
    return m
end

routine sum(a: integer, b: integer) : integer is
    var s : integer is a + b
    return s
end

routine double_sum(a: integer, b:integer) : integer is
    var s: integer is sum(a, b)
    return s
end

routine sum_function(a:integer, b: integer) : integer is
    var s : integer is a
    s := sum(a, b)
    return s
end

routine first_more_second(a: integer, b: integer) : boolean is
    var ans : boolean is a > b
    return ans
end

routine comparator(a: integer, b: integer ) : integer is
    var max : integer is b
    if (a > b) then
        max := a
    end
    return max
end

routine sum_arr(a: integer, b: integer) : integer is
    var arr : array[10] integer
    var x : integer is 0
    for i in 0..9 loop
        arr[i] := i * i
    end
    x := arr[4]
    return x
end

var first : integer is 5
var second : integer is 10
var ans : integer is 99
print ( sum_function(first, second) )
print( comparator(first, second))
print(first_more_second(second,first))
print(ans)
var f : integer is 4
var s : integer is 8
print(factor(f,s))

var k : integer is 7

print(double_sum(f, s))

print(sum_arr(k, k))   
```


## Installation
### Prerequisites
* LLVM: Ensure that LLVM (version 18.1.8) is installed on your system.
* Clang: Required for compiling the compiler.
You can install LLVM and Clang on Arch Linux using pacman:

```bash
sudo pacman -S llvm clang
```
## Compilation
To compile the ZENITH compiler, navigate to the root directory of the project and execute the compile.sh script:

```bash
./compile.sh
```
## Usage
After successfully compiling the compiler, you can run it using the run.sh script:

```bash
./run.sh
```





