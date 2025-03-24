#pragma once
#include <string>
#include <DirectXMath.h>
#include "Core/AbstractClass/Singleton.h"
#include "Core/Container/Array.h"
#include "Core/Container/Map.h"
#include "Primitive/PrimitiveVertices.h"


class ObjLoader : public TSingleton<ObjLoader> {
public:
    ObjLoader() = default;
    ~ObjLoader() = default;
    bool LoadFromFile(const std::string& filename);
};
