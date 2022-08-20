#include "type.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <ostream>

#include "passes/module.h"
#include "utils.h"

Type::Type(TypeID id) : _id(id) {}

// IntegerType *int32_type = IntegerType::get(32);
// FloatType *float_type =
Type *Type::getVoidTy() {
    static Type _void_type(Type::VoidTyID);
    return &_void_type;
}
Type *Type::getLabelTy() {
    static Type _label_ty(Type::LabelTyID);
    return &_label_ty;
}
IntegerType *Type::getInt1Ty() {
    static IntegerType _int1_ty(1);
    return &_int1_ty;
}
IntegerType *Type::getInt32Ty() {
    static IntegerType _int32_ty(32);
    return &_int32_ty;
}
FloatType *Type::getFloatTy() {
    static FloatType _float_ty;
    return &_float_ty;
}
PointerType *Type::getInt32PtrTy(){
    static PointerType _int32_ptr_ty(Type::getInt32Ty());
    return &_int32_ptr_ty;
}
PointerType *Type::getFloatPtrTy(){
    static PointerType _float_ptr_ty(Type::getFloatTy());
    return &_float_ptr_ty;
}
Type *Type::getPointerElementType() {
    if (this->isPointerTy())
        return static_cast<PointerType *>(this)->getElementType();
    else
        return nullptr;
}

int Type::getSize(bool extended) {
    if (this->isIntegerTy()) {
        auto bits = static_cast<IntegerType *>(this)->getNumBits() / 8;
        return bits > 0 ? bits : 1;
    }
    if (this->isFloatTy()) {
        return 4;
    }
    if (this->isArrayTy()) {
        auto element_size =
            static_cast<ArrayType *>(this)->getElementType()->getSize();
        auto num_elements = static_cast<ArrayType *>(this)->getNumOfElements();
        return element_size * num_elements;
    }
    if (this->isPointerTy()) {
        if (extended && this->getPointerElementType()->isArrayTy()) {
            return this->getPointerElementType()->getSize();
        } else {
            return 4;
        }
    }
    return 0;
}

void Type::print(std::ostream &output_file) {
    TYPE_LOG("visiting type::print");
    switch (_id) {
        case LabelTyID:
            TYPE_LOG("get label");
            output_file << "<label> ";
            break;

        case IntegerTyID:
            if (static_cast<IntegerType *>(this)->getNumBits() == 1) {
                TYPE_LOG("get i1");
                output_file << "i1 ";
            } else {
                TYPE_LOG("get i32");
                output_file << "i32 ";
            }
            break;

        case ArrayTyID:
            TYPE_LOG("get array");
            output_file << "[ "
                        << static_cast<ArrayType *>(this)->getNumOfElements()
                        << " x ";
            static_cast<ArrayType *>(this)->getElementType()->print(
                output_file);
            output_file << "] ";
            break;

        case PointerTyID:
            TYPE_LOG("get ptr");
            getPointerElementType()->print(output_file);
            output_file << "* ";
            break;
        case VoidTyID:
            TYPE_LOG("get void");
            output_file << "void ";
            break;

        case FloatTyID:
            TYPE_LOG("get float");
            output_file << "float ";
            break;
        case FunctionTyID:
            TYPE_LOG("get func type");
            ERROR("should not print func type", ERROR_TYPE);

        default:
            TYPE_LOG("get error type");
            break;
    }
    TYPE_LOG("return ");
    return;
}

std::string Type::CommentPrint() {
    std::string typeString;
    switch (_id) {
        case LabelTyID:
            typeString += "<label>";
            break;

        case IntegerTyID:
            if (static_cast<IntegerType *>(this)->getNumBits() == 1) {
                typeString += "i1";
            } else {
                typeString += "i32";
            }
            break;

        case ArrayTyID:
            typeString += "[ ";
            typeString +=
                std::to_string(
                    static_cast<ArrayType *>(this)->getNumOfElements()) +
                " x ";
            typeString += static_cast<ArrayType *>(this)
                              ->getElementType()
                              ->CommentPrint();
            typeString += "]";
            break;

        case PointerTyID:
            typeString += getPointerElementType()->CommentPrint();
            typeString += "*";
            break;

        default:
            break;
    }
    return typeString;
}

