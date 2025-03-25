#pragma once
#include<string>
#include "Core/Math/Vector.h"
#include "Core/Container/Array.h"
#include "Primitive/PrimitiveVertices.h"

struct FSubMeshData {
    TArray<FVertexPNCT> Vertices;
    TArray<uint32_t> Indices;
    std::string MaterialName;
};

struct FMaterialData {
    std::string Name;
    std::string DiffuseTexturePath;
    FVector DiffuseColor = { 1.0f, 1.0f, 1.0f };
};