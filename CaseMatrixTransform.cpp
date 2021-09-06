
#define _USE_MATH_DEFINES
#include <cmath>
#include "glm\gtc\matrix_transform.hpp"
#include "TRSMatrix.h"
#include <iostream>
#include "TRSVector.h"

double epsilon = 1e-6;
bool judgetSame(const TRSMatrix& m1, const glm::mat4& m2)
{
    for (int i=0; i<4; i++)
    {
        for (int j=0; j<4; j++)
        {
            double delta = m1[i][j] - m2[i][j];
            if ( std::fabs(delta) > epsilon )
            {
                return false;
            }
        }
    }
    return true;
}


void CaseMatrixTransform()
{
    //glm::mat4 mat;
    //glm::mat4 newMat = translate(mat, glm::vec3(1, 0, 0));
    //glm::mat4 newMat2 = translate(newMat, glm::vec3(3, 3, 3));
    //glm::vec4 pt(0, 0, 0, 1);
    //glm::vec4 newPt = newMat2 * pt;
    //glm::vec4 newPt2 = pt * newMat2;


    //double zvalue = M_PI / 2;
    //TRSMatrix matrix;
    //matrix.makeRotate(zvalue, 1, 0, 0);
    //glm::mat4 second = rotate(mat, float(zvalue), glm::vec3(1, 0, 0));
    //bool same = judgetSame(matrix, second);
    //std::cout << "angle:" << zvalue << ", " << same << std::endl;
    //for (double zvalue = 0; zvalue<M_PI; zvalue+=0.01)
    //{
    //    TRSMatrix matrix;
    //    matrix.makeRotate(zvalue, 1, 0, 0);
    //    glm::mat4 second = rotate(mat, float(zvalue), glm::vec3(1, 0, 0));
    //    bool same = judgetSame(matrix, second);
    //    std::cout << "angle:" << zvalue << ", " << same << std::endl;
    //}
    //for (double zvalue = 0; zvalue < 3; zvalue += 0.01)
    //{
    //    TRSMatrix matrix;
    //    matrix.makeRotate(M_PI_4, 1, 1, zvalue);
    //    glm::mat4 second = rotate(mat, float(M_PI_4), glm::vec3(1, 1, zvalue));
    //    bool same = judgetSame(matrix, second);
    //    std::cout << "angle:" << zvalue << ", " << same << std::endl;
    //}

    glm::mat4 mat = glm::ortho<double>(-2, 3, 0, 4, 0, 100);
    TRSMatrix trsMat;
    trsMat.makeOtho(-2, 3, 0, 4, 0, 100);
    bool same = judgetSame(trsMat, mat);

    return ;
}




