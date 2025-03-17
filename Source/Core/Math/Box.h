#pragma once
#include "Vector.h"
#include "Matrix.h"
#include "Primitive/PrimitiveVertices.h"
#include "Core/Container/Array.h"
#include "Transform.h"

class FBox
{
private:
	bool IsValid;
	FVector Min;
	FVector Max;

public:
	FBox();
	FBox(const TArray<FVertexSimple> &Vertices);
	FBox(const TArray<FVertexSimple> &Vertices, const FMatrix& TransformMatrix);
	FBox(const TArray<FVertexSimple> &Vertices, const FTransform& Transform);
	FBox(const TArray<FVector> &Vertices);
	FBox(const TArray<FVector> &Vertices, const FMatrix& TransformMatrix);
	FBox(const TArray<FVector> &Vertices, const FTransform& Transform);

	FBox(const FVector InMin, const FVector InMax);

	static FBox BuildAABB(const FVector Origin, const FVector Extent);
	bool Intersects(const FVector RayOrigin, const FVector RayDir, float& Distance);

	FVector GetCenter() const;
	FVector GetExtent() const;

private:
	FORCEINLINE void UpdateValid()
	{
		IsValid = (Min.X < Max.X && Min.Y < Max.Y && Min.Z < Max.Z);
	}
};

