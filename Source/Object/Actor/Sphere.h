#pragma once
#include "Object/Actor/Actor.h"
#include "Object/ObjectMacros.h"
class ASphere : public AActor
{
	using Super = AActor;
	DECLARE_CLASS(ASphere, AActor)
public:
	ASphere();
	virtual ~ASphere() = default;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//virtual const char* GetTypeName() override;
};

