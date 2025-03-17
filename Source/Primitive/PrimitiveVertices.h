#pragma once
#include "Core/Container/Array.h"
#include "Core/Container/Map.h"
#include "Core/HAL/PlatformType.h"

struct FVertexSimple
{
    float X, Y, Z;    // Position
    float R, G, B, A; // Color
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
	EPT_Text,
	EPT_Max,
	EPT_Bounding_Box,
};

extern TMap<EPrimitiveType, TArray<FVertexSimple>> OriginVertices;
