#include "Sphere.h"
#include <Object/PrimitiveComponent/UPrimitiveComponent.h>
#include <Object/PrimitiveComponent/UBoundingBox.h>
ASphere::ASphere()
{
	bCanEverTick = true;
;
	USphereComp* SphereComponent = AddComponent<USphereComp>();
	RootComponent = SphereComponent;

	UCubeComp* cube = AddComponent<UCubeComp>();
	cube->SetupAttachment(RootComponent);
	cube->SetRelativeTransform(FTransform(FVector(3, 3, 3), FQuat(), FVector(0.4, 1, 2)));

	SetActorRelatvieTransform(FTransform());

	UUIDTextComponent = AddComponent<UUUIDTextComponent>();
	UUIDTextComponent->SetupAttachment(RootComponent);
	UUIDTextComponent->SetRelativeTransform(FTransform(FVector(0.0f, 0.9f, 0.9f), FVector(), FVector().One()));

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
