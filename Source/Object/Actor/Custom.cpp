#include "Custom.h"

#include "Object/PrimitiveComponent/CustomComponent.h"

ACustom::ACustom()
{
    bCanEverTick = true;

    UCustomComponent* CustomComp = AddComponent<UCustomComponent>();
    CustomComp->SetupAttachment(RootComponent);
    CustomComp->LoadFromObj("Models/cube.obj");
}

void ACustom::BeginPlay()
{
    SetUUIDTag();

    Super::BeginPlay();
}

void ACustom::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}