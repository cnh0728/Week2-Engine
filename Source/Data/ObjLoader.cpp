#include "ObjLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Core/Rendering/URenderer.h"
#include "Primitive/PrimitiveVertices.h"
#include "Static/ResourceManager.h"
#include <sstream>

bool ObjLoader::LoadFromFile(const std::string& filename)
{
    // 1. mtllib 파싱
    std::ifstream objFile(filename);
    if (objFile.is_open())
    {
        std::string line;
        while (std::getline(objFile, line))
        {
            std::istringstream iss(line);
            std::string token;
            iss >> token;
            if (token == "mtllib")
            {
                std::string mtlFile;
                iss >> mtlFile;

                UResourceManager::Get().LoadMtlFile(mtlFile);
                break; // 첫 mtllib만 처리
            }
        }
    }

    // 2. Assimp로 obj 로딩
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename,
        aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::string errorMessage = importer.GetErrorString();
        std::cout << "Assimp 오류: " << errorMessage << std::endl;
        return false;
    }

    TArray<FSubMeshData> SubMeshes;

    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        FSubMeshData subMesh;

        for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
            FVertexSimple Vertex;
            Vertex.SetPos({ mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z });
            Vertex.SetVertexColor({ 1.f, 1.f, 1.f, 1.f });
            Vertex.SetNormal({ mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z });

            if (mesh->HasTextureCoords(0))
                Vertex.SetUV({ mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y });
            else
                Vertex.SetUV({ 0, 0 });

            subMesh.Vertices.Add(Vertex);
        }

        for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
            aiFace face = mesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; k++) {
                subMesh.Indices.Add(face.mIndices[k]);
            }
        }

        if (scene->HasMaterials()) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            aiString matName;
            material->Get(AI_MATKEY_NAME, matName);
            subMesh.MaterialName = matName.C_Str();
        }

        SubMeshes.Add(subMesh);
    }

    UResourceManager::Get().SetMeshData(filename, SubMeshes);

    return true;
}

