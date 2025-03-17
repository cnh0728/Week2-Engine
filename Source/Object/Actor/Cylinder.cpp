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

    FTransform UUIDTextTransform;
    UUIDTextTransform.SetPosition(0.0f, 0.3f, 0.6f);
    UUIDTextComponent->SetRelativeTransform(UUIDTextTransform);

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
