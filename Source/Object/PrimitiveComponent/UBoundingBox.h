#pragma once
#include "UPrimitiveComponent.h"
class UBoundingBox :
    public UPrimitiveComponent
{
	using Super = USceneComponent;
public:
	UBoundingBox()
	{
		bCanBeRendered = false;
	}
	virtual ~UBoundingBox() = default;
	virtual void Tick(float DeltaTime) override;
	virtual const FMatrix GetComponentTransformMatrix() override;
	EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Bounding_Box;
	}

	FVector MinBoxCorner;
	FVector MaxBoxCorner;
	FTransform OverrideBoxTransform;
private:
	void UpdateMinMax();
	
};

