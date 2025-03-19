#include "Spotlight.h"
#include <Object/PrimitiveComponent/UPrimitiveComponent.h>
#include <Object/PrimitiveComponent/UBoundingBox.h>
ASpotlight::ASpotlight()
{
	bCanEverTick = true;

	USpotlightComp* SpotlightComp = AddComponent<USpotlightComp>();

	RootComponent = SpotlightComp;

	SetActorRelatvieTransform(FTransform());
}

void ASpotlight::BeginPlay()
{
	SetUUIDTag();

	Super::BeginPlay();
}

void ASpotlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//const char* ASphere::GetTypeName()
//{
//	return "Sphere";
//}
