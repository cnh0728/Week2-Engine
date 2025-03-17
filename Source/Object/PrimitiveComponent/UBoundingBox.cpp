#include "UBoundingBox.h"

void UBoundingBox::Tick(float DeltaTime)
{
	if (!Parent)
	{
		bCanBeRendered = false;
		return;
	}
	else
	{
		bCanBeRendered = true;
		UpdateMinMax();
	}
}

// 부모 component의 scale을 나눠서 구할 수 없음 -> override
const FMatrix UBoundingBox::GetComponentTransformMatrix()
{
	return OverrideBoxTransform.GetMatrix();
}

void UBoundingBox::UpdateMinMax()
{
	if (!Parent)
		return;

	UPrimitiveComponent* ParentCasted = dynamic_cast<UPrimitiveComponent*>(Parent);

	if (!ParentCasted)
	{
		return;
	}
	FMatrix ParentTransformMatrix = Parent->GetComponentTransformMatrix();

	FVector4 MaxCorner(-FLT_MAX, -FLT_MAX, -FLT_MAX, 1);
	FVector4 MinCorner(FLT_MAX, FLT_MAX, FLT_MAX, 1);
	TArray<FVertexSimple> Vertices = OriginVertices[ParentCasted->GetType()];
	for (const FVertexSimple& Vertex : Vertices)
	{
		FVector4 Vertex4 = FVector4(Vertex.X, Vertex.Y, Vertex.Z , 1.f);
		Vertex4 = Vertex4 * ParentTransformMatrix;
		
		// Affine transform에서는 w가 변하지 않음 -> w 나누기 생략
		MinCorner.X = min(MinCorner.X, Vertex4.X);
		MinCorner.Y = min(MinCorner.Y, Vertex4.Y);
		MinCorner.Z = min(MinCorner.Z, Vertex4.Z);
		MaxCorner.X = max(MaxCorner.X, Vertex4.X);
		MaxCorner.Y = max(MaxCorner.Y, Vertex4.Y);
		MaxCorner.Z = max(MaxCorner.Z, Vertex4.Z);
	}

	// 중점 찾기
	FVector BoxOrigin = (MinCorner + MaxCorner) / 2.f;
	//BoxOrigin = BoxOrigin - ParentTransformMatrix.GetTranslation();

	FVector Scale = (MaxCorner - MinCorner) / 2.f;
	
	this->OverrideBoxTransform = FTransform(BoxOrigin, FQuat(), Scale);
}
