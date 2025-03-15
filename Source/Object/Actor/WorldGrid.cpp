#include "WorldGrid.h"

#include "Object/PrimitiveComponent/UPrimitiveComponent.h"

AWorldGrid::AWorldGrid()
{
    bCanEverTick = true;

    USceneComponent* RootComponent = AddComponent<USceneComponent>();
    RootComponent = RootComponent;
    
    for (int i=-50; i<=50; i++)
    {
        ULineComp* XLineComponent = AddComponent<ULineComp>();
        XLineComponent->SetRelativeTransform(FTransform(FVector(0.f, static_cast<float>(i), 0.f), FQuat(0.f, 0., 0.f,  0.f), FVector(10.f, 1.f, 1.f)));
        XLineComponent->SetupAttachment(RootComponent);

        ULineComp* YLineComponent = AddComponent<ULineComp>();
        YLineComponent->SetRelativeTransform(FTransform(FVector(static_cast<float>(i), 0.f, 0.f), FQuat(0.f, 0., PIDIV2/2, PIDIV2/2), FVector(10.f, 1.f, 1.f)));
        YLineComponent->SetupAttachment(RootComponent);

        // ULineComp* YLineComponent = AddComponent<ULineComp>();
    }

    // UConeComp* ConeComp1 = AddComponent<UConeComp>();
    // ConeComp1->SetRelativeTransform(FTransform(FVector(0.0f,2.0f, 1.0f), FQuat(0, 0.3, 0.7, 1), FVector(1.f, 1.f, 1.f)));
	   
    // RootComponent = CylinderComp;
    // ConeComp->SetupAttachment(CylinderComp);
    // ConeComp1->SetupAttachment(ConeComp);
    
}

void AWorldGrid::BeginPlay()
{
    Super::BeginPlay();
}

void AWorldGrid::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
