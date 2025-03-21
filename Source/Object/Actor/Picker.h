#pragma once

#include "Actor.h"
#include "Object/ObjectMacros.h"

namespace std {
    template<>
    struct hash<pair<float, AActor*>> {
        size_t operator()(const pair<float, AActor*>& p) const {
            return hash<float>()(p.first) ^ hash<AActor*>()(p.second);
        }
    };
}

class APicker : public AActor
{
    using Super = AActor;
	DECLARE_CLASS(APicker, AActor)
public:
    APicker();
    ~APicker() = default;
    
    static FVector4 EncodeUUID(unsigned int UUID);
    static uint32_t DecodeUUID(FVector4 color);

    TMap<UPrimitiveComponent*, float> PickActorByRay();
    AActor* PickActorByPixel(FVector MousePos);
    bool IntersectsRay(const FVector& rayOrigin, const FVector& rayDir, float& Distance, FVector MinBound, FVector MaxBound);
    void UpdateRayInfo();

    virtual void Tick(float DeltaTime) override;
    virtual void LateTick(float DeltaTime) override;
    //virtual const char* GetTypeName() override;

protected:
    FVector4 RayOrigin;
    FVector4 RayEnd;
    FVector RayDir;
};
