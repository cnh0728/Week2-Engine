#include "MtlLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "Static/ResourceManager.h"
#include "Data//MaterialData.h"

namespace fs = std::filesystem;

bool MtlLoader::LoadMtlFile(const std::string& mtlPath) {
    fs::path finalMtlPath = fs::path(mtlPath);
    if (!finalMtlPath.is_absolute()) {
        finalMtlPath = fs::absolute(fs::path("Materials") / finalMtlPath);  //절대경로인지 확인 -> 아니면 Materials 폴더 내에 경로 만들어줌
    }

    std::ifstream file(finalMtlPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open MTL file: " << finalMtlPath << std::endl;
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

            std::string newName;
            iss >> newName;
            currentMaterial = FMaterialData();
            currentMaterial.Name = newName;
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
