#include "ObjLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Core/Rendering/URenderer.h"
#include "Primitive/PrimitiveVertices.h"

bool ObjLoader::LoadFromFile(const std::string& filename)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename, 
        aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::string errorMessage = importer.GetErrorString();
        std::cout << "Assimp 오류: " << errorMessage << std::endl;
        return false;
    }

    URenderer* Renderer = UEngine::Get().GetRenderer();
    // 메시 데이터 추출

    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        TArray<FVertexSimple> Vertices;
        TArray<uint32_t> Indices;

        // 정점, 인덱스, 텍스처 데이터 처리

        for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
            FVertexSimple Vertex;
            Vertex.SetPos({ mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z });
            Vertex.SetVertexColor({0.f,0.f,0.f,1.f});
            Vertex.SetNormal({ mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z});
            Vertex.SetUV({ mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y });
            Vertices.Add(Vertex);
        }

        for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
            aiFace face = mesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; k++) {
                Indices.Add(face.mIndices[k]);
            }
        }

        OriginVertices.Add(EPT_Custom, Vertices);
        OriginIndices.Add(EPT_Custom, Indices);
    }

    return true;
}
