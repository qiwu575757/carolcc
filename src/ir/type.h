#ifndef COMPILER_TYPE_H
#define COMPILER_TYPE_H

#include "passes/module.h"
#include <iostream>
#include <vector>

class IntegerType;
class FunctionType;
class ArrayType;
class FloatType;
class PointerType;

class Type {
public:
    enum TypeID {
        VoidTyID,    // Void
        LabelTyID,   // Labels, e.g., BasicBlock
        IntegerTyID, // Integers, include 32 bits and 1 bit
        FloatTyID,   // Float
        FunctionTyID,// Functions
        ArrayTyID,   // Arrays
        PointerTyID, // Pointer
    };

    explicit Type(TypeID id);
    ~Type() = default;

public:
    TypeID getTypeID() const { return _id; }

    bool isVoidTy() const { return getTypeID() == VoidTyID; }
    bool isLabelTy() const { return getTypeID() == LabelTyID; }
    bool isIntegerTy() const { return getTypeID() == IntegerTyID; }
    bool isFloatTy() const { return getTypeID() == FloatTyID; }
    bool isFunctionTy() const { return getTypeID() == FunctionTyID; }
    bool isArrayTy() const { return getTypeID() == ArrayTyID; }
    bool isPointerTy() const { return getTypeID() == PointerTyID; }

    static Type *getVoidTy(Module *m) { return m->getVoidTy(); }
    static Type *getLabelTy(Module *m) { return m->getLabelTy(); }
    static IntegerType *getInt1Ty(Module *m) { return m->getInt1Ty(); }
    static IntegerType *getInt32Ty(Module *m) { return m->getInt32Ty(); }
    static FloatType *getFloatTy(Module *m) { return m->getFloatTy(); }
    static PointerType *getInt32PtrTy(Module *m) { return m->getInt32PtrTy(); }

    Type *getPointerElementType();

    int getSize(bool extended = true);

    void print();

    std::string CommentPrint();

    bool eq(Type rhs);

private:
    TypeID _id;
};

class IntegerType : public Type {
public:
    explicit IntegerType(unsigned num_bits);

    bool isInt1();

    bool isInt32();

    static IntegerType *get(unsigned num_bits) { return new IntegerType(num_bits); }

    unsigned getNumBits() { return _num_bits; };

private:
    unsigned _num_bits;
};

class FloatType : public Type {
private:
    FloatType();

public:
    static FloatType *get();
};

class FunctionType : public Type {
public:
    FunctionType(Type *result, std::vector<Type *> params);

    static bool isValidReturnType(Type *ty);
    static bool isValidArgumentType(Type *ty);

    static FunctionType *get(Type *result, std::vector<Type *> params) { return new FunctionType(result, params); }

    unsigned getNumArgs() const;

    Type *getArgType(unsigned i) const;
    Type *getResultType() const;

private:
    Type *_result;
    std::vector<Type *> _args;
};

class ArrayType : public Type {
public:
    ArrayType(Type *contained, unsigned num_elements);

    static bool isValidElementType(Type *ty);

    static ArrayType *get(Type *contained, unsigned num_elements) { return new ArrayType(contained, num_elements); }

    Type *getElementType() const { return _contained; }
    unsigned getNumOfElements() const { return _num_elements; }
    std::vector<unsigned> getDims() const;

private:
    Type *_contained;      // The element type of the array.
    unsigned _num_elements;// Number of elements in the array.
};

class PointerType : public Type {
public:
    PointerType(Type *contained);

    Type *getElementType() const { return _contained; }

    static PointerType *get(Type *contained) { return new PointerType(contained); }

private:
    Type *_contained;// The element type of the ptr.
};
#endif// COMPILER_TYPE_H