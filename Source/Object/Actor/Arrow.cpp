﻿#include "Arrow.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"

AArrow::AArrow()
{
	bCanEverTick = true;
	
	// test
	UCylinderComp* CylinderComp = AddComponent<UCylinderComp>();

	CylinderComp->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.5f), FQuat(0, 0, 0, 1), FVector(1.f, 1.f, 1.f)));

	UConeComp* ConeComp = AddComponent<UConeComp>();
	ConeComp->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 1.0f), FQuat(0, 0, 0, 1), FVector(1.f, 1.f, 1.f)));


	UConeComp* ConeComp1 = AddComponent<UConeComp>();
	ConeComp1->SetRelativeTransform(FTransform(FVector(0.0f,2.0f, 1.0f), FQuat(0, 0.3, 0.7, 1), FVector(1.f, 1.f, 1.f)));
	
	RootComponent = CylinderComp;
	ConeComp->SetupAttachment(CylinderComp);
	ConeComp1->SetupAttachment(ConeComp);
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
