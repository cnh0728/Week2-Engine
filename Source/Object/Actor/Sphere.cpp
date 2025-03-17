#include "Sphere.h"
#include <Object/PrimitiveComponent/UPrimitiveComponent.h>
#include <Object/PrimitiveComponent/UBoundingBox.h>
ASphere::ASphere()
{
	bCanEverTick = true;

	USphereComp* SphereComponent = AddComponent<USphereComp>();
	RootComponent = SphereComponent;
	
	UBoundingBox* BBox = AddComponent<UBoundingBox>();
	BBox->SetupAttachment(SphereComponent);

	SetActorRelatvieTransform(FTransform());

	UUIDTextComponent = AddComponent<UUUIDTextComponent>();
	UUIDTextComponent->SetupAttachment(RootComponent);
	UUIDTextComponent->SetRelativeTransform(FTransform());

}

void ASphere::BeginPlay()
{
	Super::BeginPlay();
	UUIDTextComponent->SetText(FString::FromInt(GetUUID()));
}

void ASphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const char* ASphere::GetTypeName()
{
	return "Sphere";
}
