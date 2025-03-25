#include "Box.h"

FBox::FBox()
{
	bIsValid = false;
	Min = Max = FVector(0, 0, 0);
}

FBox::FBox(const TArray<FVertexPNCT>& Vertices)
{
	Max = FVector(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	Min = FVector(FLT_MAX, FLT_MAX, FLT_MAX);
	for (const FVertexPNCT& Vertex : Vertices)
	{
		// Affine transform에서는 w가 변하지 않음 -> w 나누기 생략
		Min.X = min(Min.X, Vertex.X);
		Min.Y = min(Min.Y, Vertex.Y);
		Min.Z = min(Min.Z, Vertex.Z);
		Max.X = max(Max.X, Vertex.X);
		Max.Y = max(Max.Y, Vertex.Y);
		Max.Z = max(Max.Z, Vertex.Z);
	}
	//UpdateValid();
}

FBox::FBox(const TArray<FVertexPNCT>& Vertices, const FMatrix& TransformMatrix)
{

	Max = FVector(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	Min = FVector(FLT_MAX, FLT_MAX, FLT_MAX);
	for (const FVertexPNCT& Vertex : Vertices)
	{
		FVector4 Vertex4 = FVector4(Vertex.X, Vertex.Y, Vertex.Z, 1.f);
		Vertex4 = Vertex4 * TransformMatrix;
		
		// Affine transform에서는 w가 변하지 않음 -> w 나누기 생략
		Min.X = min(Min.X, Vertex4.X);
		Min.Y = min(Min.Y, Vertex4.Y);
		Min.Z = min(Min.Z, Vertex4.Z);
		Max.X = max(Max.X, Vertex4.X);
		Max.Y = max(Max.Y, Vertex4.Y);
		Max.Z = max(Max.Z, Vertex4.Z);
	}
	//UpdateValid();
}

FBox::FBox(const TArray<FVertexPNCT>& Vertices, const FTransform& Transform)
{
	FMatrix TransformMatrix = Transform.GetMatrix();
	Max = FVector(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	Min = FVector(FLT_MAX, FLT_MAX, FLT_MAX);
	for (const FVertexPNCT& Vertex : Vertices)
	{
		FVector4 Vertex4 = FVector4(Vertex.X, Vertex.Y, Vertex.Z, 1.f);
		Vertex4 = Vertex4 * TransformMatrix;

		// Affine transform에서는 w가 변하지 않음 -> w 나누기 생략
		Min.X = min(Min.X, Vertex.X);
		Min.Y = min(Min.Y, Vertex.Y);
		Min.Z = min(Min.Z, Vertex.Z);
		Max.X = max(Max.X, Vertex.X);
		Max.Y = max(Max.Y, Vertex.Y);
		Max.Z = max(Max.Z, Vertex.Z);
	}
	//UpdateValid();
}

FBox::FBox(const TArray<FVector>& Vertices)
{
	Max = FVector(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	Min = FVector(FLT_MAX, FLT_MAX, FLT_MAX);
	for (const FVector& Vertex : Vertices)
	{
		// Affine transform에서는 w가 변하지 않음 -> w 나누기 생략
		Min.X = min(Min.X, Vertex.X);
		Min.Y = min(Min.Y, Vertex.Y);
		Min.Z = min(Min.Z, Vertex.Z);
		Max.X = max(Max.X, Vertex.X);
		Max.Y = max(Max.Y, Vertex.Y);
		Max.Z = max(Max.Z, Vertex.Z);
	}
	//UpdateValid();
}

FBox::FBox(const TArray<FVector>& Vertices, const FMatrix& TransformMatrix)
{

	Max = FVector(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	Min = FVector(FLT_MAX, FLT_MAX, FLT_MAX);
	for (const FVector& Vertex : Vertices)
	{
		FVector4 Vertex4 = FVector4(Vertex.X, Vertex.Y, Vertex.Z, 1.f);
		Vertex4 = Vertex4 * TransformMatrix;
		
		// Affine transform에서는 w가 변하지 않음 -> w 나누기 생략
		Min.X = min(Min.X, Vertex.X);
		Min.Y = min(Min.Y, Vertex.Y);
		Min.Z = min(Min.Z, Vertex.Z);
		Max.X = max(Max.X, Vertex.X);
		Max.Y = max(Max.Y, Vertex.Y);
		Max.Z = max(Max.Z, Vertex.Z);
	}
	//UpdateValid();
}

FBox::FBox(const TArray<FVector>& Vertices, const FTransform& Transform)
{
	FMatrix TransformMatrix = Transform.GetMatrix();
	Max = FVector(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	Min = FVector(FLT_MAX, FLT_MAX, FLT_MAX);
	for (const FVector& Vertex : Vertices)
	{
		FVector4 Vertex4 = FVector4(Vertex.X, Vertex.Y, Vertex.Z, 1.f);
		Vertex4 = Vertex4 * TransformMatrix;

		// Affine transform에서는 w가 변하지 않음 -> w 나누기 생략
		Min.X = min(Min.X, Vertex.X);
		Min.Y = min(Min.Y, Vertex.Y);
		Min.Z = min(Min.Z, Vertex.Z);
		Max.X = max(Max.X, Vertex.X);
		Max.Y = max(Max.Y, Vertex.Y);
		Max.Z = max(Max.Z, Vertex.Z);
	}
	//UpdateValid();
}

FBox::FBox(const TArray<FStaticMesh>& RenderUnits, const FMatrix& TransformMatrix)
{
	Max = FVector(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	Min = FVector(FLT_MAX, FLT_MAX, FLT_MAX);
	for (const FStaticMesh& RenderUnit : RenderUnits)
	{
		for (const FVertexPNCT& Vertex : RenderUnit.Vertices)
		{
			FVector4 Vertex4 = FVector4(Vertex.X, Vertex.Y, Vertex.Z, 1.f);
			Vertex4 = Vertex4 * TransformMatrix;
			// Affine transform에서는 w가 변하지 않음 -> w 나누기 생략
			Min.X = min(Min.X, Vertex4.X);
			Min.Y = min(Min.Y, Vertex4.Y);
			Min.Z = min(Min.Z, Vertex4.Z);
			Max.X = max(Max.X, Vertex4.X);
			Max.Y = max(Max.Y, Vertex4.Y);
			Max.Z = max(Max.Z, Vertex4.Z);
		}
	}
}

FBox::FBox(const FVector InMin, const FVector InMax)
{
	Min = InMin;
	Max = InMax;
	//UpdateValid();
}

FBox FBox::BuildAABB(const FVector Origin, const FVector Extent)
{
	return FBox(Origin - Extent, Origin + Extent);
}

bool FBox::Intersects(const FVector RayOrigin, const FVector RayDir, float& Distance) const
{
	double tMin = -FLT_MAX;
	double tMax = FLT_MAX;
	if (fabs(RayDir.X) < FLT_EPSILON) {
		if (RayOrigin.X < Min.X || RayOrigin.X > Max.X)
			return false;
	}
	else {
		double t1 = (Min.X - RayOrigin.X) / RayDir.X;
		double t2 = (Max.X - RayOrigin.X) / RayDir.X;
		if (t1 > t2)
			std::swap(t1, t2);
		tMin = (tMin < t1) ? t1 : tMin;
		tMax = (tMax < t2) ? tMax : t2;
	}

	if (fabs(RayDir.Y) < FLT_EPSILON) {
		if (RayOrigin.Y < Min.Y || RayOrigin.Y > Max.Y)
			return false;
	}
	else {
		double t1 = (Min.Y - RayOrigin.Y) / RayDir.Y;
		double t2 = (Max.Y - RayOrigin.Y) / RayDir.Y;
		if (t1 > t2) std::swap(t1, t2);
		tMin = (tMin < t1) ? t1 : tMin;
		tMax = (tMax < t2) ? tMax : t2;
	}
	if (fabs(RayDir.Z) < FLT_EPSILON) {
		if (RayOrigin.Z < Min.Z || RayOrigin.Z > Max.Z)
			return false;
	}
	else
	{
		double t1 = (Min.Z - RayOrigin.Z) / RayDir.Z;
		double t2 = (Max.Z - RayOrigin.Z) / RayDir.Z;
		if (t1 > t2) std::swap(t1, t2);
		tMin = (tMin < t1) ? t1 : tMin;
		tMax = (tMax < t2) ? tMax : t2;
	}
	if (tMax >= tMin && tMax >= 0) {
		Distance = tMin; // world상의 거리는 밖에서 계산하기
		return true;
	}
	return false;
}

FVector FBox::GetCenter() const
{
	return(Max + Min) / 2.f;
}

FVector FBox::GetExtent() const
{
	return(Max - Min); // bounding box 는 -0.5 ~ 0.5 사이의 크기
}
