#include "Cone.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"

ACone::ACone()
{
    bCanEverTick = true;

    UConeComp* ConeComponent = AddComponent<UConeComp>();
    RootComponent = ConeComponent;
	
    SetActorTransform(FTransform());

    UUIDTextComponent = AddComponent<UUUIDTextComponent>();
    UUIDTextComponent->SetupAttachment(RootComponent);
    UUIDTextComponent->SetRelativeTransform(FTransform());

}

void ACone::BeginPlay()
{
    Super::BeginPlay();
    UUIDTextComponent->SetText(FString::FromInt(GetUUID()));
}

void ACone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

const char* ACone::GetTypeName()
{
    return "Cone";
}
