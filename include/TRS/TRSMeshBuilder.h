#pragma once

#include <vector>
#include <unordered_map>
#include <utility>
#include "TRS/TRSExport.h"
#include "TRS/TRSVector.h"
#include "TRS/TRSDefStruct.h"

class TRSMesh;
using IndexPair = std::pair<unsigned int, unsigned int>;

class IndexPairHasher
{
public:
    std::size_t operator()(const IndexPair& pair) const;
};

class TRS_EXPORT TRSMeshBuilder
{
public:
    TRSMeshBuilder();

    ~TRSMeshBuilder();

    void setDivideLevel(int level);

    void buildSphere(const TRSVec3 &center, float radius, TRSMesh *mesh);

private:
    std::vector<unsigned int> initEightTriangle();

    void recurseTriangulate(int level, const TriIndex &tri);

    IndexPair makePair(unsigned int idx1, unsigned int idx2);

    TRSVec3 interpolateNormal(const TRSVec3& norm1, const TRSVec3& norm2);

    unsigned int getOrCreateMiddle(unsigned int idx1, unsigned int idx2);

private:
    TRSMesh* m_mesh = nullptr;
    int m_divideLevel = 4;
    TRSVec3 m_center;
    float m_radius = 1.0;

    std::unordered_map<IndexPair, unsigned int, IndexPairHasher> m_centerHash;
};