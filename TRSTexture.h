/*!  
 *@brief    ����
 *@author   wangps
 *@date     2019��5��3��
 */

#pragma once
#include <vector>
#include <string>
#include "TRSExport.h"

//�������ݣ�����һ����������ݽṹ
struct TextureData
{
    unsigned int id;
    std::string strImageFile;
    std::string strSampleName;
    TextureData()
    {

    }
    TextureData(unsigned int nID, std::string strImage, std::string strName)
        :id(nID), strImageFile(strImage), strSampleName(strName)
    {

    }
};

class TRS_EXPORT TRSTexture
{
public:
    TRSTexture();
    TRSTexture(int texCount, ...);
    ~TRSTexture();

    int createTexture(const std::string& imageFile, const std::string& sampleName = "");

    void activeAllTextures(unsigned int program);

    //���������ʹ����ͬ������ͼƬ�����Թ���
    void addSharedTexture(const TextureData& textureData);

    //��ѯ����������Ƿ�����ļ���ΪimageFile����������У�������������
    bool getTextureDataByName(const std::string& imageFile, TextureData& outData);

    int count();

    std::string debugInfo();

protected:
    std::vector<unsigned int> m_nTextures;      //����ID
    std::vector<std::string> m_sImageFileNames; //����ͼƬ��·��
    std::vector<std::string> m_sSampleNames;    //��ɫ���в�����������
};

