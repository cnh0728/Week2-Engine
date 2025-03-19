#include "UBoundingBox.h"

void UBoundingBoxComponent::Tick(float DeltaTime)
{
	if (!Parent)
	{
		bIsDefaultRendered = false;
		return;
	}
	else
	{
		bIsDefaultRendered = true;
		UpdateMinMax();
	}
}

// 부모 component의 scale을 나눠서 구할 수 없음 -> override
FMatrix UBoundingBoxComponent::GetComponentTransformMatrix()
{
	return OverrideBoxTransform.GetMatrix();
}

void UBoundingBoxComponent::UpdateMinMax()
{
	if (!TargetPrimitive)
	{
		return;
	}
	FMatrix TargetTransformMatrix = TargetPrimitive->GetComponentTransformMatrix();

	BBox = FBox(OriginVertices[TargetPrimitive->GetType()], TargetTransformMatrix);
	if (BBox.IsValid())
	{
		this->OverrideBoxTransform = FTransform(BBox.GetCenter(), FQuat(), BBox.GetExtent());
	}
}
