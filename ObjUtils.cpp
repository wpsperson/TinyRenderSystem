#include "ObjUtils.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>


void outputVertex(std::ofstream& stream, double x, double y, double z)
{
    stream << "v " << std::to_string(x) << " " << std::to_string(y) << " "<<std::to_string(z) << std::endl;
}
void outputFace(std::ofstream& stream, int id0, int id1, int id2)
{
    stream << "f " << std::to_string(id0) << " " << std::to_string(id1) << " " << std::to_string(id2) << std::endl;
}

void generateSphereObj(double radius, int resolution, const std::string& fileName)
{
    std::ofstream stream(fileName.c_str());
    if (stream.fail())
    {
        return;
    }
    if (resolution < 3)
    {
        return;
    }
    // output vertex
    int totalVertexNum = 1;
    outputVertex(stream, 0, 0, radius);
    double elevationStep = M_PI / (resolution - 1);
    double azimuthStep = M_PI / (resolution);
    for (int i=1; i<resolution-1; i++)
    {
        double elev = i * elevationStep;
        for (int j = 0; j < resolution; j++)
        {
            double azimuth = azimuthStep * j;
            double z = radius * std::cos(elev);
            double x = radius * std::sin(elev) * std::cos(azimuth);
            double y = radius * std::sin(elev) * std::sin(azimuth);
            outputVertex(stream, x, y, z);
            totalVertexNum++;
        }
    }
    outputVertex(stream, 0, 0, -radius);
    totalVertexNum++;

    stream << std::endl;
    // output vertex
    for (int j=0; j<resolution; j++)
    {
        int first = j + 1;
        int second = j + 2;
        if (j == resolution-1)
        {
            second = 1;
        }
        outputFace(stream, 0, j + 1, j + 2);
    }



    stream.close();
}

