#include <stdio.h>
#include <inttypes.h>
#include "isa.h"

int isaDisassembleIllegal(uint32_t instr, char *out, size_t bufsz);
int isaDisassembleOP(uint32_t instr, char *out, size_t bufsz);
int isaDisassembleOPIMM(uint32_t instr, char *out, size_t bufsz);
int isaDisassembleLOAD(uint32_t instr, char *out, size_t bufsz);
int isaDisassembleSTORE(uint32_t instr, char *out, size_t bufsz);
int isaDisassembleBRANCH(uint32_t instr, char *out, size_t bufsz);
int isaDisassembleSYSTEM(uint32_t instr, char *out, size_t bufsz);


int isaDisassemble(uint32_t instr, char *out, size_t bufsz)
{
  t_cpuRegID rd, rs1;
  int32_t imm;

  switch (ISA_INST_OPCODE(instr)) {
    case ISA_INST_OPCODE_OP:
      return isaDisassembleOP(instr, out, bufsz);
    case ISA_INST_OPCODE_OPIMM:
      return isaDisassembleOPIMM(instr, out, bufsz);
    case ISA_INST_OPCODE_LOAD:
      return isaDisassembleLOAD(instr, out, bufsz);
    case ISA_INST_OPCODE_STORE:
      return isaDisassembleSTORE(instr, out, bufsz);
    case ISA_INST_OPCODE_BRANCH:
      return isaDisassembleBRANCH(instr, out, bufsz);
    case ISA_INST_OPCODE_JAL:
      rd = ISA_INST_RD(instr);
      imm = (int32_t)ISA_INST_J_IMM21_SEXT(instr);
      return snprintf(out, bufsz, "JAL x%d, *%+" PRId32, rd, imm);
    case ISA_INST_OPCODE_JALR:
      if (ISA_INST_FUNCT3(instr) != 0)
        return isaDisassembleIllegal(instr, out, bufsz);
      rd = ISA_INST_RD(instr);
      rs1 = ISA_INST_RS1(instr);
      imm = (int32_t)ISA_INST_I_IMM12_SEXT(instr);
      return snprintf(out, bufsz, "JALR x%d, %" PRId32 "(x%d)", rd, imm, rs1);
    case ISA_INST_OPCODE_LUI:
      rd = ISA_INST_RD(instr);
      imm = (int32_t)ISA_INST_U_IMM20(instr);
      return snprintf(out, bufsz, "LUI x%d, 0x%05" PRIx32, rd, imm);
    case ISA_INST_OPCODE_AUIPC:
      rd = ISA_INST_RD(instr);
      imm = (int32_t)ISA_INST_U_IMM20(instr);
      return snprintf(out, bufsz, "AUIPC x%d, 0x%05" PRIx32, rd, imm);
    case ISA_INST_OPCODE_SYSTEM:
      return isaDisassembleSYSTEM(instr, out, bufsz);
  }

  return isaDisassembleIllegal(instr, out, bufsz);
}

int isaDisassembleIllegal(uint32_t instr, char *out, size_t bufsz)
{
  return snprintf(out, bufsz, "<illegal>");
}

int isaDisassembleOP(uint32_t instr, char *out, size_t bufsz)
{
  t_cpuRegID rd = ISA_INST_RD(instr);
  t_cpuRegID rs1 = ISA_INST_RS1(instr);
  t_cpuRegID rs2 = ISA_INST_RS2(instr);
  static const char *mnems00[] = {
      "ADD", "SLL", "SLT", "SLTU", "XOR", "SRL", "OR", "AND"};
  static const char *mnems20[] = {
      "SUB", NULL, NULL, NULL, NULL, "SRA", NULL, NULL};
  static const char *mnems01[] = {
      "MUL", "MULH", "MULHSU", "MULHU", "DIV", "DIVU", "REM", "REMU"};

  static const char *mnem;
  if (ISA_INST_FUNCT7(instr) == 0x00) {
    mnem = mnems00[ISA_INST_FUNCT3(instr)];
  } else if (ISA_INST_FUNCT7(instr) == 0x20) {
    mnem = mnems20[ISA_INST_FUNCT3(instr)];
  } else if (ISA_INST_FUNCT7(instr) == 0x01) {
    mnem = mnems01[ISA_INST_FUNCT3(instr)];
  } else {
    return isaDisassembleIllegal(instr, out, bufsz);
  }

  if (mnem != NULL)
    return snprintf(out, bufsz, "%s x%d, x%d, x%d", mnem, rd, rs1, rs2);
  return isaDisassembleIllegal(instr, out, bufsz);
}

