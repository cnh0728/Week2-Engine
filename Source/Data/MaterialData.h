#pragma once
#include<string>
#include "Core/Math/Vector.h"
struct FMaterialData {
    std::string Name;
    std::string DiffuseTexturePath;
    FVector DiffuseColor = { 1.0f, 1.0f, 1.0f };
};