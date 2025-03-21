#pragma once
#include "Actor.h"
#include "Object/ObjectMacros.h"
class ACone : public AActor
{
    using Super = AActor;
	DECLARE_CLASS(ACone, AActor)
public:
    ACone();
    virtual ~ACone() = default;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    //virtual const char* GetTypeName() override;

};

