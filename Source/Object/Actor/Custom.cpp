#include "Custom.h"

#include "Object/PrimitiveComponent/CustomComponent.h"

ACustom::ACustom()
{
    bCanEverTick = true;

    UCustomComponent* CustomComp = AddComponent<UCustomComponent>();
    CustomComp->SetupAttachment(RootComponent);
    CustomComp->SetTextureResource(ECustom);
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