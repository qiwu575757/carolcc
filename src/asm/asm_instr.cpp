#include "asm_instr.h"

#include <cmath>
#include <tuple>

namespace InstGen {

std::string condCode(const CmpOp &cond) {
  std::string asm_instr;
  switch (cond) {
  case EQ:
    asm_instr += "eq";
    break;
  case NE:
    asm_instr += "ne";
    break;
  case GT:
    asm_instr += "gt";
    break;
  case GE:
    asm_instr += "ge";
    break;
  case LT:
    asm_instr += "lt";
    break;
  case LE:
    asm_instr += "le";
    break;
  default:
    break;
  }
  return asm_instr;
}

std::string push(const std::vector<Reg> &reg_list) {
  std::string asm_instr;
  bool flag = false;
  asm_instr += spaces;
  asm_instr += "push";
  asm_instr += " ";
  asm_instr += "{";
  for (auto &i : reg_list) {
    if (flag) {
      asm_instr += ", ";
    }
    asm_instr += i.getName();
    flag = true;
  }
  asm_instr += "}";
  asm_instr += newline;
  return asm_instr;
}

std::string pop(const std::vector<Reg> &reg_list) {
  std::string asm_instr;
  bool flag = false;
  asm_instr += spaces;
  asm_instr += "pop";
  asm_instr += " ";
  asm_instr += "{";
  for (auto &i : reg_list) {
    if (flag) {
      asm_instr += ", ";
    }
    asm_instr += i.getName();
    flag = true;
  }
  asm_instr += "}";
  asm_instr += newline;
  return asm_instr;
}

std::string mov(const Reg &dst, const Value &src, const CmpOp &cond) {
  std::string asm_instr;
  if (src.is_reg() && dst.getName() == src.getName()) {
    return asm_instr;
  }
  asm_instr += spaces;
  asm_instr += "mov";
  asm_instr += condCode(cond);
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += src.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string mvn(const Reg &dst, const Value &src, const CmpOp &cond) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "mvn";
  asm_instr += condCode(cond);
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += src.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string setValue(const Reg &dst, const Constant &src) {
  std::string asm_instr;
  auto val = src.getValue();
  if (0 <= val && val <= imm_8_max) {
    asm_instr += mov(dst, Constant(val));
  } else if (-imm_8_max <= val && val <= 0) {
    asm_instr += mvn(dst, Constant(-val - 1));
  } else {
    uint32_t imm = src.getValue();
    uint32_t imm_low = imm & ((1 << 16) - 1);
    uint32_t imm_high = imm >> 16;
    asm_instr += spaces;
    asm_instr += "movw";
    asm_instr += " ";
    asm_instr += dst.getName();
    asm_instr += ", ";
    asm_instr += "#" + std::to_string(imm_low);
    asm_instr += newline;
    asm_instr += spaces;
    asm_instr += "movt";
    asm_instr += " ";
    asm_instr += dst.getName();
    asm_instr += ", ";
    asm_instr += "#" + std::to_string(imm_high);
    asm_instr += newline;
  }
  return asm_instr;
}

std::string getAddress(const Reg &dst, const Label &src) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "adrl";
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += src.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string ldr(const Reg &dst, const Addr &src) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "ldr";
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += src.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string ldr(const Reg &dst, const Label &src) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "ldr";
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += src.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string ldr(const Reg &dst, const Reg &base, const Reg &offset) {
  return ldr(dst, base, offset, Constant(0));
}

std::string ldr(const Reg &dst, const Reg &base, const Reg &offset,
                const Constant &shift) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "ldr";
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += "[";
  asm_instr += base.getName();
  asm_instr += ", ";
  asm_instr += "+";
  asm_instr += offset.getName();
  asm_instr += ", ";
  asm_instr += "lsl";
  asm_instr += " ";
  asm_instr += shift.getName();
  asm_instr += "]";
  asm_instr += newline;
  return asm_instr;
}

std::string str(const Reg &src, const Addr &dst) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "str";
  asm_instr += " ";
  asm_instr += src.getName();
  asm_instr += ", ";
  asm_instr += dst.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string str(const Reg &src, const Label &dst) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "str";
  asm_instr += " ";
  asm_instr += src.getName();
  asm_instr += ", ";
  asm_instr += dst.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string str(const Reg &dst, const Reg &base, const Reg &offset) {
  return str(dst, base, offset, Constant(0));
}

std::string str(const Reg &dst, const Reg &base, const Reg &offset,
                const Constant &shift) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "str";
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += "[";
  asm_instr += base.getName();
  asm_instr += ", ";
  asm_instr += "+";
  asm_instr += offset.getName();
  asm_instr += ", ";
  asm_instr += "lsl";
  asm_instr += " ";
  asm_instr += shift.getName();
  asm_instr += "]";
  asm_instr += newline;
  return asm_instr;
}

