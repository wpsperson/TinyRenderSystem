#pragma once

#include "TRS/TRSExport.h"
#include "TRS/TRSGroup.h"
#include "TRS/TRSVector.h"


class TRSGeode;
class TRSDynamicText;
class TRSMesh;
class TRS_EXPORT TRSAxis : public TRSGroup
{
public:
    TRSAxis();

    ~TRSAxis();

    void initialize(TRSViewer* viewer) override;

    TRSBox boundingBox() const override;

    void setSizeInfo(float cylen, float cyRadius, float conelen, float coneRadius);

private:
    void buildCylinderMesh(const TRSVec3 &dirx, const TRSVec3& diry, TRSMesh *mesh);

    void buildConeMesh(const TRSVec3& dirx, const TRSVec3& diry, TRSMesh* mesh);

private:
    float m_cylen = 1.0f;
    float m_cyRadius = 0.05f;
    float m_conelen = 0.3f;
    float m_coneRadius = 0.125f;
    TRSGeode* m_xCylinder = nullptr;
    TRSGeode* m_yCylinder = nullptr;
    TRSGeode* m_zCylinder = nullptr;
    TRSGeode* m_xCone = nullptr;
    TRSGeode* m_yCone = nullptr;
    TRSGeode* m_zCone = nullptr;
    TRSDynamicText* m_xlabel = nullptr;
    TRSDynamicText* m_ylabel = nullptr;
    TRSDynamicText* m_zlabel = nullptr;
};

