#pragma once
#include "Actor.h"
#include "Object/ObjectMacros.h"
class ASpotlight : public AActor
{
	using Super = AActor;
	DECLARE_CLASS(ASpotlight, AActor)
public:
	ASpotlight();
	virtual ~ASpotlight() = default;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};

