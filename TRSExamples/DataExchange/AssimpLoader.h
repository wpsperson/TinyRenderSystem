#pragma once

#include <string>
#include <memory>

class TRSGroup;
class TRSNode;
class TRSTexture;
class TRSShader;

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;
enum aiTextureType;

using std::string;

class AssimpLoader
{
public:
    AssimpLoader();
    ~AssimpLoader();

    TRSGroup* getGroupNode() const;

    TRSGroup*  loadByAssimp(const std::string& file);

    void recurseNode(aiNode* pNode, const aiScene* pScene);

    std::shared_ptr<TRSNode> retrieveGeodeByMesh(aiMesh *pMesh, const aiScene *pScene);

private:
    TRSGroup* m_pGroupNode;
    std::string m_strDirectory;
    TRSTexture* m_pGlobalTexture;
};