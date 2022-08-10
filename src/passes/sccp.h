
#ifndef COMPILER_SCCP_H
#define COMPILER_SCCP_H

#include "ir/constant.h"
#include "ir/instruction.h"
#include "pass.h"
#include <map>
#include <list>


// #define SCCP_LOG_
#ifdef SCCP_LOG_
#define SCCP_LOG(format, ...)                                  \
    do {                                                                             \
        printf(YELLOW "[%s:%d]" format RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        fflush(stdout);                                                              \
    } while (0)
#else
#define SCCP_LOG(format, ...)
#endif // SCCP_LOG_

class Function;
class BinaryInst;
class Instruction;
/**
 * sparse conditional constant propagation
 */
class SCCP : public Transform {
   public:
    SCCP(Module* m,const std::string &name): Transform(m,name), uncertainValue(Value::getUncertainValue()){

    }
    void run() override;
    static Constant* calConstantIntBinary(Instruction* pInstruction,
                                   ConstantInt* oprt1_ptr,
                                   ConstantInt* oprt2_ptr);
    static Constant* calConstantFloatBinary(Instruction* instr,
                                     ConstantFloat* oprt1_ptr,
                                     ConstantFloat* oprt2_ptr);
    static bool detectBinaryConstantFold(BinaryInst* inst);
    static bool detectCastConstantFold(UnaryInst* pInst);
    static bool detectCmpConstantFold(CmpInst* pInst);
    static Constant* calConstantBinary(Instruction* inst, Value* oprt1, Value* oprt2);

   private:
//    Function* _cur_fun;
    void constantFoldAndPropagation(Function* f);


    // for sparse conditional constant propagation
    void printDebugInfo(Function *f);
    void sparseConditionalConstantPropagation(Function* f);
    void replaceConstant(Function * f);
    void sccpOnInstrution(Instruction* instr);
    void sccpOnBasicBlock(BasicBlock* bb);
    std::map<Value* ,Value*> _instr_assign_table;
    std::map<BasicBlock*,bool> _block_excutable_table;
    std::list<Value*> _value_work_list;
    std::list<BasicBlock*> _block_work_list;
    bool isBlockExcutable(BasicBlock* bb);
    void setBlockExcutable(BasicBlock* bb,bool is_excutable);
    bool isValueNeverAssigned(Value* value);
    bool isValueHasDefiniteValue(Value* value);
    bool isValueHasMultiValue(Value* value);
    Value* uncertainValue;

};

#endif  // COMPILER_SCCP_H
