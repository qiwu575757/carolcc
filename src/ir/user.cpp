
#include "user.h"
#include <cstdio>
#include "visitor/ir_visitor_base.h"
#include "utils.h"
User::User(Type *type, const std::string &name, unsigned int use_number) : Value(name,type),_use_number(use_number){
    _operands_list.resize(use_number);
}
void User::setOperand(unsigned no, Value *v) {

        // MyAssert("out of index", no >= 0 && no < _use_number,
                //  EXIT_CODE_ERROR_327);
    // }
    // MyAssert("null ptr",v!=nullptr,EXIT_CODE_ERROR_329);
    _operands_list[no] = v;
    v->addUse(this,no);
}
Value *User::getOperand(unsigned int no)const {
    if(!(no>=0&&no<_use_number)){
        ERROR("out of index: expect %d,but only has %d oprands",EXIT_CODE_ERROR_328,no,_use_number);
    }
    return _operands_list.at(no);
}
void User::accept(IrVisitorBase *v) {
    v->visit(this);
}
/**
 * 删掉 自己参数被使用的记录
 */
void User::removeUseOps() {
    int i = 0;
    // printf("指令%s的oprand :", this->getPrintName().c_str());
    // for(auto op : _operands_list){
    //     printf("(%d,%s)",i,op->getPrintName().c_str());
    // }
    // printf("\n");
    // i = 0;
    for ( auto op : _operands_list) {
        op->removeUse(this, i);
        i++;
    }
    _use_number = 0;
    // _operands_list.clear();

}
void User::addOperand(Value *v) {
    _operands_list.push_back(v);
    v->addUse(this,_use_number);
    _use_number++;
}
void User::rmOperand(unsigned  index) {

    removeUseOps();
    std::vector<Value* > old_operands ;
    old_operands.swap(_operands_list);
    // std::vector<Value* > old_operands ;
    // old_operands.assign(_operands_list.begin(),_operands_list.end());
    // removeUseOps();
    for (int i = 0; i < old_operands.size(); ++i) {
        if( i == index)
            continue ;
        addOperand(old_operands[i]);

    }
}
void User::rmOperand(unsigned  i1,unsigned  i2) {

    removeUseOps();
    std::vector<Value* > old_operands ;
    old_operands.swap(_operands_list);
    // std::vector<Value* > old_operands ;
    // old_operands.assign(_operands_list.begin(),_operands_list.end());
    // removeUseOps();
    for (int i = 0; i < old_operands.size(); ++i) {
        if( i == i1|| i == i2)
            continue ;
        addOperand(old_operands[i]);

    }
}

void User::changeOperand(int index, Value *new_val) {
    auto old_val = getOperand(index);
    old_val->removeUse(this, index);
    setOperand(index, new_val);
}