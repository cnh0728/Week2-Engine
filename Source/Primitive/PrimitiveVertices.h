#pragma once
#include "Core/Container/Array.h"
#include "Core/Container/Map.h"
#include "Core/HAL/PlatformType.h"
#include "Core/Math/Vector.h"
#include "Core/Math/Matrix.h"

struct FVertexPNCT
{
	float X, Y, Z;    // Position
	float NX, NY, NZ;
	float R, G, B, A; // Color
	float U, V;
	
	void SetPos(const FMatrix& WorldMatrix)
	{
		FVector4 Position(X, Y, Z, 1.0f);
		Position = Position * WorldMatrix;
		X = Position.X / Position.W;
		Y = Position.Y / Position.W;
		Z = Position.Z / Position.W;
	}

	void SetPos(const FVector& Position)
	{
		X = Position.X;
		Y = Position.Y;
		Z = Position.Z;
	}
	
	void SetVertexColor(const FVector4& Color)
	{
		R = Color.X;
		G = Color.Y;
		B = Color.Z;
		A = Color.W;
	}

	void SetNormal(const FVector& Normal)
	{
		NX = Normal.X;
		NY = Normal.Y;
		NZ = Normal.Z;
	}

	void SetUV(const FVector2 UV)
	{
		U = UV.X;
		V = UV.Y;
	}
	
public:
	static void CreateOriginVertices();
	
	static void CreateCubeVertices();
	static void CreateCylinderVertices();
	static void CreateTriangleVertices();
	static void CreateRingVertices();
	static void CreateConeVertices();
	static void CreateSphereVertices();
	static void CreateTextureBoardVertices();
	static void CreateBoundingBoxVertices();
	static void CreateSpotlightVertices();
};

enum EPrimitiveType : uint8
{
	EPT_None, //EPTNone은 항상 처음
	EPT_Triangle,
	EPT_Cube,
	EPT_Sphere,
	EPT_Line,
	EPT_Cylinder,
	EPT_Cone,
	EPT_Texture,
	EPT_BoundingBox,
	EPT_Ring,
	EPT_Spotlight,

	EPT_Custom = 100,
};

extern TMap<EPrimitiveType, TArray<FVertexPNCT>> OriginVertices;
extern TMap<EPrimitiveType, TArray<uint32_t>> OriginIndices;