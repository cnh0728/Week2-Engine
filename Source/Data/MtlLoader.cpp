
#include "MtlLoader.h"
#include <iostream>
#include "Static/ResourceManager.h"

bool MtlLoader::LoadMtlFile(const std::string& mtlPath) {
    std::ifstream file(mtlPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open MTL file: " << mtlPath << std::endl;
        return false;
    }

    FMaterialData currentMaterial;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "newmtl") {
            if (!currentMaterial.Name.empty()) {
                UResourceManager::Get().SetMaterial(currentMaterial.Name, currentMaterial);
            }
            iss >> currentMaterial.Name;
            currentMaterial = FMaterialData();
            currentMaterial.Name = token;
        }
        else if (token == "Kd") {
            float r, g, b;
            iss >> r >> g >> b;
            currentMaterial.DiffuseColor = { r, g, b };
        }
        else if (token == "map_Kd") {
            iss >> currentMaterial.DiffuseTexturePath;
        }
    }
    if (!currentMaterial.Name.empty()) {
        UResourceManager::Get().SetMaterial(currentMaterial.Name, currentMaterial);
    }

    return true;
}


}