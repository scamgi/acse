/// @file target_info.h
/// @brief Properties of the target machine

#ifndef TARGET_INFO_H
#define TARGET_INFO_H

#include <stdbool.h>
#include <stdint.h>
#include "list.h"
#include "program.h"

/**
 * @defgroup target Target-specific Code
 * @brief Definitions and functions to support the compilation target
 *
 * These functions and definitions allow the compiler to appropriately handle
 * the target-specific semantics of some special instructions, included the
 * instructions that are available to the ACSE intermediate representation but
 * are not part of the target assembly language itself.
 * @{
 */

/// Signed data type with the same size of a target register.
typedef int32_t t_regInt;

/// Name of the target architecture.
#define TARGET_NAME "RISC-V_RV32IM"

/// Number of bytes for each memory address.
#define TARGET_PTR_GRANULARITY 1

/// Defined to 'true' if the target has a 'zero' register whose value is always
/// the constant zero.
#define TARGET_REG_ZERO_IS_CONST true

/// Number of general-purpose registers usable by the register allocator.
#define NUM_GP_REGS 23

/// Number of registers available for spilled temporaries. Should be equal to
/// the maximum number of unique register operands in a single instruction.
#define NUM_SPILL_REGS 3

/// Machine register names.
enum {
  REG_ZERO = 0,
  REG_RA,
  REG_SP,
  REG_GP,
  REG_TP,
  REG_T0,
  REG_T1,
  REG_T2,
  REG_S0,
  REG_S1,
  REG_A0,
  REG_A1,
  REG_A2,
  REG_A3,
  REG_A4,
  REG_A5,
  REG_A6,
  REG_A7,
  REG_S2,
  REG_S3,
  REG_S4,
  REG_S5,
  REG_S6,
  REG_S7,
  REG_S8,
  REG_S9,
  REG_S10,
  REG_S11,
  REG_T3,
  REG_T4,
  REG_T5,
  REG_T6,
  NUM_REGISTERS
};

/** Opcode IDs used internally by ACSE to identify the various instructions.
 *
 * Some opcodes are labeled "pseudo" if they need to be transformed to a
 * sequence of other non-pseudo opcodes before emitting the output assembly
 * file. Other pseudo opcodes are handled by the assembler, those are not
 * marked as "pseudo" here. */
enum {
  // Arithmetic
  OPC_ADD,   // rd = rs1 +  rs2
  OPC_SUB,   // rd = rs1 -  rs2
  OPC_AND,   // rd = rs1 &  rs2
  OPC_OR,    // rd = rs1 |  rs2
  OPC_XOR,   // rd = rs1 ^  rs2
  OPC_MUL,   // rd = rs1 *  rs2
  OPC_DIV,   // rd = rs1 /  rs2
  OPC_REM,   // rd = rs1 %  rs2
  OPC_SLL,   // rd = rs1 << rs2
  OPC_SRL,   // rd = rs1 >> rs2            (logical)
  OPC_SRA,   // rd = rs1 >> rs2            (arithmetic)

  // Arithmetic with immediate
  OPC_ADDI,  // rd = rs1 +  imm
  OPC_SUBI,  // rd = rs1 -  imm                         (pseudo)
  OPC_ANDI,  // rd = rs1 &  imm
  OPC_ORI,   // rd = rs1 |  imm
  OPC_XORI,  // rd = rs1 ^  imm
  OPC_MULI,  // rd = rs1 *  imm                         (pseudo)
  OPC_DIVI,  // rd = rs1 /  imm                         (pseudo)
  OPC_REMI,  // rd = rs1 %  imm                         (pseudo)
  OPC_SLLI,  // rd = rs1 << imm
  OPC_SRLI,  // rd = rs1 >> imm            (logical)
  OPC_SRAI,  // rd = rs1 >> imm            (arithmetic)

  // Comparison
  OPC_SEQ,   // rd = rs1 == rs2                         (pseudo)
  OPC_SNE,   // rd = rs1 != rs2                         (pseudo)
  OPC_SLT,   // rd = rs1 <  rs2            (signed)
  OPC_SLTU,  // rd = rs1 <  rs2            (unsigned)
  OPC_SGE,   // rd = rs1 >= rs2            (signed)     (pseudo)
  OPC_SGEU,  // rd = rs1 >= rs2            (unsigned)   (pseudo)
  OPC_SGT,   // rd = rs1 >  rs2            (signed)     (pseudo)
  OPC_SGTU,  // rd = rs1 >  rs2            (unsigned)   (pseudo)
  OPC_SLE,   // rd = rs1 <= rs2            (signed)     (pseudo)
  OPC_SLEU,  // rd = rs1 <= rs2            (unsigned)   (pseudo)

