# ACSE: A Custom Compiler Toolchain

ACSE is a complete compiler toolchain for a custom-designed language that targets the **RISC-V RV32IM architecture**. This project provides all the necessary tools for development, including a compiler, an assembler, and a simulator.

---

## 🏗️ Overall Architecture

The ACSE toolchain consists of three main components that work together to take your source code from a high-level language to a runnable executable:

1.  **Compiler (`acse`):** Translates the custom language source code into RISC-V assembly code.
2.  **Assembler (`asrv32im`):** Converts the assembly code into a standard ELF executable file.
3.  **Simulator (`simrv32im`):** Executes and provides a debugging environment for the ELF file.

The typical workflow is:

Source Code -> [acse Compiler] -> Assembly (.asm) -> [asrv32im Assembler] -> ELF Executable -> [simrv32im Simulator]

---

## 🛠️ Components

### Compiler (`acse`)

The `acse` compiler is the heart of the toolchain. It processes the source code in several stages:

1.  **Parsing:** It performs lexical and syntactical analysis to build an internal representation of the code.
2.  **Intermediate Representation (IR):** The code is converted into an IR, which is then optimized and lowered towards machine-level instructions.
3.  **Register Allocation:** A linear scan algorithm allocates physical machine registers to variables and handles spilling to memory when necessary.
4.  **Code Generation:** Finally, it produces a `.asm` file containing the RISC-V assembly code.

### Assembler (`asrv32im`)

The `asrv32im` assembler takes the human-readable assembly code from the compiler and turns it into a machine-executable format.

- **Input:** A `.s` file containing RISC-V assembly instructions and directives (e.g., `.data`, `.text`, `.word`).
- **Output:** An ELF (Executable and Linkable Format) file, which is the final executable.

### Simulator (`simrv32im`)

The `simrv32im` simulator runs and debugs the ELF executables without needing physical hardware.

- **Execution:** It simulates the RV32IM instruction set to run the program.
- **Debugging:** It includes a full-featured debugger with support for breakpoints, single-stepping, and inspection of memory and registers (activated with the `-d` flag).
- **System Calls:** It provides a supervisor to handle system calls for I/O operations, such as printing to the console.

---

## 🎯 Target Architecture: RISC-V RV32IM

The entire toolchain is built to support the **RISC-V RV32IM** instruction set architecture.

- **RV32I:** The base 32-bit integer instruction set.
- **M Extension:** Adds instructions for integer multiplication and division.

The test files in this repository contain numerous examples of RV32IM instructions, such as `add`, `sub`, `mul`, `div`, `lw`, `sw`, `beq`, and `jal`, showcasing the capabilities of the toolchain.

---

## 🚀 How to Compile and Run LANCE Code

Here's how to compile and run your LANCE code using the ACSE toolchain:

### 1. Compile your LANCE code

Use the `acse` compiler to translate your LANCE source file (e.g., `my_program.lance`) into RISC-V assembly code.

```bash
./acse my_program.lance -o my_program.asm
```