int isaDisassembleOPIMM(uint32_t instr, char *out, size_t bufsz)
{
  t_cpuRegID rd = ISA_INST_RD(instr);
  t_cpuRegID rs1 = ISA_INST_RS1(instr);
  int32_t imm = (int32_t)ISA_INST_I_IMM12_SEXT(instr);
  static const char *mnems[] = {
      "ADDI", "SLLI", "SLTI", "SLTIU", "XORI", "SRLI", "ORI", "ANDI"};

  const char *mnem = mnems[ISA_INST_FUNCT3(instr)];

  if (ISA_INST_FUNCT3(instr) == 3) {
    imm &= 0x7FF;
  } else if (ISA_INST_FUNCT3(instr) == 1) {
    if (ISA_INST_FUNCT7(instr) != 0)
      return isaDisassembleIllegal(instr, out, bufsz);
  } else if (ISA_INST_FUNCT3(instr) == 5) {
    if (ISA_INST_FUNCT7(instr) == 0x20) {
      mnem = "SRAI";
      imm &= 0x1F;
    } else if (ISA_INST_FUNCT7(instr) != 0)
      return isaDisassembleIllegal(instr, out, bufsz);
  }

  return snprintf(out, bufsz, "%s x%d, x%d, %" PRId32, mnem, rd, rs1, imm);
}

int isaDisassembleLOAD(uint32_t instr, char *out, size_t bufsz)
{
  t_cpuRegID rd = ISA_INST_RD(instr);
  t_cpuRegID rs1 = ISA_INST_RS1(instr);
  int32_t imm = (int32_t)ISA_INST_I_IMM12_SEXT(instr);
  static const char *mnems[] = {
      "LB", "LH", "LW", NULL, "LBU", "LHU", NULL, NULL};

  const char *mnem = mnems[ISA_INST_FUNCT3(instr)];
  if (mnem == NULL)
    return isaDisassembleIllegal(instr, out, bufsz);

  return snprintf(out, bufsz, "%s x%d, %" PRId32 "(x%d)", mnem, rd, imm, rs1);
}

int isaDisassembleSTORE(uint32_t instr, char *out, size_t bufsz)
{
  t_cpuRegID rs1 = ISA_INST_RS1(instr);
  t_cpuRegID rs2 = ISA_INST_RS2(instr);
  int32_t imm = (int32_t)ISA_INST_S_IMM12_SEXT(instr);
  static const char *mnems[] = {
      "SB", "SH", "SW", NULL, NULL, NULL, NULL, NULL};

  const char *mnem = mnems[ISA_INST_FUNCT3(instr)];
  if (mnem == NULL)
    return isaDisassembleIllegal(instr, out, bufsz);

  return snprintf(out, bufsz, "%s x%d, %" PRId32 "(x%d)", mnem, rs2, imm, rs1);
}

int isaDisassembleBRANCH(uint32_t instr, char *out, size_t bufsz)
{
  t_cpuRegID rs1 = ISA_INST_RS1(instr);
  t_cpuRegID rs2 = ISA_INST_RS2(instr);
  int32_t imm = (int32_t)ISA_INST_B_IMM13_SEXT(instr);
  static const char *mnems[] = {
      "BEQ", "BNE", NULL, NULL, "BLT", "BGE", "BLTU", "BGEU"};

  const char *mnem = mnems[ISA_INST_FUNCT3(instr)];
  if (mnem == NULL)
    return isaDisassembleIllegal(instr, out, bufsz);

  return snprintf(out, bufsz, "%s x%d, x%d, *%+" PRId32, mnem, rs1, rs2, imm);
}

int isaDisassembleSYSTEM(uint32_t instr, char *out, size_t bufsz)
{
  if (ISA_INST_FUNCT3(instr) != 0)
    return isaDisassembleIllegal(instr, out, bufsz);
  if (ISA_INST_I_IMM12(instr) == 0)
    return snprintf(out, bufsz, "ECALL");
  if (ISA_INST_I_IMM12(instr) == 1)
    return snprintf(out, bufsz, "EBREAK");
  return isaDisassembleIllegal(instr, out, bufsz);
}
