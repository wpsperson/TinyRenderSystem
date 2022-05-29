/*!  
 *@brief    
 *@author   wangps
 *@date     2019年7月8日
 */

#pragma once
#include "TRSExport.h"
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

class TRS_EXPORT TRSAssimpLoader
{
public:
    TRSAssimpLoader();
    ~TRSAssimpLoader();

    TRSGroup* getGroupNode() const;
    
    TRSGroup*  loadByAssimp(const std::string& file);
    void recurseNode(aiNode* pNode, const aiScene* pScene);
    std::shared_ptr<TRSNode> retrieveGeodeByMesh(aiMesh *pMesh, const aiScene *pScene);

protected:
    int createShaderByMesh(aiMesh *pMesh, TRSShader* shader);

private:
    TRSGroup* m_pGroupNode;
    std::string m_strDirectory;
    TRSTexture* m_pGlobalTexture;
};