#ifndef COMMON_H
#define COMMON_H

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <glut.h>
#endif

#define NUM_FIGURE 7

#define BLOCK_SIZE 19

#define BLOCK_SPACING 1

#define WINDOW_WIDTH 350
#define WINDOW_HEIGHT 500

#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4

#define EMPTY 0

#define TRUE 1
#define FALSE 0


const int image[7][4][2]={{{0,0},{0,1},{1,1},{1,0}},{{0,-1},{0,0},{0,1},{0,2}},
			  {{0,1},{-1,1},{-1,0},{-1,-1}},{{0,1},{1,1},{1,0},{1,-1}},
			  {{-1,1}, {-1,0}, {0,0}, {0,-1}},{{0,1},{0,0},{-1,0},{-1,-1}},
			  {{-1,0},{0,0},{1,0},{0,1}}};

struct Rectangle
{
  int left, right, top, bottom;
};

#endif
