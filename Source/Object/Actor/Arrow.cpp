#include "Arrow.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"

AArrow::AArrow()
{
	bCanEverTick = true;

	UCylinderComp* CylinderComp = AddComponent<UCylinderComp>();
	RootComponent = CylinderComp;

	RootComponent->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, -1.f), FQuat(0, 0, 0, 1), FVector(1.f, 1.f, 1.f)));

	UConeComp* ConeComp = AddComponent<UConeComp>();
	ConeComp->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, +1.f), FQuat(0, 0, 0, 1), FVector(1.f, 1.f, 1.f)));
	//CylinderComp->AttachToComponent(ConeComp);

}

void AArrow::BeginPlay()
{
	Super::BeginPlay();
}

void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const char* AArrow::GetTypeName()
{
	return "Arrow";
}
