#pragma once

#include "Actor.h"

class APicker : public AActor
{
    using Super = AActor;
public:
    APicker();
    ~APicker() = default;
    
    static FVector4 EncodeUUID(unsigned int UUID);
    static int DecodeUUID(FVector4 color);
    
    virtual void Tick(float DeltaTime) override;
    virtual void LateTick(float DeltaTime) override;
    virtual const char* GetTypeName() override;
};
