/***********************************
Project: Tetris
Language: C++
API: Opengl
Date: April 12, 2012
***********************************/

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#include "Common.h"
#include "Tetris.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <glut.h>
#endif

using namespace std;

// variables' declaration and initialization
int newFigureFlag=TRUE;  
int loseFlag=FALSE;
Figure tetrisFigure(rand()%NUM_FIGURE); 
Game tetrisGame;

// function prototypes
void Display ( void );
void MyInit ( );
void Update(int value);
void HandleKeyPress(unsigned char key, int x, int y);

// handle keyboard
void HandleKeyPress(unsigned char key, int x, int y){
  switch(key){
  case 27:
    exit(1);
  }
  // Control keys
  if(loseFlag==FALSE){
    switch(key){
    case 's':
      tetrisFigure.Translate(0,-1);
      glutPostRedisplay();
      break;
    case 'a':
      tetrisFigure.Translate(-1,0);
      glutPostRedisplay();
      break;
    case 'd':
      tetrisFigure.Translate(1,0);
      glutPostRedisplay();
      break;
    case 'w':
      tetrisFigure.Rotate(LEFT);
      glutPostRedisplay();
      break;
    case 'e':
      tetrisFigure.Translate(0,1);
      glutPostRedisplay();
      break;
    default:
      break;
    }
  }
  else{
    switch(key){
    case 'n':
      tetrisGame.New();
      loseFlag=FALSE;
      glutPostRedisplay();
      break;
    default:
      break;
    }
  }
}

// display's function
void Display ( void ) {
  glClear ( GL_COLOR_BUFFER_BIT );
  glClear (GL_DEPTH_BUFFER_BIT);
  if(loseFlag==FALSE)
    tetrisFigure.Draw(); // only draw if loseFlag not TRUE
  tetrisGame.Draw();  // Draw the game
  glutSwapBuffers();
}

// Initialization
void MyInit ( void ) {;
  glClearColor   ( 0.0, 0.0, 0.0, 0.0 ) ; //black background
  glMatrixMode   ( GL_MODELVIEW ) ;  
  glLoadIdentity ( ) ; 
  gluOrtho2D     ( -50, (GLdouble)WINDOW_WIDTH+50, -50, (GLdouble)WINDOW_HEIGHT+50 ) ;
  srand(time(NULL));
} 

// update function
void Update(int value){
  if(loseFlag==FALSE){
    newFigureFlag=tetrisFigure.Translate(0, -1);
    if(!newFigureFlag){
      tetrisGame.Update(tetrisFigure);
      tetrisFigure.New(rand()%NUM_FIGURE);
    }
  }
  glutPostRedisplay(); // Refresh Display
  glutTimerFunc(100, Update, 0);
}

int main(int argc, char **argv) {
  glutInit               ( &argc, argv );
  glutInitDisplayMode    ( GLUT_DOUBLE | GLUT_RGB );
  glutInitWindowSize     ( WINDOW_WIDTH+100, WINDOW_HEIGHT+100 );      // window size
  glutInitWindowPosition ( 50, 50 );      // window position on screen
  glutCreateWindow       ( "Tetris's Game" );

  MyInit ( );  // initialization

  glutDisplayFunc ( Display );  // display
  glutKeyboardFunc(HandleKeyPress);  // keyboard 

  glutTimerFunc(100,Update,0);  // timer's function
  glutMainLoop ( );
}
