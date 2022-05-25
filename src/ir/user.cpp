//
// Created by yonchicy on 2022/5/25.
//

#include "user.h"
User::User(Type *type, const char *name, unsigned int use_number) : Value(name,type),_use_number(use_number){
    _use_list.resize(use_number);
}
