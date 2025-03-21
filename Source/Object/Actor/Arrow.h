#pragma once
#include "Object/Actor/Actor.h"
#include "Object/ObjectMacros.h"
class AArrow : public AActor
{
	using Super = AActor;
	DECLARE_CLASS(AArrow, AActor)
public:
	AArrow();
	virtual ~AArrow() = default;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//virtual const char* GetTypeName() override;
};

