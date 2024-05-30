/*!
 *@brief    some basic geometry algorithm
 *@date     2021-5-29
 */
#pragma once
#include <iostream>

float distance(float x1, float y1, float z1, float x2, float y2, float z2);

float distance(float* p1, float*p2);

void divTwoPt(float* p1, float*p2, float prop, float* out);

void normlize(float* v);

void add(float* p1, float* p2, float* out);

void cross(float* p1, float*p2, float* out);

unsigned int* genWireFrameElementsArray(int n, int m);

unsigned int* genBS2BezierElementArray(int ptNum);

unsigned int* genBS2BezierPatchEleArr(int ptNumU, int ptNumV);

