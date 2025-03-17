#include "WorldGrid.h"

#include "Object/PrimitiveComponent/UPrimitiveComponent.h"

AWorldGrid::AWorldGrid()
{
    bCanEverTick = true;
    bCanPick = false;
    
    for (int i=-500; i<=500; i++)
    {
        ULineComp* XLineComponent = AddComponent<ULineComp>();
        XLineComponent->SetupAttachment(RootComponent);
        XLineComponent->SetRelativeTransform(FTransform(FVector(0.f, static_cast<float>(i), 0.f), FVector(0.f, 0.f, 0.f), FVector(500.f, 1.f, 1.f)));

        ULineComp* YLineComponent = AddComponent<ULineComp>();
        YLineComponent->SetupAttachment(RootComponent);
        YLineComponent->SetRelativeTransform(FTransform(FVector(static_cast<float>(i), 0.f, 0.f), FVector(0.f, 0.f, 90.f), FVector(500.f, 1.f, 1.f)));

        // ULineComp* YLineComponent = AddComponent<ULineComp>();
    }

     //UConeComp* ConeComp1 = AddComponent<UConeComp>();
     //ConeComp1->SetRelativeTransform(FTransform(FVector(0.0f,2.0f, 1.0f), FQuat(0, 0.3, 0.7, 1), FVector(1.f, 1.f, 1.f)));
	   
     //RootComponent = CylinderComp;
     //ConeComp->SetupAttachment(CylinderComp);
     //ConeComp1->SetupAttachment(ConeComp);
    
}

void AWorldGrid::SetSpacing(float InSpacing)
{
    Spacing = InSpacing;
    FTransform Transform = GetActorRelativeTransform();
    Transform.SetScale(InSpacing, InSpacing, InSpacing);
    SetActorRelatvieTransform(Transform);
}

void AWorldGrid::BeginPlay()
{
    Super::BeginPlay();
}

void AWorldGrid::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
