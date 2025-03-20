#include "UBoundingBox.h"
#include "BillBoardComponent.h"

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
	if (TargetPrimitive->IsA(UBillBoardComponent::StaticClass()))
	{
		BBox = FBox();
		return;
	}

	//if (UBillBoardComponent* BillBoard = Cast<UBillBoardComponent>(TargetPrimitive))
	//{
	//	FMatrix BillboardTransformMatrix = BillBoard->GetComponentTransformMatrix();

	//	TArray<FVector> BillboardVertices = { { }}
	//	BBox = FBox(, BillboardTransformMatrix);
	//	if (BBox.IsValid())
	//	{
	//		this->OverrideBoxTransform = FTransform(BBox.GetCenter(), FQuat(), BBox.GetExtent());
	//	}
	//}
	//else
	{
		FMatrix TargetTransformMatrix = TargetPrimitive->GetComponentTransformMatrix();

		BBox = FBox(OriginVertices[TargetPrimitive->GetType()], TargetTransformMatrix);
		if (BBox.IsValid())
		{
			this->OverrideBoxTransform = FTransform(BBox.GetCenter(), FQuat(), BBox.GetExtent());
		}
	}
}
