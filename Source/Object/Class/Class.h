#pragma once
#include "Core/Container/Name.h"
#include <memory>
#include "Core/HAL/PlatformMemory.h"
#include <concepts>
#include "../UObject.h"



// uobject를 상속받는 클래스당 하나씩만 만들어짐
class UClass : public UObject
{
public:
    UClass* SuperClass;

    uint32 ClassCastFlags;
    UClass(const char* InClassName, uint32 InClassSize, uint32 InAlignment, UClass* InSuperClass);

	//virtual ~UClass() override = default;

	// 복사 & 이동 생성자 제거
	UClass(const UClass&) = delete;
	UClass& operator=(const UClass&) = delete;
	UClass(UClass&&) = delete;
	UClass& operator=(UClass&&) = delete;


	/** SomeBase의 자식 클래스인지 확인합니다. */
	bool IsChildOf(const UClass* SomeBase) const;

	template <typename T>
		requires std::derived_from<T, UObject>
	bool IsChildOf() const
	{
		return IsChildOf(T::StaticClass());
	}

	/**
	 * 부모의 UClass를 가져옵니다.
	 *
	 * @note AActor::StaticClass()->GetSuperClass() == UObject::StaticClass()
	 */
	UClass* GetSuperClass() const
	{
		return SuperClass;
	}

	/*UObject* GetDefaultObject() const
	{
		if (!ClassDefaultObject)
		{
			const_cast<UClass*>(this)->CreateDefaultObject();
		}
		return ClassDefaultObject;
	}*/

    FORCEINLINE FName GetName() const {
        return this -> Name;
    }

//protected:
//	virtual UObject* CreateDefaultObject();
















private:

    uint32 ClassSize;

    uint32 ClassAlignment;

   // UObject* ClassDefaultObject = nullptr;

};

struct UClassDeleter
{
    void operator()(UClass* ClassPtr) const
    {
        ClassPtr->~UClass();
        FPlatformMemory::Free<EAT_Object>(ClassPtr, sizeof(UClass));
    }
};



