#include "Axis.h"

AAxis::AAxis()
{
	bIsGizmo = true;

	RootComponent = AddComponent<USceneComponent>();
	
	ULineComp* LineX = AddComponent<ULineComp>();
	FTransform XTransform = LineX->GetRelativeTransform();
	XTransform.SetScale(FVector(1000.0f, 10.0f, 10.0f));
	XTransform.Rotate({0.0f, 0.0f, 0.0f});
	LineX->SetRelativeTransform(XTransform);
	LineX->SetColor(FVector4(1.0f, 0.0f, 0.0f, 1.0f));
	LineX->SetupAttachment(RootComponent);

	ULineComp* LineY = AddComponent<ULineComp>();
	FTransform YTransform = LineY->GetRelativeTransform();
	YTransform.SetScale(FVector(1000.0f, 10.0f, 10.0f));
	YTransform.Rotate({0.0f, 0.0f, 90.0f});
	LineY->SetRelativeTransform(YTransform);
	LineY->SetColor(FVector4(0.0f, 1.0f, 0.0f, 1.0f));
	LineY->SetupAttachment(RootComponent);


	ULineComp* LineZ = AddComponent<ULineComp>();
	FTransform ZTransform = LineZ->GetRelativeTransform();
	ZTransform.SetScale(FVector(1000.0f, 10.0f, 10.0f));
	ZTransform.Rotate({0.0f, 90.0f, 0.0f});
	LineZ->SetRelativeTransform(ZTransform);
	LineZ->SetColor(FVector4(0.0f, 0.0f, 1.0f, 2.0f));
	LineZ->SetupAttachment(RootComponent);
} 

void AAxis::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAxis::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const char* AAxis::GetTypeName()
{
	return "Axis";
}
