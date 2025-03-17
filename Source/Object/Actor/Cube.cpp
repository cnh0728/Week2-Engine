#include "Cube.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"

ACube::ACube()
{
	bCanEverTick = true;

	UCubeComp* CubeComponent = AddComponent<UCubeComp>();
	RootComponent = CubeComponent;

	CubeComponent->SetRelativeTransform(FTransform());

	UUIDTextComponent = AddComponent<UUUIDTextComponent>();
	UUIDTextComponent->SetupAttachment(RootComponent);

	FTransform UUIDTextTransform;
	UUIDTextTransform.SetPosition(0.0f, 0.7f, 0.7f);
	UUIDTextComponent->SetRelativeTransform(UUIDTextTransform);
}

void ACube::BeginPlay()
{
	Super::BeginPlay();
	UUIDTextComponent->SetText(FString::FromInt(GetUUID()));
}

void ACube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const char* ACube::GetTypeName()
{
	return "Cube";
}