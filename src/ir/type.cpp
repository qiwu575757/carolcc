#include "type.h"
#include "passes/module.h"
#include <cassert>

Type::Type(TypeID id) : _id(id) {}

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

void Type::print() {
    switch (_id) {
        case LabelTyID:
            std::cerr << "<label>";
            break;

        case IntegerTyID:
            if (static_cast<IntegerType *>(this)->getNumBits() == 1) {
                std::cerr << "i1";
            } else {
                std::cerr << "i32";
            }
            break;

        case ArrayTyID:
            std::cerr << "[ " << static_cast<ArrayType *>(this)->getNumOfElements()
                      << " x ";
            static_cast<ArrayType *>(this)->getElementType()->print();
            std::cerr << "]";
            break;

        case PointerTyID:
            getPointerElementType()->print();
            std::cerr << "*";
            break;

        default:
            break;
    }
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
                    std::to_string(static_cast<ArrayType *>(this)->getNumOfElements()) +
                    " x ";
            typeString +=
                    static_cast<ArrayType *>(this)->getElementType()->CommentPrint();
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

bool Type::eq(Type rhs) {
    if (this->_id != rhs._id) {
        return false;
    } else if (this->isPointerTy()) {
        return this->getPointerElementType()->eq(*rhs.getPointerElementType());
    } else {
        return true;
    }
}
Type *Type::getVoidTy(Module *m) { return m->getVoidTy(); }
Type *Type::getLabelTy(Module *m) { return m->getLabelTy(); }
IntegerType *Type::getInt1Ty(Module *m) { return m->getInt1Ty(); }
IntegerType *Type::getInt32Ty(Module *m) { return m->getInt32Ty(); }
FloatType *Type::getFloatTy(Module *m) { return m->getFloatTy(); }
PointerType *Type::getInt32PtrTy(Module *m) { return m->getInt32PtrTy(); }
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
    //               isValidReturnType(result) && "Invalid return type for function!");
    MyAssert("_InvalidRetVal_Constructor_FunctionType", isValidReturnType(result) && "Invalid return type for function!");

    for (auto p: params) {
        //        exit_ifnot(_InvalidArgType_Constructor_FunctionType,
        //                   isValidArgumentType(p) &&
        //                           "Not a valid type for function argument!");
        MyAssert("_InvalidRetVal_Constructor_FunctionType", isValidArgumentType(p));
        _args.push_back(p);
    }
}

bool FunctionType::isValidReturnType(Type *ty) {
    return ty->isIntegerTy() || ty->isVoidTy();
}

bool FunctionType::isValidArgumentType(Type *ty) {
    return ty->isIntegerTy() || ty->isPointerTy();
}

unsigned FunctionType::getNumArgs() const { return _args.size(); }

Type *FunctionType::getArgType(unsigned i) const { return _args[i]; }

Type *FunctionType::getResultType() const { return _result; }
// ArrayType
ArrayType::ArrayType(Type *contained, unsigned num_elements)
    : Type(Type::ArrayTyID), _num_elements(num_elements) {
    MyAssert("_InvalidElemType_Constructor_ArrayType", isValidElementType(contained));
    _contained = contained;
}

bool ArrayType::isValidElementType(Type *ty) {
    return ty->isIntegerTy() || ty->isArrayTy();
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
FloatType *FloatType::get() {
    return new FloatType();
}
FloatType::FloatType() : Type(Type::FloatTyID) {
}
