
#include "glm\gtc\matrix_transform.hpp"



void CaseMatrixTransform()
{
    glm::mat4 mat;
    glm::mat4 newMat = translate(mat, glm::vec3(1, 0, 0));
    glm::mat4 newMat2 = translate(newMat, glm::vec3(3, 3, 3));
    return ;
}




