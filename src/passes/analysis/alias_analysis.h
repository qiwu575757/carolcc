#ifndef COMPILER_ALIAS_ANALYSIS_H
#define COMPILER_ALIAS_ANALYSIS_H

#include "passes/pass.h"
#include "passes/module.h"
#include <set>
#include <list>
#include <map>
class Module;
class Function;
class BasicBlock;
class Value;
class StoreInst;
class LoadInst;

class AliasAnalysis  {
   public:
   static Value* getArrayPtr(Value* ptr);
   static bool isGlobal(Value* array);
   static bool isParam(Value* array);
   static bool isLocal(Value* array);
   static bool isGlobalArray(Value* array);
   static bool alias(Value* array1,Value* array2);
 static bool isSamePtr(StoreInst* store, LoadInst* load) ;
};

#endif  // COMPILER_DOMINATORS_H
