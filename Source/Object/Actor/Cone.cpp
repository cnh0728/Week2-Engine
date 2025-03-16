#include "Cone.h"

#include <Object/PrimitiveComponent/UPrimitiveComponent.h>

ACone::ACone()
{
    bCanEverTick = true;

    UConeComp* ConeComponent = AddComponent<UConeComp>();
    RootComponent = ConeComponent;
	
    SetActorRelatvieTransform(FTransform());
}

void ACone::BeginPlay()
{
    Super::BeginPlay();
}

void ACone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

const char* ACone::GetTypeName()
{
    return "Cone";
}
