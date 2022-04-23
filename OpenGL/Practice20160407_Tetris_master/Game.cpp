#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include "Common.h"
#include "Tetris.h"

extern Figure tetrisFigure;
extern int loseFlag;

// constructor 
Game::Game(){
  // Number of rows and columns
  num_col=(WINDOW_WIDTH)/(BLOCK_SIZE+BLOCK_SPACING);
  num_row=(WINDOW_HEIGHT)/(BLOCK_SIZE+BLOCK_SPACING);
}

// destructor
Game::~Game(){
}

// create new game
void Game::New(){
  int i,j;
  for(i=0; i<num_row; ++i){
    for(j=0; j<num_col; ++j){
      map[i][j]=0;
    }
  }
  tetrisFigure.New(rand()%NUM_FIGURE);
}

// draw game
void Game::Draw(){
  int i,j;
  // Draw Boder
  glBegin(GL_LINE_LOOP);
  glColor3f(0.5,0.2,0.1);
  glVertex3f(0,0,0);
  glVertex3f(WINDOW_WIDTH-BLOCK_SIZE/2,0,0);
  glVertex3f(WINDOW_WIDTH-BLOCK_SIZE/2,WINDOW_HEIGHT,0);
  glVertex3f(0,WINDOW_HEIGHT,0);
  glEnd();

  // draw game
  if(loseFlag==FALSE){
    struct Rectangle rect;
    for(i=0; i<num_row; ++i){
      for(j=0; j<num_col; ++j){
	if(map[i][j]){
	  rect.left=j*(BLOCK_SIZE+BLOCK_SPACING)-BLOCK_SPACING;
	  rect.right=rect.left+BLOCK_SIZE;
	  rect.bottom=i*(BLOCK_SIZE+BLOCK_SPACING)-BLOCK_SPACING;
	  rect.top=rect.bottom+BLOCK_SIZE;       
	  glBegin(GL_QUADS);
	  glColor3d(1,1,1);
	  glVertex3f(rect.left, rect.top, 0);
	  glVertex3f(rect.right, rect.top,0);
	  glVertex3f(rect.right, rect.bottom, 0);
	  glVertex3f(rect.left, rect.bottom, 0);
	  glEnd();
	}
      }
    }
  }
  else{
    char string[]="GAME OVER";
    int len;
    glRasterPos2f(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
    len = (int) strlen(string);
    for (i = 0; i < len; i++) {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    }
  }
}

// update game's state
void Game::Update(Figure &figure){
  Block *tmp;
  int i,num;
  
  // update new figure
  tmp=figure.data;
  for(i=0; i<4; ++i){
    map[tmp[i].GetPosY()][tmp[i].GetPosX()]=1;
  }
  
  // update full line
  num=figure.GetPosY();
  for(i=num+2; i>=num-2; --i){
    if(RowIsFull(i)){
      ClearRow(i);
    }
  }
}

// clear row
void Game::ClearRow(int row){
  int i,j;
  if(row<0 || row>=num_row)
    return;
  for(i=row; i<num_row-2; ++i)
    for(j=0; j<num_col; ++j)
      map[i][j]=map[i+1][j];
  for(j=0; j<num_col; ++j)
    map[num_row-1][j]=0;
}

// check if row is full
int Game::RowIsFull(int row){
  int i;
  if(row<0 || row>=num_row)
    return FALSE;
  for(i=0; i<num_col; ++i)
    if(map[row][i]==EMPTY)
      return FALSE;
  return TRUE;
}

// print the game as a matrix
void Game::Print(){
  int i,j;
  printf("%d %d\n", num_row, num_col);
  for(i=num_row-1; i>=0; --i){
    for(j=0; j<num_col; ++j){
      printf("%2d", map[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

// get grids
int Game::GetMap(int x, int y){
  if((x<0) || (y<0) || (x>= num_col) 
     || (y>=num_row))
    return -1;
  else
    return map[y][x];
}
