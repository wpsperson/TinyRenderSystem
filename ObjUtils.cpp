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
    double azimuthStep = (M_PI * 2) / (resolution);
    for (int i = 1; i <= resolution - 2; i++)
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
    int frontIdx = 1;
    int lastIdx = totalVertexNum;
    int offset = 1;// pola index
    for (int i = 1; i <= resolution - 1; i++)
    {
        for (int j = 1; j <= resolution; j++)
        {
            int first = j + offset;
            int second = j + offset + 1;
            if (j == resolution)
            {
                second = offset + 1;
            }
            if (i == 1)
                outputFace(stream, frontIdx, first, second);
            else if (i == resolution - 1)
                outputFace(stream, lastIdx, second - resolution, first - resolution);
            else
            {
                outputFace(stream, first, second, second - resolution);
                outputFace(stream, first, second - resolution, first - resolution);
            }
        }
        offset += resolution;
    }
    stream.close();
}

