/*!
 *@brief    some basic geometry algorithm
 *@date     2021-5-29
 */
#pragma once
#include <iostream>
#include "TRS/TRSExport.h"

TRS_EXPORT float distance(float x1, float y1, float z1, float x2, float y2, float z2);

TRS_EXPORT float distance(float* p1, float*p2);

TRS_EXPORT void divTwoPt(float* p1, float*p2, float prop, float* out);

TRS_EXPORT void normlize(float* v);

TRS_EXPORT void add(float* p1, float* p2, float* out);

TRS_EXPORT void cross(float* p1, float*p2, float* out);

TRS_EXPORT unsigned int* genWireFrameElementsArray(int n, int m);

TRS_EXPORT unsigned int* genBS2BezierElementArray(int ptNum);

TRS_EXPORT unsigned int* genBS2BezierPatchEleArr(int ptNumU, int ptNumV);

