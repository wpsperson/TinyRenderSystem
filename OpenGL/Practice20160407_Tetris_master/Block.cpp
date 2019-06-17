#include "Common.h"
#include "Tetris.h"
#include <cmath>

extern Game tetrisGame;
Block::Block(){
  int num_blocks_x= WINDOW_WIDTH/ (BLOCK_SIZE+BLOCK_SPACING);
  int num_blocks_y= WINDOW_HEIGHT/ (BLOCK_SIZE+BLOCK_SPACING);
  
  // Center top
  posX=num_blocks_x/2;
  posY=num_blocks_y-1;
}

Block::~Block(){
  Destroy();
}

void Block::Create(){
}
  
void Block::Draw(){
  struct Rectangle rect;
  rect.left=posX*(BLOCK_SIZE+BLOCK_SPACING)-BLOCK_SPACING;
  rect.right=rect.left+BLOCK_SIZE;
  rect.bottom=posY*(BLOCK_SIZE+BLOCK_SPACING)-BLOCK_SPACING;
  rect.top=rect.bottom+BLOCK_SIZE;

  glBegin(GL_QUADS);
  glColor3d(1,1,1);
  glVertex3f(rect.left, rect.top, 0);
  glVertex3f(rect.right, rect.top,0);
  glVertex3f(rect.right, rect.bottom, 0);
  glVertex3f(rect.left, rect.bottom, 0);
  glEnd();
}

void Block::Destroy(){
}
  
void Block::SetPosX(int x){
  posX=x;
}
  
void Block::SetPosY(int y){
  posY=y;
}

int Block::GetPosX(){
  return posX;
}
  
int Block:: GetPosY(){
  return posY;
}
  
int Block:: CanRotate(int direction, int originX, int originY){
  int offsetX=posX-originX;
  int offsetY=posY-originY;
  if(!offsetX && !offsetY)
    return TRUE;
  int tmpX[3], tmpY[3], nextPosX, nextPosY;
  // Obstacle
  nextPosX=originX-offsetY;
  nextPosY=originY+offsetX;
  if(tetrisGame.GetMap(nextPosX,nextPosY)!=EMPTY)
    return FALSE;
  tmpX[0]=originX+offsetX-offsetY;
  tmpY[0]=originY+offsetX+offsetY;
  if(abs(offsetX)>1 || abs(offsetY)>1){
    tmpX[1]=(tmpX[0]+posX)/2;
    tmpX[2]=(tmpX[0]+originX-offsetY)/2;
    tmpY[1]=(tmpY[0]+posY)/2;
    tmpY[2]=(tmpY[0]+originY+offsetX)/2;
    if(tetrisGame.GetMap(tmpX[1],tmpY[1])!=EMPTY || tetrisGame.GetMap(tmpX[2], tmpY[2])!=EMPTY)
      return FALSE;
  }
  else if(abs(offsetX)==1 && abs(offsetY)==1){
    tmpX[1]=(tmpX[0]+posX)/2;
    tmpY[1]=(tmpY[0]+posY)/2;
    if(tetrisGame.GetMap(tmpX[1],tmpY[1])!=EMPTY)
      return FALSE;
  }
  else if(tetrisGame.GetMap(tmpX[0], tmpY[0])!=EMPTY)
    return FALSE;
  return TRUE;
}
  
int Block:: Rotate(int direction, int originX, int originY){
  int offsetX=posX-originX;
  int offsetY=posY-originY;
    
  if(!CanRotate(direction, originX, originY))
    return FALSE;
  int nextPosX, nextPosY;
  // Rotate
  nextPosX=originX-offsetY;
  nextPosY=originY+offsetX;
  SetPosX(nextPosX);
  SetPosY(nextPosY);
  return TRUE;
}
  
int Block::Translate(int offsetX, int offsetY){
  int nextPosX=posX+offsetX, nextPosY=posY+offsetY;
  // Obstacle 
  if(tetrisGame.GetMap(nextPosX, nextPosY)!=EMPTY)
    return FALSE;
  SetPosX(nextPosX);
  SetPosY(nextPosY);
  return TRUE;
}
  
int Block::CanTranslate(int offsetX, int offsetY){
  int nextPosX=posX+offsetX, nextPosY=posY+offsetY;
  // Obstacle 
  if(tetrisGame.GetMap(nextPosX, nextPosY)!=EMPTY)
    return FALSE;
  return TRUE;
}
