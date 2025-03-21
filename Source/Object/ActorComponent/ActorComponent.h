#pragma once
#include "Core/EngineTypes.h"
#include "Object/UObject.h"
#include "Object/ObjectMacros.h"

class UActorComponent : public UObject
{
	DECLARE_CLASS(UActorComponent, UObject)

public:
	UActorComponent() = default;

	virtual void BeginPlay();
	virtual void Tick(float DeltaTime);
	virtual void EndPlay(const EEndPlayReason::Type Reason);

	bool CanEverTick() const { return bCanEverTick; }

	virtual class AActor* GetOwner() const;
	virtual void SetOwner(class AActor* InOwner) { Owner = InOwner; }

protected:
	bool bCanEverTick = true;
	class AActor* Owner = nullptr;
};

