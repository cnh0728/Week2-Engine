#pragma once
#include "Core/Container/Name.h"
#include <concepts>

// uobject를 상속받는 클래스당 하나씩만 만들어짐
class UClass
{
public:
    UClass* SuperClass;
    FName Name;

    uint32 ClassCastFlags;
    UClass(UClass* InSuperClass, FName InName) : SuperClass(InSuperClass), Name(InName) {}

    FORCEINLINE bool IsChildOf(const UClass* SomeBase) const {
        for (const UClass* Super = this; Super; Super = Super->SuperClass)
        {
            if (Super == SomeBase)
            {
                return true;
            }
        }
        return false;
    }

    FORCEINLINE FName GetName() const {
        return Name;
    }

};


#define DECLARE_CLASS(Class, Super) \
    public: \
    FORCEINLINE static UClass* StaticClass() { \
        static UClass ClassPrivate(Super::StaticClass(), FName(#Class)); \
        return &ClassPrivate; \
    } \
    FORCEINLINE virtual UClass* GetClass() const override { \
        return StaticClass(); \
    } \
    FORCEINLINE virtual const char* GetTypeName() override { \
        return #Class; \
    } \
    private:

