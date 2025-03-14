#pragma once
#include <memory>
#include "Core/HAL/PlatformType.h"
#include "Core/Container/Name.h"


// TODO: RTTI 구현하면 enable_shared_from_this 제거
class UObject : public std::enable_shared_from_this<UObject>
{
	
	friend class FObjectFactory;

	uint32 UUID = 0;
	uint32 InternalIndex; // Index of GUObjectArray
protected:
	FName Name;
	static uint32_t InstanceCounter;
public:
	UObject();
	virtual ~UObject();
	//FName Name;

public:
	uint32 GetUUID() const { return UUID; }
	uint32 GetInternalIndex() const { return InternalIndex; }
};