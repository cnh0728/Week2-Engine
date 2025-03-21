#pragma once
#include "Object/Actor/Actor.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"
#include "Object/ObjectMacros.h"
class AAxis : public AActor
{
	using Super = AActor;
	DECLARE_CLASS(AAxis, AActor)
public:
	AAxis();
	virtual ~AAxis() = default;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//virtual const char* GetTypeName() override;
};

