#include "Cube.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"
ACube::ACube()
{
	bCanEverTick = true;

	UCubeComp* CubeComponent = AddComponent<UCubeComp>();
	RootComponent = CubeComponent;

	CubeComponent->SetRelativeTransform(FTransform());
}

void ACube::BeginPlay()
{
	SetUUIDTag();
	
	Super::BeginPlay();
	
}

void ACube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//const char* ACube::GetTypeName()
//{
//	return "Cube";
//}