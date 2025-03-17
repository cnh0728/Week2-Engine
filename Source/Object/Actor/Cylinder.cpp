#include "Cylinder.h"
#include <Object/PrimitiveComponent/UPrimitiveComponent.h>

ACylinder::ACylinder()
{
    bCanEverTick = true;

    UCylinderComp* CylinderComponent = AddComponent<UCylinderComp>();
    RootComponent = CylinderComponent;
	
    SetActorRelatvieTransform(FTransform());

	UUIDTextComponent = AddComponent<UUUIDTextComponent>();
	UUIDTextComponent->SetupAttachment(RootComponent);
	UUIDTextComponent->SetRelativeTransform(FTransform());

}

void ACylinder::BeginPlay()
{
    Super::BeginPlay();
    UUIDTextComponent->SetText(FString::FromInt(GetUUID()));
}

void ACylinder::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

const char* ACylinder::GetTypeName()
{
    return "Cylinder";
}
