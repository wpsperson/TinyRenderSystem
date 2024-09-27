#include "TRS/TRSMeshBuilder.h"

#include <cmath>

#include "TRS/TRSMesh.h"

TRSMeshBuilder::TRSMeshBuilder()
{
}

TRSMeshBuilder::~TRSMeshBuilder()
{
}

void TRSMeshBuilder::setDivideLevel(int level)
{
    m_divideLevel = level;
}

void TRSMeshBuilder::buildSphere(const TRSVec3& center, float radius, TRSMesh* mesh)
{
    m_mesh = mesh;
    m_center = center;
    m_radius = radius;
    m_centerHash.clear();

    mesh->reset();
    int pointNum = static_cast<int>( std::pow(2, m_divideLevel + 1) )+ 2;
    int triNum = static_cast<int>(std::pow(2, m_divideLevel * 2));
    std::vector<TRSVec3> &points = mesh->getVertices();
    std::vector<TRSVec3> &normals = mesh->getNormals();
    std::vector<unsigned int> &indices = mesh->getIndices();
    points.reserve(pointNum);
    normals.reserve(pointNum);
    indices.reserve(triNum);

    // first initialize the eight triangles.
    std::vector<unsigned int> start_indices = initEightTriangle();

    int level = 1;
    std::size_t indice_size = start_indices.size() / 3;
    for (int idx = 0; idx < indice_size; idx ++)
    {
        TriIndex tri;
        tri.idxa = start_indices[idx * 3 + 0];
        tri.idxb = start_indices[idx * 3 + 1];
        tri.idxc = start_indices[idx * 3 + 2];
        recurseTriangulate(level, tri);
    }

    mesh->setNeedUpload();
}

std::vector<unsigned int> TRSMeshBuilder::initEightTriangle()
{
    std::vector<TRSVec3>& points = m_mesh->getVertices();
    std::vector<TRSVec3>& normals = m_mesh->getNormals();

    // initiate 8 octant sphere triangles
    float xx = m_center[0];
    float yy = m_center[1];
    float zz = m_center[2];
    TRSVec3 top(xx, yy, zz + m_radius);
    TRSVec3 btm(xx, yy, zz - m_radius);
    TRSVec3 left(xx - m_radius, yy, zz);
    TRSVec3 right(xx + m_radius, yy, zz);
    TRSVec3 front(xx, yy - m_radius, zz);
    TRSVec3 back(xx, yy + m_radius, zz);
    unsigned int id = 0;

    points.emplace_back(top);  normals.emplace_back(G_ZDIR);    unsigned int topId = id++;
    points.emplace_back(btm); normals.emplace_back(-G_ZDIR);    unsigned int btmId = id++;
    points.emplace_back(left); normals.emplace_back(-G_XDIR);    unsigned int leftId = id++;
    points.emplace_back(right); normals.emplace_back(G_XDIR);    unsigned int rightId = id++;
    points.emplace_back(front); normals.emplace_back(-G_YDIR);    unsigned int frontId = id++;
    points.emplace_back(back); normals.emplace_back(G_YDIR);    unsigned int backId = id++;

    std::vector<unsigned int> indices;
    indices.reserve(8 * 3);

    indices.emplace_back(topId);
    indices.emplace_back(rightId);
    indices.emplace_back(backId);

    indices.emplace_back(topId);
    indices.emplace_back(backId);
    indices.emplace_back(leftId);

    indices.emplace_back(topId);
    indices.emplace_back(leftId);
    indices.emplace_back(frontId);

    indices.emplace_back(topId);
    indices.emplace_back(frontId);
    indices.emplace_back(rightId);

    indices.emplace_back(btmId);
    indices.emplace_back(backId);
    indices.emplace_back(rightId);

    indices.emplace_back(btmId);
    indices.emplace_back(leftId);
    indices.emplace_back(backId);

    indices.emplace_back(btmId);
    indices.emplace_back(frontId);
    indices.emplace_back(leftId);

    indices.emplace_back(btmId);
    indices.emplace_back(rightId);
    indices.emplace_back(frontId);
    return indices;
}

void TRSMeshBuilder::recurseTriangulate(int level, const TriIndex& tri)
{
    // recurse exit condition
    if (level == m_divideLevel)
    {
        std::vector<unsigned int>& indices = m_mesh->getIndices();
        indices.emplace_back(tri.idxa);
        indices.emplace_back(tri.idxb);
        indices.emplace_back(tri.idxc);
        return;
    }

    int next_level = level + 1;
    unsigned int idxa = tri.idxa;
    unsigned int idxb = tri.idxb;
    unsigned int idxc = tri.idxc;
    unsigned int idxC = getOrCreateMiddle(idxa, idxb);
    unsigned int idxB = getOrCreateMiddle(idxa, idxc);
    unsigned int idxA = getOrCreateMiddle(idxb, idxc);
    TriIndex top{idxa, idxC, idxB};
    TriIndex left{idxC, idxb, idxA};
    TriIndex right{idxB, idxA, idxc};
    TriIndex center{idxA, idxB, idxC};
    recurseTriangulate(next_level, top);
    recurseTriangulate(next_level, left);
    recurseTriangulate(next_level, right);
    recurseTriangulate(next_level, center);
}

IndexPair TRSMeshBuilder::makePair(unsigned int idx1, unsigned int idx2)
{
    if (idx1 < idx2)
    {
        return std::make_pair(idx1, idx2);
    }
    else
    {
        return std::make_pair(idx2, idx1);
    }
}

TRSVec3 TRSMeshBuilder::interpolateNormal(const TRSVec3& norm1, const TRSVec3& norm2)
{
    TRSVec3 norm = (norm1 + norm2);
    norm.normalize();
    return norm;
}

unsigned int TRSMeshBuilder::getOrCreateMiddle(unsigned int idx1, unsigned int idx2)
{
    std::vector<TRSVec3>& normals = m_mesh->getNormals();
    std::vector<TRSVec3>& points = m_mesh->getVertices();

    IndexPair pair = makePair(idx1, idx2);
    unsigned int idxMiddle = -1;
    if (m_centerHash.contains(pair))
    {
        idxMiddle = m_centerHash.at(pair);
    }
    else
    {
        const TRSVec3& norm1 = normals.at(idx1);
        const TRSVec3& norm2 = normals.at(idx2);
        TRSVec3 normMiddle = interpolateNormal(norm1, norm2);
        idxMiddle = static_cast<unsigned int>(normals.size());
        normals.emplace_back(normMiddle);
        points.emplace_back(m_center + normMiddle * m_radius);
        m_centerHash.insert(std::make_pair(pair, idxMiddle));
    }
    return idxMiddle;
}

std::size_t IndexPairHasher::operator()(const IndexPair& pair) const
{
    return static_cast<std::size_t>(pair.first + pair.second);
}
