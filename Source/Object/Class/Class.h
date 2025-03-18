#pragma once
#include "Core/Container/Name.h"


// uobject를 상속받는 클래스당 하나씩만 만들어짐
class UClass
{
public:
    UClass* SuperClass;
    FName Name;
    UClass(UClass* InSuperClass, FName InName) : SuperClass(InSuperClass), Name(InName) {}

    bool IsChildOf(const UClass* SomeBase) const {
        for (const UClass* Super = this; Super; Super = Super->SuperClass)
        {
            if (Super == SomeBase)
            {
                return true;
            }
        }
        return false;
    }

    FName GetName() const {
        return Name;
    }
};


#define DECLARE_CLASS(Class, Super) \
    public: \
    static UClass* StaticClass() { \
        static UClass ClassPrivate(Super::StaticClass(), FName(#Class)); \
        return &ClassPrivate; \
    } \
    virtual UClass* GetClass() const override { \
        return StaticClass(); \
    } \
    virtual const char* GetTypeName() override { \
        return #Class; \
    } \
    private:

