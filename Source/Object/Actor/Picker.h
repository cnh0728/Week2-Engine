#pragma once

#include "Actor.h"

class APicker : public AActor
{
    using Super = AActor;
public:
    APicker();
    ~APicker() = default;
    
    static FVector4 EncodeUUID(unsigned int UUID);
    static uint32_t DecodeUUID(FVector4 color);

    AActor* PickActorByRay(FVector MousePos);
    AActor* PickActorByPixel(FVector MousePos);
    bool IntersectsRay(const FVector& rayOrigin, const FVector& rayDir, float& Distance, FVector MinBound, FVector MaxBound);

    virtual void Tick(float DeltaTime) override;
    virtual void LateTick(float DeltaTime) override;
    virtual const char* GetTypeName() override;
};