std::string bl(const std::string &dst_func_name) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "bl";
  asm_instr += " ";
  asm_instr += dst_func_name;
  asm_instr += newline;
  return asm_instr;
}

std::string add(const Reg &dst, const Reg &op1, const Value &op2) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "add";
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += op1.getName();
  asm_instr += ", ";
  asm_instr += op2.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string sub(const Reg &dst, const Reg &op1, const Value &op2) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "sub";
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += op1.getName();
  asm_instr += ", ";
  asm_instr += op2.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string rsb(const Reg &dst, const Reg &op1, const Value &op2) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "rsb";
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += op1.getName();
  asm_instr += ", ";
  asm_instr += op2.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string and_(const Reg &dst, const Reg &op1, const Value &op2) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "and";
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += op1.getName();
  asm_instr += ", ";
  asm_instr += op2.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string orr(const Reg &dst, const Reg &op1, const Value &op2) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "orr";
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += op1.getName();
  asm_instr += ", ";
  asm_instr += op2.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string eor(const Reg &dst, const Reg &op1, const Value &op2) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "eor";
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += op1.getName();
  asm_instr += ", ";
  asm_instr += op2.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string clz(const Reg &dst, const Reg &op1) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "clz";
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += op1.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string lsl(const Reg &dst, const Reg &op1, const Value &op2) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "lsl";
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += op1.getName();
  asm_instr += ", ";
  asm_instr += op2.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string asl(const Reg &dst, const Reg &op1, const Value &op2) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "asl";
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += op1.getName();
  asm_instr += ", ";
  asm_instr += op2.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string lsr(const Reg &dst, const Reg &op1, const Value &op2) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "lsr";
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += op1.getName();
  asm_instr += ", ";
  asm_instr += op2.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string asr(const Reg &dst, const Reg &op1, const Value &op2) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "asr";
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += op1.getName();
  asm_instr += ", ";
  asm_instr += op2.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string mul(const Reg &dst, const Reg &op1, const Reg &op2) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "mul";
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += op1.getName();
  asm_instr += ", ";
  asm_instr += op2.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string smmul(const Reg &dst, const Reg &op1, const Reg &op2) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "smmul";
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += op1.getName();
  asm_instr += ", ";
  asm_instr += op2.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string mla(const Reg &dst, const Reg &op1, const Reg &op2,
                const Reg &op3) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "mla";
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += op1.getName();
  asm_instr += ", ";
  asm_instr += op2.getName();
  asm_instr += ", ";
  asm_instr += op3.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string smmla(const Reg &dst, const Reg &op1, const Reg &op2,
                  const Reg &op3) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "smmla";
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += op1.getName();
  asm_instr += ", ";
  asm_instr += op2.getName();
  asm_instr += ", ";
  asm_instr += op3.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string mls(const Reg &dst, const Reg &op1, const Reg &op2,
                const Reg &op3) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "mls";
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += op1.getName();
  asm_instr += ", ";
  asm_instr += op2.getName();
  asm_instr += ", ";
  asm_instr += op3.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string smull(const Reg &dst, const Reg &op1, const Reg &op2,
                  const Reg &op3) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "smull";
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += op1.getName();
  asm_instr += ", ";
  asm_instr += op2.getName();
  asm_instr += ", ";
  asm_instr += op3.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string sdiv(const Reg &dst, const Reg &op1, const Reg &op2) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "sdiv";
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += ", ";
  asm_instr += op1.getName();
  asm_instr += ", ";
  asm_instr += op2.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string cmp(const Reg &lhs, const Value &rhs) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "cmp";
  asm_instr += " ";
  asm_instr += lhs.getName();
  asm_instr += ", ";
  asm_instr += rhs.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string cmp(const Reg &lhs, const Reg &rhs) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "cmp";
  asm_instr += " ";
  asm_instr += lhs.getName();
  asm_instr += ", ";
  asm_instr += rhs.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string b(const Label &dst, const CmpOp &cond) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "b";
  asm_instr += condCode(cond);
  asm_instr += " ";
  asm_instr += dst.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string instConst(std::string (*inst)(const Reg &dst, const Reg &op1,
                                          const Value &op2),
                      const Reg &dst, const Reg &op1, const Constant &op2) {
  std::string asm_instr;
  int val = op2.getValue();
  if (dst == op1 && op2.getValue() == 0 && (inst == add || inst == sub)) {
    return asm_instr;
  } else if (0 <= val && val <= imm_8_max) {
    asm_instr += inst(dst, op1, op2);
  } else {
    asm_instr += setValue(vinst_temp_reg, op2);
    asm_instr += inst(dst, op1, vinst_temp_reg);
  }
  return asm_instr;
}

