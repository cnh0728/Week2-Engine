#include "Sphere.h"
#include <Object/PrimitiveComponent/UPrimitiveComponent.h>
#include <Object/PrimitiveComponent/UBoundingBox.h>
ASphere::ASphere()
{
	bCanEverTick = true;

	USphereComp* SphereComponent = AddComponent<USphereComp>();
	SphereComponent -> SetupAttachment(RootComponent);

	UBoundingBox* BBox = AddComponent<UBoundingBox>();
	BBox->SetupAttachment(SphereComponent);

	SetActorRelatvieTransform(FTransform());
}

void ASphere::BeginPlay()
{
	SetUUIDTag();

	Super::BeginPlay();
}

void ASphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const char* ASphere::GetTypeName()
{
	return "Sphere";
}
