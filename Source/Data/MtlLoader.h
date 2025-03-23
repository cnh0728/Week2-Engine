#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include "Core/Container/Map.h"
#include "Core/Math/Vector.h"
#include "Core/AbstractClass/Singleton.h"

struct FMaterialData {
    std::string Name;
    std::string DiffuseTexturePath;
    FVector DiffuseColor = { 1.0f, 1.0f, 1.0f };
};

class MtlLoader : public TSingleton<MtlLoader> {
public:
    bool LoadMtlFile(const std::string& mtlPath);
};