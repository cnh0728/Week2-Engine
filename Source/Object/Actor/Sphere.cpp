#include "Sphere.h"
#include <Object/PrimitiveComponent/UPrimitiveComponent.h>

ASphere::ASphere()
{
	bCanEverTick = true;

	USphereComp* SphereComponent = AddComponent<USphereComp>();
	RootComponent = SphereComponent;
	
	SetActorTransform(FTransform());

	UUIDTextComponent = AddComponent<UUUIDTextComponent>();
	UUIDTextComponent->SetupAttachment(RootComponent);
	UUIDTextComponent->SetRelativeTransform(FTransform());
}

void ASphere::BeginPlay()
{
	Super::BeginPlay();
	UUIDTextComponent->SetUUIDText(GetUUID());
}

void ASphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const char* ASphere::GetTypeName()
{
	return "Sphere";
}
