#include "TRSAssimpLoader.h"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
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
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return nullptr;
    }
    // retrieve the directory path of the filepath
    m_strDirectory = file.substr(0, file.find_last_of('/'));

    // process ASSIMP's root node recursively
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
        aiMesh* pMesh = pScene->mMeshes[pNode->mMeshes[i]];
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
    // data to fill
    vector<Vertex> vertices;
    vector<unsigned int> indices;

    // Walk through each of the mesh's vertices
    for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
                          // positions
        vector.x = pMesh->mVertices[i].x;
        vector.y = pMesh->mVertices[i].y;
        vector.z = pMesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        vector.x = pMesh->mNormals[i].x;
        vector.y = pMesh->mNormals[i].y;
        vector.z = pMesh->mNormals[i].z;
        vertex.Normal = vector;
        // texture coordinates
        if (pMesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = pMesh->mTextureCoords[0][i].x;
            vec.y = pMesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        // tangent
        vector.x = pMesh->mTangents[i].x;
        vector.y = pMesh->mTangents[i].y;
        vector.z = pMesh->mTangents[i].z;
        vertex.Tangent = vector;
        // bitangent
        vector.x = pMesh->mBitangents[i].x;
        vector.y = pMesh->mBitangents[i].y;
        vector.z = pMesh->mBitangents[i].z;
        vertex.Bitangent = vector;
        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
    {
        aiFace face = pMesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    std::shared_ptr<TRSGeode> pGeode = std::make_shared<TRSGeode>();
    float* pData = (float*)(&vertices[0]);
    unsigned int* pIndice = &indices[0];
    //每个vertex顶点数据中有14个float
    pGeode->readFromVertex(pData, vertices.size() * sizeof(Vertex)/sizeof(float), EnAssimpFormat, pIndice, indices.size());
    TRSStateSet* pStateSet = pGeode->getOrCreateStateSet().get();
    TRSTexture* pCurTexture = pStateSet->getTexture();
    TRSShader* pShader = pStateSet->getShader();
    pShader->createProgram("shaders/3_1AssimpTextureVertex.glsl", "shaders/3_1AssimpTextureFragment.glsl");
    // process materials
    aiMaterial* material = pScene->mMaterials[pMesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN
    aiTextureType arrTexTypes[4] =
    {
        aiTextureType_DIFFUSE,
        aiTextureType_SPECULAR,
        aiTextureType_HEIGHT,
        aiTextureType_AMBIENT
    };
    std::string arrTexSampleNames[4] =
    {
        "texture_diffuse",
        "texture_specular",
        "texture_normal",
        "texture_height"
    };
    int nTexTypeCount = sizeof(arrTexTypes) / sizeof(arrTexTypes[0]);
    for (int i = 0; i < nTexTypeCount; i++)
    {
        aiTextureType textype = arrTexTypes[i];
        loadMaterialTextures(material, textype, arrTexSampleNames[i], pCurTexture);
    }
    return pGeode;
}

void TRSAssimpLoader::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string sampleName, TRSTexture* pCurTexture)
{
    int nTypeTexCount = mat->GetTextureCount(type);
    for (int i=0; i<nTypeTexCount; i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string strImageFile = m_strDirectory+"/"+ std::string(str.C_Str());
        TextureData texData;
        if (m_pGlobalTexture->getTextureDataByName(strImageFile, texData))
        {
            pCurTexture->addSharedTexture(texData);
        }
        else
        {
            int nID = m_pGlobalTexture->createTexture(strImageFile, sampleName);
            texData = TextureData(nID, strImageFile, sampleName);
            pCurTexture->addSharedTexture(texData);
        }
    }
}
