#pragma once
#include "Actor.h"

class AWorldGrid : public AActor
{
    using Super = AActor;
public:
    AWorldGrid();
    virtual ~AWorldGrid() = default;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual const char* GetTypeName() override { return "AWorldGrid"; }
};
