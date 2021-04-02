/*!  
 *@brief    
 *@author   wangps
 *@date     2019年7月8日
 */

#pragma once
#include "TRSExport.h"
#include <string>
#include <memory>
#include "assimp/material.h"
class TRSGroup;
class TRSNode;
class TRSTexture;

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;

using std::string;

class TRS_EXPORT TRSAssimpLoader
{
public:
    TRSAssimpLoader();
    ~TRSAssimpLoader();

    TRSGroup* getGroupNode() const;
    
    TRSGroup*  loadByAssimp(const std::string& file);
    void recurseNode(aiNode* pNode, const aiScene* pScene);
    std::shared_ptr<TRSNode> retrieveGeodeByMesh(aiMesh *pMesh, const aiScene *pScene);
    void loadMaterialTextures(aiMaterial *mat, aiTextureType type, string sampleName, TRSTexture* pCurTexture);
private:
    TRSGroup* m_pGroupNode;
    std::string m_strDirectory;
    TRSTexture* m_pGlobalTexture;
};