bool Type::eq(Type *rhs) {
#ifdef TTTT_LOGGG
    TYPE_LOG("lhs type:");
    this->print(std::cout);
    std::cout << std::endl;
    TYPE_LOG("rhs type:");
    rhs->print(std::cout);
    std::cout << std::endl;
    std::flush(std::cout);

#endif  // DEBUG
    if (this->_id != rhs->_id) {
        return false;
    } else if (this->isPointerTy()) {
#ifdef TTTT_LOGGG
        this->getPointerElementType()->print(std::cout);
        rhs->getPointerElementType()->print(std::cout);
        std::flush(std::cout);
#endif  // DEBUG
        return this->getPointerElementType()->eq(rhs->getPointerElementType());
    } else {
        return true;
    }
}

bool Type::isBool() {
    if (getTypeID() == IntegerTyID) {
        if (static_cast<IntegerType *>(this)->getNumBits() == 1) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}
bool Type::isInt32() {
    if (getTypeID() == IntegerTyID) {
        if (static_cast<IntegerType *>(this)->getNumBits() == 32) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}
int Type::getDims() {
    int res = 0;
    switch (_id) {
        case LabelTyID:
            res = -10000;
            break;
        case ArrayTyID:
            res =
                1 + static_cast<ArrayType *>(this)->getElementType()->getDims();
            break;
        case PointerTyID:
            res = 1 + static_cast<PointerType *>(this)
                          ->getPointerElementType()
                          ->getDims();
            break;
        default:
            res = 0;
    }
    return res;
}
// IntegerType
IntegerType::IntegerType(unsigned num_bits)
    : Type(Type::IntegerTyID), _num_bits(num_bits) {}

bool IntegerType::isInt1() {
    if (static_cast<IntegerType *>(this)->getNumBits() == 1) {
        return true;
    } else {
        return false;
    }
}

bool IntegerType::isInt32() {
    if (static_cast<IntegerType *>(this)->getNumBits() == 32) {
        return true;
    } else {
        return false;
    }
}
// FunctionType
FunctionType::FunctionType(Type *result, std::vector<Type *> &params)
    : Type(Type::FunctionTyID), _result(result) {
    //    exit_ifnot(_InvalidRetVal_Constructor_FunctionType,
    //               isValidReturnType(result) && "Invalid return type for
    //               function!");
    MyAssert("_InvalidRetVal_Constructor_FunctionType",
             isValidReturnType(result) && "Invalid return type for function!",
             EXIT_CODE_ERROR_322);

    for (auto p : params) {
        //        exit_ifnot(_InvalidArgType_Constructor_FunctionType,
        //                   isValidArgumentType(p) &&
        //                           "Not a valid type for function argument!");
        MyAssert("_InvalidRetVal_Constructor_FunctionType",
                 isValidArgumentType(p), EXIT_CODE_ERROR_323);
        _args.push_back(p);
    }
}

bool FunctionType::isValidReturnType(Type *ty) {
    return ty->isIntegerTy() || ty->isVoidTy() || ty->isFloatTy();
}

bool FunctionType::isValidArgumentType(Type *ty) {
    return ty->isIntegerTy() || ty->isPointerTy() || ty->isFloatTy();
}

unsigned FunctionType::getNumArgs() const { return _args.size(); }

Type *FunctionType::getArgType(unsigned i) const { return _args[i]; }

Type *FunctionType::getResultType() const { return _result; }
// ArrayType
ArrayType::ArrayType(Type *contained, unsigned num_elements)
    : Type(Type::ArrayTyID), _num_elements(num_elements) {
    MyAssert("_InvalidElemType_Constructor_ArrayType",
             isValidElementType(contained), EXIT_CODE_ERROR_324);
    _contained = contained;
}

bool ArrayType::isValidElementType(Type *ty) {
    return ty->isIntegerTy() || ty->isArrayTy() || ty->isFloatTy();
}

std::vector<unsigned> ArrayType::getDims() const {
    std::vector<unsigned> rets;
    auto elem_ty = _contained;
    rets.push_back(_num_elements);
    while (elem_ty->isArrayTy()) {
        auto arr = static_cast<ArrayType *>(elem_ty);
        rets.push_back(arr->getNumOfElements());
        elem_ty = arr->getElementType();
    }
    return rets;
}
// PointerType
PointerType::PointerType(Type *contained)
    : Type(Type::PointerTyID), _contained(contained) {}
FloatType *FloatType::get() { return new FloatType(); }
FloatType::FloatType() : Type(Type::FloatTyID) {}