  // Comparison with immediate
  OPC_SEQI,  // rd = rs1 == imm                         (pseudo)
  OPC_SNEI,  // rd = rs1 != imm                         (pseudo)
  OPC_SLTI,  // rd = rs1 <  imm            (signed)
  OPC_SLTIU, // rd = rs1 <  imm            (unsigned)
  OPC_SGEI,  // rd = rs1 >= imm            (signed)     (pseudo)
  OPC_SGEIU, // rd = rs1 >= imm            (unsigned)   (pseudo)
  OPC_SGTI,  // rd = rs1 >  imm            (signed)     (pseudo)
  OPC_SGTIU, // rd = rs1 >  imm            (unsigned)   (pseudo)
  OPC_SLEI,  // rd = rs1 <= imm            (signed)     (pseudo)
  OPC_SLEIU, // rd = rs1 <= imm            (unsigned)   (pseudo)

  // Jump/Branch
  OPC_J,     // goto addr;
  OPC_BEQ,   // if (rs1 == rs2) goto addr;
  OPC_BNE,   // if (rs1 == rs2) goto addr;
  OPC_BLT,   // if (rs1 <  rs2) goto addr; (signed)
  OPC_BLTU,  // if (rs1 <  rs2) goto addr; (unsigned)
  OPC_BGE,   // if (rs1 >= rs2) goto addr; (signed)
  OPC_BGEU,  // if (rs1 >= rs2) goto addr; (unsigned)
  OPC_BGT,   // if (rs1 >  rs2) goto addr; (signed)     (pseudo)
  OPC_BGTU,  // if (rs1 >  rs2) goto addr; (unsigned)   (pseudo)
  OPC_BLE,   // if (rs1 <= rs2) goto addr; (signed)     (pseudo)
  OPC_BLEU,  // if (rs1 <= rs2) goto addr; (unsigned)   (pseudo)

  // Load/Store
  OPC_LI,    // rd = imm
  OPC_LA,    // rd = addr
  OPC_LW,    // rd = *(int *)(immediate + rs1)
  OPC_SW,    // *(int *)(immediate + rs1) = rs2
  OPC_LW_G,  // rd = *(int *)label                      (pseudo)
  OPC_SW_G,  // *(int *)label = rs2                     (pseudo)

  // Other
  OPC_NOP,
  OPC_ECALL,
  OPC_EBREAK,

  // Syscall opcodes
  OPC_CALL_EXIT_0,
  OPC_CALL_READ_INT,
  OPC_CALL_PRINT_INT,
  OPC_CALL_PRINT_CHAR
};


/** Tests for jump/branch instructions.
 *  @param instr The instruction to be examined.
 *  @returns true if the instruction is a jump/branch. */
bool isJumpInstruction(t_instruction *instr);

/** Tests for unconditional jump instructions.
 *  @param instr The instruction to be examined.
 *  @returns true if the instruction is an unconditional jump. */
bool isUnconditionalJump(t_instruction *instr);

/** Tests if the instruction causes the program to exit.
 *  @param instr The instruction to be examined.
 *  @returns true if the instruction exits the program. */
bool isExitInstruction(t_instruction *instr);

/** Tests if the instruction is used to perform a function or system call.
 *  @param instr The instruction to be examined.
 *  @returns true if the instruction is a call. */
bool isCallInstruction(t_instruction *instr);


/** Retrieves a register ID suitable for spill operations. The maximum index
 *  is always bounded by NUM_SPILL_REGS.
 *  @param i The index of the spill register to return, between 0 and
 *           NUM_SPILL_REGS (excluded).
 *  @returns The register ID of a machine register suitable for spilling. */
t_regID getSpillMachineRegister(int i);

/** Retrieves the list of register IDs available for the register allocator,
 *  sorted in order of priority.
 *  @returns The list of register IDs stored inline as integers. */
t_listNode *getListOfGenPurposeMachineRegisters(void);

/** Retrieves the complete list of machine registers exception made for ones
 *  with a fixed value.
 *  @returns The list of register IDs stored inline as integers. */
t_listNode *getListOfMachineRegisters(void);

/** Retrieves the list of register IDs that can be modified by a given function
 *  call instruction, except for input and output parameters.
 *  @returns The list of register IDs stored inline as integers. */
t_listNode *getListOfCallerSaveMachineRegisters(void);

/**
 * @}
 */

#endif
