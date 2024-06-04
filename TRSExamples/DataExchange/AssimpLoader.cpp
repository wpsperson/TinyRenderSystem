#include "AssimpLoader.h"
#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "TRS/TRSDefEnum.h"
#include "TRS/TRSGeode.h"
#include "TRS/TRSGroup.h"
#include "TRS/TRSTexture.h"
#include "TRS/TRSNode.h"
#include "TRS/TRSShader.h"

using namespace std;


AssimpLoader::AssimpLoader()
{
    m_pGlobalTexture = new TRSTexture();
    m_pGroupNode = new TRSGroup;
}

AssimpLoader::~AssimpLoader()
{

}


TRSGroup* AssimpLoader::getGroupNode() const
{
    return m_pGroupNode;
}

TRSGroup* AssimpLoader::loadByAssimp(const std::string& file)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "loadByAssimp fails: " << importer.GetErrorString() << std::endl;
        return nullptr;
    }
    m_strDirectory = file.substr(0, file.find_last_of('/'));
    recurseNode(scene->mRootNode, scene);
    return m_pGroupNode;
}

void AssimpLoader::recurseNode(aiNode* pNode, const aiScene* pScene)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < pNode->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        int index = pNode->mMeshes[i];
        aiMesh* pMesh = pScene->mMeshes[index];
        std::shared_ptr<TRSNode> pNode = retrieveGeodeByMesh(pMesh, pScene);
        m_pGroupNode->addChild(pNode);
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < pNode->mNumChildren; i++)
    {
        recurseNode(pNode->mChildren[i], pScene);
    }
}

std::shared_ptr<TRSNode> AssimpLoader::retrieveGeodeByMesh(aiMesh *pMesh, const aiScene *pScene)
{
    unsigned int vertexCount = pMesh->mNumVertices;
    unsigned int indexCount = pMesh->mNumFaces * 3;

    vector<TRSVec3> points;
    vector<TRSVec3> normals;
    vector<TRSVec2> UVs;
    vector<unsigned int> indices;

    points.reserve(vertexCount);
    normals.reserve(vertexCount);
    UVs.reserve(vertexCount);
    indices.reserve(indexCount);

    TRSVec3 point;
    TRSVec3 normal;
    TRSVec2 UV;
    for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
    {
        point[0] = pMesh->mVertices[i].x;
        point[1] = pMesh->mVertices[i].y;
        point[2] = pMesh->mVertices[i].z;
        points.emplace_back(point);
        if (pMesh->mNormals)
        {
            normal[0] = pMesh->mNormals[i].x;
            normal[1] = pMesh->mNormals[i].y;
            normal[2] = pMesh->mNormals[i].z;
            normals.emplace_back(normal);
        }
        if (pMesh->mTextureCoords[0])
        {
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            UV[0] = pMesh->mTextureCoords[0][i].x;
            UV[1] = pMesh->mTextureCoords[0][i].y;
            UVs.emplace_back(UV);
        }
    }
    for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
    {
        aiFace face = pMesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    std::shared_ptr<TRSGeode> pGeode = std::make_shared<TRSGeode>();
    //each vertex has 14 float
    //float* pData = (float*)(&vertices[0]);
    //unsigned int* pIndice = &indices[0];
    //pGeode->readFromVertex(pData, vertices.size() * sizeof(Vertex)/sizeof(float), EnVertexNormTexture, pIndice, indices.size());
    pGeode->setMeshData(points, normals, UVs, std::vector<TRSVec3>());
    pGeode->setIndexArray(indices);
    TRSTexture* pCurTexture = pGeode->getTexture();

    aiMaterial* material = pScene->mMaterials[pMesh->mMaterialIndex];
    // current stage, we just focus on diffuse texture;
    std::string strSampleName = "texture_diffuse";
    int nTypeTexCount = material->GetTextureCount(aiTextureType_DIFFUSE);
    for (int i = 0; i < nTypeTexCount; i++)
    {
        aiString imageName;
        material->GetTexture(aiTextureType_DIFFUSE, i, &imageName);
        std::string strImageFile = m_strDirectory + "/" + std::string(imageName.C_Str());
        TextureData texData;
        if (m_pGlobalTexture->getTextureDataByName(strImageFile, texData))
        {
            pCurTexture->addSharedTexture(texData);
        }
        else
        {
            int nID = m_pGlobalTexture->createTexture(strImageFile, strSampleName);
            texData = TextureData(nID, strImageFile, strSampleName);
            pCurTexture->addSharedTexture(texData);
        }
    }
    return pGeode;
}
