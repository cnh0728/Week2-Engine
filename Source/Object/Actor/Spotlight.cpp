#include "Spotlight.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"
#include "Object/PrimitiveComponent/UBoundingBox.h"
#include "Object/PrimitiveComponent/BillBoardComponent.h"
#include "Object/World/World.h"

ASpotlight::ASpotlight()
{
	bCanEverTick = true;

	USpotlightComp* SpotlightComp = AddComponent<USpotlightComp>();

	SetActorRelativeTransform(FTransform());

	UBillBoardComponent* BillBoardComponent = AddComponent<UBillBoardComponent>();

	RootComponent = BillBoardComponent;
	SpotlightComp->SetupAttachment(RootComponent);
	BillBoardComponent->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.0f), FVector(), FVector().One()));

	BillBoardComponent->SetIsDefaultRendered(false);	
	UEngine::Get().GetWorld()->AddBillBoardComponent(BillBoardComponent);
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
