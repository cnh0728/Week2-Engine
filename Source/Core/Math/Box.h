#pragma once
#include "Vector.h"
#include "Matrix.h"
#include "Primitive/PrimitiveVertices.h"
#include "Core/Container/Array.h"
#include "Transform.h"
#include "Object/PrimitiveComponent/StaticMeshComponent.h"

class FBox
{
private:
	bool bIsValid;
	FVector Min;
	FVector Max;

public:
	FBox();
	FBox(const TArray<FVertexPNCT> &Vertices);
	FBox(const TArray<FVertexPNCT> &Vertices, const FMatrix& TransformMatrix);
	FBox(const TArray<FVertexPNCT> &Vertices, const FTransform& Transform);
	FBox(const TArray<FVector> &Vertices);
	FBox(const TArray<FVector> &Vertices, const FMatrix& TransformMatrix);
	FBox(const TArray<FVector> &Vertices, const FTransform& Transform);
	FBox(const TArray<FStaticMesh> &RenderUnits, const FMatrix& TransformMatrix);
	FBox(const FVector InMin, const FVector InMax);

	static FBox BuildAABB(const FVector Origin, const FVector Extent);
	bool Intersects(const FVector RayOrigin, const FVector RayDir, float& Distance) const;

	FVector GetCenter() const;
	FVector GetExtent() const;

	bool IsValid() const { return bIsValid; }

private:
	FORCEINLINE void UpdateValid()
	{
		bIsValid = (Min.X < Max.X && Min.Y < Max.Y && Min.Z < Max.Z);
	}
};

