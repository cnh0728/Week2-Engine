#pragma once
#include "Core/Container/Array.h"
#include "Core/Container/Map.h"
#include "Core/HAL/PlatformType.h"
#include "Core/Math/Vector.h"
#include "Core/Math/Matrix.h"

struct FVertexSimple
{
	float X, Y, Z;    // Position
	float R, G, B, A; // Color

	void SetPos(const FMatrix WorldMatrix)
	{
		FVector4 Position(X, Y, Z, 1.0f);
		Position = Position * WorldMatrix;
		X = Position.X / Position.W;
		Y = Position.Y / Position.W;
		Z = Position.Z / Position.W;
	}

	void SetColor(const FVector4 Color)
	{
		R = Color.X;
		G = Color.Y;
		B = Color.Z;
		A = Color.W;
	}
};

enum class EPrimitiveType : uint8
{
	EPT_None,
	EPT_Triangle,
	EPT_Cube,
	EPT_Sphere,
	EPT_Line,
	EPT_Cylinder,
	EPT_Cone,
	EPT_Max,
	EPT_Bounding_Box,
};

extern TMap<EPrimitiveType, TArray<FVertexSimple>> OriginVertices;
