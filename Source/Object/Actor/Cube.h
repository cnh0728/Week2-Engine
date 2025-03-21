#pragma once
#include "Actor.h"
#include "Object/ObjectMacros.h"
class UUUIDTextComponent;

class ACube : public AActor
{
	DECLARE_CLASS(ACube, AActor)
	using Super = AActor;
public:
	ACube();
	virtual ~ACube() = default;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//virtual const char* GetTypeName() override;
};

