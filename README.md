# cave
**Cave** is a code compiler written in pure C, utilizing LLVM as its backend.
It is designed to compile the Cave language, identifiable by the `.cv` extension.

## Dependencies
- [LLVM](https://github.com/llvm/llvm-project) - Ensure LLVM binaries are
accessible in your system's PATH for successful compilation of the project.
The compiler is compatible with LLVM version 19 and above.

## Installation
1. Clone the repository to your local machine by executing the following command:
```bash
git clone https://github.com/SzAkos04/cave.git
```

2. Go inside the directory and build it with the following command:
```bash
cd cave
make release
```
This command will generate an executable inside the cave/build directory.
To use the program globally, it is recommended to add the build folder to path.

## Setup
Ensure that the LLVM libraries are properly linked by adding the following line
to your shell profile (e.g., .bashrc, .zshrc):
```bash
export LD_LIBRARY_PATH=$(llvm-config --libdir):$LD_LIBRARY_PATH
```

## Usage
To use the compiler, simply run
```bash
cave [file.cv]
```
This will create an executable
named `a.out`. Customization of the output executable name is not currently supported.

## Cave code example
Here's a sample snippet demonstrating the basic syntax of the `Cave` language:
```
const a: i32 = 0;
const b: i32 = a;

fn main() i32 {

    return b;
}
```

### Explanation
In this example: 

- `const a: i32 = 0;`: Declares a constant named `a` of type `i32` with the value of `0`.

- `const b: i32 = a;`: Declares a constant named `b` of type `i32` with the value of the value of the constant `a`, `0`

- `fn main() i32`: Defines the `main` function, which serves as the entry point to
the program. It is expeced to return a 32 bit integer (`i32`).

- `return b;`: Returns the value of `b` from the `main` function.
