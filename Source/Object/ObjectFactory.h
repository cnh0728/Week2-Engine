#pragma once
#include "Core/Engine.h"
#include "Core/EngineStatics.h"
#include "Core/HAL/PlatformMemory.h"
#include "Debug/DebugConsole.h"
#include "Core/Container/Name.h"
class UObject;

class FObjectFactory
{
public:
    /**
     * UObject를 생성합니다.
     * @tparam T UObject를 상속받은 클래스
     * @return 캐스팅된 UObject*
     */
    template<typename T>
        requires std::derived_from<T, UObject>

    static T* ConstructObject()
    {
        UE_LOG("DEBUG: Construct %s Object", *(T::StaticClass()->GetName().GetString()));

        constexpr size_t ObjectSize = sizeof(T);
        void* RawMemory = FPlatformMemory::Malloc<EAT_Object>(ObjectSize);

        T* ObjectPtr = new (RawMemory) T();
        std::shared_ptr<T> NewObject(ObjectPtr, [ObjectSize](T* Obj)
        {
            Obj->~T();
            FPlatformMemory::Free<EAT_Object>(Obj, ObjectSize);
        });
        NewObject->UUID = UEngineStatics::GenUUID();
        NewObject->ClassPrivate = T::StaticClass();

        // Object 제거시 Index가 달라지기 때문에 임시 주석처리 <- RemoveSwap으로 해결 가능
        // NewObject->InternalIndex = UEngine::Get().GObjects.Add(NewObject);
        GObjects.Add(NewObject->GetUUID(), NewObject);
        FName Name = T::StaticClass()->GetName();
        FString NameStr = Name.GetString();
        static uint8 ObjectContructionCount = 0;

        NewObject->Name = FName(*NameStr, ObjectContructionCount++);
        return NewObject.get();
    }
};
