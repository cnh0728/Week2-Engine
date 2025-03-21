#pragma once
#include "Actor.h"
#include "static/Enum.h"

class ATexture : public AActor
{
    using Super = AActor;
    DECLARE_CLASS(ATexture, AActor)
public:
    ATexture();
    virtual ~ATexture() = default;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
private:
};