std::string instConst(std::string (*inst)(const Reg &op1, const Value &op2),
                      const Reg &op1, const Constant &op2) {
  std::string asm_instr;
  int val = op2.getValue();
  if (0 <= val && val <= imm_8_max) {
    asm_instr += inst(op1, op2);
  } else {
    asm_instr += setValue(vinst_temp_reg, op2);
    asm_instr += inst(op1, vinst_temp_reg);
  }
  return asm_instr;
}

std::string load(const Reg &dst, const Addr &src) {
  std::string asm_instr;
  int offset = src.getOffset();
  if (offset > imm_12_max || offset < -imm_12_max) {
    asm_instr += InstGen::setValue(vinst_temp_reg, Constant(offset));
    asm_instr += InstGen::ldr(dst, src.getReg(), vinst_temp_reg);
  } else {
    asm_instr += InstGen::ldr(dst, src);
  }
  return asm_instr;
}

std::string store(const Reg &src, const Addr &dst) {
  assert(src != vinst_temp_reg);
  std::string asm_instr;
  int offset = dst.getOffset();
  if (offset > imm_12_max || offset < -imm_12_max) {
    asm_instr += InstGen::setValue(vinst_temp_reg, Constant(offset));
    asm_instr += InstGen::str(src, dst.getReg(), vinst_temp_reg);
  } else {
    asm_instr += InstGen::str(src, dst);
  }
  return asm_instr;
}

std::string swi(const Constant &id) {
  std::string asm_instr;
  asm_instr += spaces;
  asm_instr += "swi";
  asm_instr += " ";
  asm_instr += id.getName();
  asm_instr += newline;
  return asm_instr;
}

std::string bic(const Reg &dst, const Reg &v1, const Reg &v2,
                const Reg &v3) {
  std::string asm_instr;
  asm_instr += mov(vinst_temp_reg, Constant(1));
  asm_instr += spaces + "bic" + " " + vinst_temp_reg.getName() + ", " +
              v1.getName() + ", " + vinst_temp_reg.getName() + ", " + "lsl" +
              " " + v2.getName() + newline;
  asm_instr += spaces + "orr" + " " + dst.getName() + ", " +
              vinst_temp_reg.getName() + ", " + v3.getName() + ", " + "lsl" +
              " " + v2.getName() + newline;
  return asm_instr;
}

