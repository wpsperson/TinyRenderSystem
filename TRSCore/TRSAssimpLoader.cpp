#include "TRSAssimpLoader.h"
#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "TRSGeode.h"
#include "TRSGroup.h"
#include "TRSTexture.h"
#include "TRSStateSet.h"
#include "TRSNode.h"
#include "TRSShader.h"

using namespace std;

struct Vertex {
    // position
    TRSVec3 Position;
    // normal
    TRSVec3 Normal;
    // texCoords
    TRSVec2 TexCoords;
};

TRSAssimpLoader::TRSAssimpLoader()
{
    m_pGlobalTexture = new TRSTexture();
    m_pGroupNode = new TRSGroup;
}

TRSAssimpLoader::~TRSAssimpLoader()
{

}


TRSGroup* TRSAssimpLoader::getGroupNode() const
{
    return m_pGroupNode;
}

TRSGroup* TRSAssimpLoader::loadByAssimp(const std::string& file)
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

void TRSAssimpLoader::recurseNode(aiNode* pNode, const aiScene* pScene)
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

std::shared_ptr<TRSNode> TRSAssimpLoader::retrieveGeodeByMesh(aiMesh *pMesh, const aiScene *pScene)
{
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.Position[0] = pMesh->mVertices[i].x;
        vertex.Position[1] = pMesh->mVertices[i].y;
        vertex.Position[2] = pMesh->mVertices[i].z;
        if (pMesh->mNormals)
        {
            vertex.Normal[0] = pMesh->mNormals[i].x;
            vertex.Normal[1] = pMesh->mNormals[i].y;
            vertex.Normal[2] = pMesh->mNormals[i].z;
        }
        if (pMesh->mTextureCoords[0])
        {
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vertex.TexCoords[0] = pMesh->mTextureCoords[0][i].x;
            vertex.TexCoords[1] = pMesh->mTextureCoords[0][i].y;
        }
        else
        {
            vertex.TexCoords = TRSVec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }
    for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
    {
        aiFace face = pMesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    std::shared_ptr<TRSGeode> pGeode = std::make_shared<TRSGeode>();
    float* pData = (float*)(&vertices[0]);
    unsigned int* pIndice = &indices[0];
    //每个vertex顶点数据中有14个float
    pGeode->readFromVertex(pData, vertices.size() * sizeof(Vertex)/sizeof(float), EnVertexNormTexture, pIndice, indices.size());
    TRSStateSet* pStateSet = pGeode->getOrCreateStateSet().get();
    TRSTexture* pCurTexture = pStateSet->getTexture();
    TRSShader* pShader = pStateSet->getShader();
    createShaderByMesh(pMesh, pShader);

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

int TRSAssimpLoader::createShaderByMesh(aiMesh *pMesh, TRSShader* shader)
{
    bool bHasNormal = pMesh->mNormals;
    bool bHasTexture = (pMesh->mTextureCoords && pMesh->mTextureCoords[0]);
    if (bHasNormal && !bHasTexture)
    {
        shader->createProgram("shaders/PhongVertex.glsl", "shaders/PhongFragment.glsl");
        return EnVertexNormal;
    }
    else if (!bHasNormal && bHasTexture)
    {
        throw "to do";
        return EnVertexTexture;
    }
    else if (bHasNormal && bHasTexture)
    {
        shader->createProgram("shaders/PosNormTexVertex.glsl", "shaders/PosNormTexFragment.glsl");
        return EnVertexNormTexture;
    }

    shader->createProgram("shaders/DefaultVertex.glsl", "shaders/DefaultFragment.glsl");
    return EnVertex;
}
