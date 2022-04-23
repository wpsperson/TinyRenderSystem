#include "Tetris.h"
#include "Common.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

extern Game tetrisGame;
extern int loseFlag;


Figure::Figure(){
  int numFigure=rand()%NUM_FIGURE;
  if(numFigure>=0 && numFigure<NUM_FIGURE){
    canRotate= (numFigure!=0) ? TRUE: FALSE;
    data=new Block[4];
    int num_blocks_x= WINDOW_WIDTH/ (BLOCK_SIZE+BLOCK_SPACING);
    int num_blocks_y= WINDOW_HEIGHT/ (BLOCK_SIZE+BLOCK_SPACING);
    // Center top
    posX=num_blocks_x/2;
    posY=num_blocks_y-2;
    
    int i;
    for(i=0; i<4; ++i){
      data[i].SetPosX(posX+image[numFigure][i][0]);
      data[i].SetPosY(posY+image[numFigure][i][1]);
    }
  }
}


Figure::Figure(int numFigure){
  if(numFigure>=0 && numFigure<NUM_FIGURE){
    canRotate= (numFigure!=0) ? TRUE: FALSE;
    data=new Block[4];
    int num_blocks_x= WINDOW_WIDTH/ (BLOCK_SIZE+BLOCK_SPACING);
    int num_blocks_y= WINDOW_HEIGHT/ (BLOCK_SIZE+BLOCK_SPACING);
    // Center top
    posX=num_blocks_x/2;
    posY=num_blocks_y-2;
    
    int i;
    for(i=0; i<4; ++i){
      data[i].SetPosX(posX+image[numFigure][i][0]);
      data[i].SetPosY(posY+image[numFigure][i][1]);
    }
  }
}

Figure::~Figure(){
}

void Figure::Create(int numFigure){
  
}
  
void Figure::Draw(){
  int i;
  for(i=0; i<4; ++i){
    data[i].Draw();
  }
}

Block* Figure::GetFigure(){
  return data;
}

void Figure:: New(int numFigure){
  if(numFigure>=0 && numFigure<NUM_FIGURE){
    canRotate= (numFigure!=0) ? TRUE: FALSE;
    int num_blocks_x= WINDOW_WIDTH/ (BLOCK_SIZE+BLOCK_SPACING);
    int num_blocks_y= WINDOW_HEIGHT/ (BLOCK_SIZE+BLOCK_SPACING);
    // Center top
    posX=num_blocks_x/2;
    posY=num_blocks_y-2;
    if(tetrisGame.GetMap(posX, posY+1)==1 && loseFlag==FALSE){
      loseFlag=TRUE;
      return;
    }
    int i;
    for(i=0; i<4; ++i){
      data[i].SetPosX(posX+image[numFigure][i][0]);
      data[i].SetPosY(posY+image[numFigure][i][1]);
    }
  }
}

int Figure::GetPosX(){
  return posX;
}

int Figure:: GetPosY(){
  return posY;
}

void Figure ::SetPosX(int x){
  posX=x;
}

void Figure::SetPosY(int y){
  posY=y;
}

int Figure:: Rotate(int direction){
  int i;
  if(!canRotate)
    return FALSE;
  
  for(i=0; i<4; ++i){
    if(!data[i].CanRotate(LEFT, posX, posY)){
      return FALSE;
    }
  }
  
  for(i=0; i<4; ++i){
    data[i].Rotate(LEFT, posX, posY);
  }
  return TRUE;
}

int Figure:: Translate(int offsetX, int offsetY){
  int i;
  for(i=0; i<4; ++i){
    if(!data[i].CanTranslate(offsetX, offsetY))
      return FALSE;
  }
  
  for(i=0; i<4; ++i){
    data[i].Translate(offsetX, offsetY);
  }
  SetPosX(posX+offsetX);
  SetPosY(posY+offsetY);
  return TRUE;
}
