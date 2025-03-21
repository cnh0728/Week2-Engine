#pragma once
#include "UPrimitiveComponent.h"
#include "Core/Math/Box.h"
#include "Object/ObjectMacros.h"
// Parent Component의 BoundingBox를 계산하고, 렌더합니다.
class UBoundingBoxComponent :
    public UPrimitiveComponent
{
	//using Super = USceneComponent;
	DECLARE_CLASS(UBoundingBoxComponent, UPrimitiveComponent)
public:
	UBoundingBoxComponent()
	{
		bIsDefaultRendered = true;
		bCanPick = false;
	}
	virtual ~UBoundingBoxComponent() = default;
	virtual void Tick(float DeltaTime) override;
	virtual FMatrix GetComponentTransformMatrix() override;
	EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_BoundingBox;
	}

	FBox BBox;
	FTransform OverrideBoxTransform;
private:
	void UpdateMinMax ();
	
private:
	UPrimitiveComponent* TargetPrimitive = nullptr;
public:
	void SetTargetPrimitive(UPrimitiveComponent* Target) { TargetPrimitive = Target; }
	
};

