#pragma once
#include <memory>
#include "Object/Class/Class.h"
#include "Core/HAL/PlatformMemory.h"

// ObjectMacros.h
#define DECLARE_CLASS(TClass, TSuperClass) \
private: \
    TClass(const TClass&) = delete; \
    TClass& operator=(const TClass&) = delete; \
    TClass(TClass&&) = delete; \
    TClass& operator=(TClass&&) = delete; \
public: \
    using Super = TSuperClass; \
    using ThisClass = TClass; \
    inline static UClass* StaticClass() \
    { \
        static std::unique_ptr<UClass, UClassDeleter> StaticClassInfo = nullptr; \
        if (!StaticClassInfo) \
        { \
            constexpr size_t ClassSize = sizeof(UClass); \
            void* RawMemory = FPlatformMemory::Malloc<EAT_Object>(ClassSize); \
            UClass* ClassPtr = new (RawMemory) UClass{ #TClass, static_cast<uint32>(sizeof(TClass)), static_cast<uint32>(alignof(TClass)), TSuperClass::StaticClass() }; \
            StaticClassInfo = std::unique_ptr<UClass, UClassDeleter>(ClassPtr, UClassDeleter{}); \
        } \
        return StaticClassInfo.get(); \
    } \
