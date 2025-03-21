#pragma once
#include <memory>
#include "Core/HAL/PlatformType.h"
#include "Core/Container/Name.h"


class UClass;

// TODO: RTTI 구현하면 enable_shared_from_this 제거
class UObject : public std::enable_shared_from_this<UObject>
{
	friend class FObjectFactory;

	uint32 UUID = 0;
	uint32 InternalIndex; // Index of GUObjectArray

public:
	UObject();
	virtual ~UObject();
	FName Name;
	UClass* ClassPrivate;
	static UClass* StaticClass();

private:
	friend class FObjectFactory;
	friend class UClass;


public:
	uint32 GetUUID() const { return UUID; }
	uint32 GetInternalIndex() const { return InternalIndex; }

public:
	virtual UClass* GetClass() const { return ClassPrivate; }

	/** this가 SomeBase인지, SomeBase의 자식 클래스인지 확인합니다. */
	bool IsA(const UClass* SomeBase) const;

	template<typename T>
	bool IsA() const
	{
		return IsA(T::StaticClass());
	}
};