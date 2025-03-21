#include "UObject.h"
#include  "Class/Class.h"

UObject::UObject()
	: Name("None")
	, ClassPrivate(nullptr)
{
}

UObject::~UObject()
{
	// 필요시 정리 작업, 예: 로깅 등
}

UClass* UObject::StaticClass()
{
	static std::unique_ptr<UClass, UClassDeleter> StaticClassInfo = nullptr;
	if (!StaticClassInfo)
	{
		constexpr size_t ClassSize = sizeof(UClass);
		void* RawMemory = FPlatformMemory::Malloc<EAT_Object>(ClassSize);
		UClass* ClassPtr = new(RawMemory) UClass{ "UObject", sizeof(UObject), alignof(UObject), nullptr };
		StaticClassInfo = std::unique_ptr<UClass, UClassDeleter>(ClassPtr, UClassDeleter{});
	}
	return StaticClassInfo.get();
}

bool UObject::IsA(const UClass* SomeBase) const
{
	const UClass* ThisClass = GetClass();
	return ThisClass->IsChildOf(SomeBase);
}



