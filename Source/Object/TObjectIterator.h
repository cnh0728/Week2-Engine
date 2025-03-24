#pragma once
#include "Object/UObjectArray.h"

template<class T>
class TObjectIterator
{
public:
    TObjectIterator()/*( EObjectFlags AdditionalExclusionFlags = RF_NoFlags )*/
        : Index( GObjects.Num() ) // 전체 객체 수 가져오기
//        , ExclusionFlags( AdditionalExclusionFlags )
    {
        // 초기 유효한 객체 찾기
        --Index;
        CheckObject();
    }

    /** Iterator가 계속 유효한지 확인하는 연산자 */
    FORCEINLINE explicit operator bool() const
    {
        return Index >= 0;
    }

    /** 다음 객체로 이동 */
    void operator++()
    {
        --Index;
        CheckObject();
    }

    /** 현재 객체 반환 */
    FORCEINLINE T* operator*() const
    {
        return (T*)Object;
    }

    FORCEINLINE T* operator->() const
    {
        return (T*)Object;
    }

private:
    /** 현재 인덱스 */
    int32 Index;
    
    /** 제외할 객체 플래그 */
    // EObjectFlags ExclusionFlags;
    
    /** 현재 객체 */
    UObject* Object;

    /** 현재 인덱스가 유효한 객체인지 확인 */
    void CheckObject()
    {
        while (Index >= 0)
        {
            std::shared_ptr<UObject> ObjectItem = GObjects[Index];

            if (ObjectItem
                && ObjectItem->IsA<T>()) // 객체 타입 확인
                // && !(ObjectItem->GetFlags() & ExclusionFlags)) // 제외할 플래그 확인
            {
                Object = ObjectItem.get();
                return;
            }

            --Index;
        }
        Object = nullptr;
    }
};