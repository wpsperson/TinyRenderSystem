/*!  
 *@brief    ���
 *@author   wangps
 *@date     2019��5��3��
 */

#pragma once
#include "TRSExport.h"
#include "glm/glm.hpp"
class GLFWwindow;

class TRS_EXPORT TRSCamera
{
public:
    TRSCamera();
    ~TRSCamera();

    void initMember();

    glm::mat4 getViewMatrix();

    glm::mat4 getProjectMatrix();

    void keyboardInput(GLFWwindow* pWindow);

protected:
    glm::vec3 m_pos;        //�����ǰλ��
    glm::vec3 m_front;      //�����ǰ���������ǰλ��ָ��۲����ĵ������
    glm::vec3 m_right;      //��������ַ��򣬼��ֲ�����ϵ��X��������������ϵ�е�����
    glm::vec3 m_up;         //�����ͷ�����򣬼��ֲ�����ϵ��Y��������������ϵ�е�����

    float m_fMoveSpeed;     //wsad�����ƶ�ϵ��
    float m_fMouseSensity;  //����ƶ�������ϵ��

    float m_fPitch;         //��������ƶ������ĸ����� -89~89
    float m_fYaw;           //��������ƶ�������ƫ���� ������
    float m_fZoom;          //�����ֹ���������fov�Ŵ���С 45-90
    GLFWwindow* m_pWindow;
};

