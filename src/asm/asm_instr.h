#ifndef SYSYC_INSTRUCTIONS_H
#define SYSYC_INSTRUCTIONS_H

#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include "passes/module.h"

const int imm_16_max = 65535;
const int imm_12_max = 4095;
const int imm_8_max = 255;

namespace InstGen {

const std::string spaces = std::string(4, ' ');
const std::string newline = "\n";

const std::string reg_name[] = {"r0",  "r1", "r2", "r3", "r4",  "r5",
                                "r6",  "r7", "r8", "r9", "r10", "r11",
                                "r12", "sp", "lr", "pc"};

const int max_reg_id = 15;

const std::string vfpreg_name[] = {"s0",  "s1", "s2", "s3", "s4",  "s5", "s6",  "s7",
                                "s8", "s9", "s10", "s11", "s12",  "s13", "s14",  "s15",
                                "s16", "s17", "s18", "s19", "s20",  "s21", "s22",  "s23",
                                "s24", "s25", "s26", "s27",  "s28", "s29",  "s30", "s31"
                                };
const int max_vfpreg_id = 31;


enum CmpOp {
  EQ, // ==bx
  NE, // !=
  GT, // >
  GE, // >=
  LT, // <
  LE, // <=
  NOP
};

class Value {
public:
  virtual bool is_reg() const = 0;
  virtual bool is_constant() const = 0;
  virtual bool has_shift() const = 0;
  virtual std::string getName() const = 0;
};

class Reg : public Value {
  int id;
  bool is_fp_;

public:
  explicit Reg(int id, bool is_vfpreg) : id(id), is_fp_(is_vfpreg){};
  bool is_reg() const { return true; }
  bool is_constant() const { return false; }
  bool has_shift() const { return false; }
  int getID() const { return this->id; }
  bool is_fp() const { return this->is_fp_; }
  std::string getName() const {
    if (this->is_fp_)
      return vfpreg_name[id];

    return reg_name[id];
  }
  const bool operator<(const Reg &rhs) const { return this->id < rhs.id; }
  const bool operator==(const Reg &rhs) const { return this->id == rhs.id; }
  const bool operator!=(const Reg &rhs) const { return this->id != rhs.id; }
};


class RegShift : public Value {
public:
  enum ShiftType { lsl, lsr, asl, asr };

private:
  int id;
  int shift;
  ShiftType _t;

public:
  explicit RegShift(int id, int shift, ShiftType _t = ShiftType::lsl)
      : id(id), shift(shift), _t(_t) {
    if (id < 0 || id > max_reg_id) {
      std::cerr << "Invalid Reg ID!" << std::endl;
      abort();
    }
    if (shift < 0 || shift > 31) {
      std::cerr << "Invalid Reg shift!" << std::endl;
      abort();
    }
  }
  bool is_reg() const { return true; }
  bool is_constant() const { return false; }
  bool has_shift() const { return true; }
  int getID() const { return this->id; }
  int getShift() const { return this->shift; }
  std::string getName() const {
    std::string shift_str;
    switch (this->_t) {
    case ShiftType::lsl:
      shift_str = "lsl";
      break;
    case ShiftType::asl:
      shift_str = "asl";
      break;
    case ShiftType::lsr:
      shift_str = "lsr";
      break;
    case ShiftType::asr:
      shift_str = "asr";
      break;
    default:
      break;
    }
    return reg_name[id] + ", " + shift_str + " " + "#" +
           std::to_string(this->getShift());
  }
  const bool operator<(const RegShift &rhs) const { return this->id < rhs.id; }
  const bool operator==(const RegShift &rhs) const {
    return this->id == rhs.id;
  }
  const bool operator!=(const RegShift &rhs) const {
    return this->id != rhs.id;
  }
};

const Reg fp = Reg(12, false);
const Reg sp = Reg(13, false);
const Reg lr = Reg(14, false);
const Reg pc = Reg(15, false);

class Addr {
  Reg reg;
  int offset;

public:
  explicit Addr(Reg reg, int offset) : reg(reg), offset(offset) {}
  Reg getReg() const { return this->reg; }
  int getOffset() const { return this->offset; }
  std::string getName() const {
    return "[" + reg.getName() + ", " + "#" + std::to_string(this->offset) +
           "]";
  }
};

class Constant : public Value {
  int value; // 浮点数转化为的十进制数 || 整型数
  bool is_fp_;

public:
  explicit Constant(int value, bool is_fp) : value(value), is_fp_(is_fp) {}
  bool is_reg() const { return false; }
  bool is_constant() const { return true; }
  bool has_shift() const { return false; }
  int getValue() const { return this->value; }
  bool is_fp() const { return this->is_fp_; }
  std::string getName() const { return "#" + std::to_string(this->value); }
};

class Label {
  std::string label;
  int offset;

public:
  explicit Label(std::string label, int offset)
      : label(label), offset(offset) {}
  explicit Label(std::string label) : label(label), offset(0) {}
  std::string getName() const { return label; }
};
std::string comment(std::string str1, std::string str2);
std::string condCode(const CmpOp &cond);
std::string push(const std::vector<Reg> &reg_list);
std::string pop(const std::vector<Reg> &reg_list);
std::string mov(const Reg &dst, const Value &src, const CmpOp &op = NOP);
std::string mvn(const Reg &dst, const Value &src, const CmpOp &op = NOP);
std::string setValue(const Reg &dst, const Constant &src);
std::string getAddress(const Reg &dst, const Label &src);
std::string ldr(const Reg &dst, const Addr &src);
std::string ldr(const Reg &dst, const Label &src);
std::string ldr(const Reg &dst, const Reg &base, const Reg &offset);
std::string ldr(const Reg &dst, const Reg &base, const Reg &offset,
                const Constant &shift);
std::string str(const Reg &src, const Addr &dst);
std::string str(const Reg &src, const Label &dst);
std::string str(const Reg &dst, const Reg &base, const Reg &offset);
std::string str(const Reg &dst, const Reg &base, const Reg &offset,
                const Constant &shift);
std::string store(const Reg &src, const Reg &base, const Reg &offset);
std::string bl(const std::string &dst_func_name);
std::string add(const Reg &dst, const Reg &op1, const Value &op2);
std::string sub(const Reg &dst, const Reg &op1, const Value &op2);
std::string rsb(const Reg &dst, const Reg &op1, const Value &op2);
std::string and_(const Reg &dst, const Reg &op1, const Value &op2);
std::string orr(const Reg &dst, const Reg &op1, const Value &op2);
std::string eor(const Reg &dst, const Reg &op1, const Value &op2);
std::string clz(const Reg &dst, const Reg &op1);
std::string lsl(const Reg &dst, const Reg &op1, const Value &op2);
std::string asl(const Reg &dst, const Reg &op1,
                const Value &op2); // same as lsl
std::string lsr(const Reg &dst, const Reg &op1, const Value &op2);
std::string asr(const Reg &dst, const Reg &op1, const Value &op2);
std::string mul(const Reg &dst, const Reg &op1, const Reg &op2);
std::string smmul(const Reg &dst, const Reg &op1, const Reg &op2);
std::string mla(const Reg &dst, const Reg &op1, const Reg &op2,
                const Reg &op3);
std::string smmla(const Reg &dst, const Reg &op1, const Reg &op2,
                  const Reg &op3);
std::string mls(const Reg &dst, const Reg &op1, const Reg &op2,
                const Reg &op3);
std::string smull(const Reg &dst, const Reg &op1, const Reg &op2,
                  const Reg &op3);
std::string sdiv(const Reg &dst, const Reg &op1, const Reg &op2);
std::string cmp(const Reg &lhs, const Value &rhs);
std::string b(const Label &dst, const CmpOp &op = NOP);
std::string instConst(std::string (*inst)(const Reg &dst, const Reg &op1,
                                          const Value &op2),
                const Reg &dst, const Reg &op1, const Constant &op2, Instruction *instruction);
std::string instConst(std::string (*inst)(const Reg &op1, const Value &op2),
                      const Reg &op1, const Constant &op2, Instruction *instruction);
std::string load(const Reg &dst, const Addr &src);
std::string store(const Reg &src, const Addr &dst);
std::string swi(const Constant &id);
std::string bic(const Reg &dst, const Reg &v1, const Reg &v2, const Reg &v3);
std::string vvmul(const Reg &sum, const Reg &v1, const Reg &v2, const int len);
std::tuple<int, int, int> choose_multiplier(int d, int N);
std::string divConst(const Reg &dst, const Reg &src,
                     const Constant &divisor);

std::string vmrs();// 将fcmp比较结果置位到CPSR(APSR)
std::string vcvt(const Reg &dst);

}; // namespace InstGen

const InstGen::Reg vinst_temp_reg = InstGen::Reg(11, false);

#endif