#pragma once
#include "Object/Actor/Actor.h"
#include "Object/PrimitiveComponent/UBoundingBox.h"
#include "Object/ObjectMacros.h"

enum class ESelectedAxis : uint8
{
	None,
	X,
	Y,
	Z
};

enum class EGizmoType : uint8
{
	Translate,
	Rotate,
	Scale,
	Max
};

class AGizmoHandle : public AActor
{
	DECLARE_CLASS(AGizmoHandle, AActor)
public:
	AGizmoHandle();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void SetActive(bool bActive);
	void SetSelectedAxis(ESelectedAxis NewAxis) { SelectedAxis = NewAxis; }
	ESelectedAxis GetSelectedAxis() const { return SelectedAxis; }
	EGizmoType GetGizmoType() const { return GizmoType; }
	void DoTransform(FTransform& AT, FVector Result, AActor* Actor);

private:
	bool bIsActive = false;
	TArray<class UCylinderComp*> CylinderComponents;

	ESelectedAxis SelectedAxis = ESelectedAxis::None;
	EGizmoType GizmoType = EGizmoType::Translate;
	void UpdateGizmoPrimitives();

	//virtual const char* GetTypeName() override;
private:
	TSet<UBoundingBoxComponent*> SelectedActorBoundingBox;


	UCubeComp* GizmoCube;
	UCylinderComp* XArrowBody;
	UConeComp* XArrowHead;
	UCylinderComp* YArrowBody;
	UConeComp* YArrowHead;
	UCylinderComp* ZArrowBody;
	UConeComp* ZArrowHead;
	URingComp* XRing;
	URingComp* YRing;
	URingComp* ZRing;
	UCubeComp* XRodHead;
	UCubeComp* YRodHead;
	UCubeComp* ZRodHead;
};