std::string vvmul(const Reg &dst, const Reg &v1, const Reg &v2,
                  const int len) {
  std::string asm_instr;
  if (len == 4) {
    asm_instr += spaces + "vld1.32 {d18-d19}, [" + v1.getName() + "]" + newline;
    asm_instr += spaces + "vld1.32 {d16-d17}, [" + v2.getName() + "]" + newline;
    asm_instr += spaces + "vmul.i32 q8, q8, q9" + newline;
    asm_instr += spaces + "vadd.i32 d16, d17, d16" + newline;
    asm_instr += spaces + "vpadd.i32 d16, d16, d16" + newline;
    asm_instr += spaces + "vmov.32 " + dst.getName() + ", d16[0]" + newline;
  } else if (len == 8) {
    asm_instr += spaces + "vld2.32 {d24-d27}, [" + v1.getName() + "]" + newline;
    asm_instr += spaces + "vld2.32 {d20-d23}, [" + v2.getName() + "]" + newline;
    asm_instr += spaces + "vmul.i32 q8, q12, q10" + newline;
    asm_instr += spaces + "vmla.i32 q8, q13, q11" + newline;
    asm_instr += spaces + "vadd.i32 d16, d17, d16" + newline;
    asm_instr += spaces + "vpadd.i32 d16, d16, d16" + newline;
    asm_instr += spaces + "vmov.32 " + dst.getName() + ", d16[0]" + newline;
  } else {
    std::cerr << "Unsupported SIMD vector mul vector length: " << len
              << std::endl;
    abort();
  }
  return asm_instr;
}

std::tuple<int, int, int> choose_multiplier(int d, int N) {
  assert(d >= 1);
  int l = ceil(log2((double)d - 0.5));
  int sh_post = l;
  uint64_t m_l = (((uint64_t)1) << (N + l)) / d;
  uint64_t m_h = ((((uint64_t)1) << (N + l)) + (((uint64_t)1) << (l))) / d;
  while ((m_l / 2 < m_h / 2) && (sh_post > 1)) {
    m_l /= 2;
    m_h /= 2;
    sh_post--;
  }
  sh_post--;
  return std::make_tuple((int)(m_h), sh_post, l);
};

std::string divConst(const Reg &dst, const Reg &src,
                     const Constant &divisor) {
  const int N = 32;
  std::string asm_instr;
  int d = divisor.getValue();
  assert(d != 0);
  int m, sh_post, l;
  std::tie(m, sh_post, l) = choose_multiplier(abs(d), N - 1);
  if (abs(d) == 1) {
    asm_instr += mov(dst, src);
  } else if (abs(d) == (1 << l)) {
    // q = SRA(n + SRL(SRA(n, l - 1), N - l), l);
    asm_instr += asr(vinst_temp_reg, src, Constant(l - 1));
    asm_instr += lsr(vinst_temp_reg, vinst_temp_reg, Constant(N - l));
    asm_instr += add(vinst_temp_reg, vinst_temp_reg, src);
    asm_instr += asr(dst, vinst_temp_reg, Constant(l));
  } else if (m >= 0) {
    // q = SRA(MULSH(m, n), sh_post) - XSIGN(n);
    asm_instr += setValue(vinst_temp_reg, Constant(m));
    asm_instr += smmul(vinst_temp_reg, vinst_temp_reg, src);
    asm_instr += asr(vinst_temp_reg, vinst_temp_reg, Constant(sh_post));
    asm_instr +=
        add(dst, vinst_temp_reg,
            RegShift(src.getID(), 31, InstGen::RegShift::ShiftType::lsr));
  } else {
    // q = SRA(n + MULSH(m - 2^N , n), sh_post) - XSIGN(n);
    asm_instr += setValue(vinst_temp_reg, Constant(m));
    asm_instr += smmla(vinst_temp_reg, vinst_temp_reg, src, src);
    asm_instr += asr(vinst_temp_reg, vinst_temp_reg, Constant(sh_post));
    asm_instr +=
        add(dst, vinst_temp_reg,
            RegShift(src.getID(), 31, InstGen::RegShift::ShiftType::lsr));
  }
  if (d < 0) {
    asm_instr += rsb(dst, dst, Constant(0));
  }
  return asm_instr;
}

std::string ret(const Value &src) {
  const InstGen::Reg target_reg = InstGen::Reg(0);
  std::string asm_instr;
  InstGen::CmpOp cmpop = InstGen::CmpOp(NOP);
  asm_instr += mov(target_reg,src, cmpop);

  return asm_instr;
}

std::string br(const Reg &target) {
  std::string asm_instr;
  asm_instr += spaces+"b "+newline;

  return asm_instr;
}


}; // namespace InstGen