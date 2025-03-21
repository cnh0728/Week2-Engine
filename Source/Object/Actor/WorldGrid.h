#pragma once
#include "Actor.h"
#include "Object/ObjectMacros.h"
class AWorldGrid : public AActor
{
    using Super = AActor;

	DECLARE_CLASS(AWorldGrid, AActor)

    float Spacing = 1.f;
    
public:
    AWorldGrid();

    void SetSpacing(float InSpacing);
    float GetSpacing() { return Spacing; }
    
    virtual ~AWorldGrid() = default;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    //virtual const char* GetTypeName() override { return "AWorldGrid"; }
};
