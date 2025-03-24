#pragma once
#include "Actor.h"
#include "Object/ObjectMacros.h"

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
    void SetObjPath(const std::string& InPath);

private:
    
};
