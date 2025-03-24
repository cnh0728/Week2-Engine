#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include "Core/Container/Map.h"
#include "Core/Math/Vector.h"
#include "Core/AbstractClass/Singleton.h"


class MtlLoader  {
public:
    bool LoadMtlFile(const std::string& mtlPath);

private:
    std::string MtlDir = "Materials/";
};