#pragma once
#include "Actor.h"

class ACustom : public AActor
{
public:
    using Super = AActor;
    DECLARE_CLASS(ACustom, AActor)
public:
    ACustom();
    virtual ~ACustom() = default;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
private:
    
};
