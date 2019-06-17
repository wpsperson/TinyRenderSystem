#include<windows.h>
#include<gl/Gl.h>
#include<gl/glu.h>
#include"glut.h"

void myInit(void)
{
	glClearColor(1.0,1.0,1.0,0.0);
	glColor3f(0.0f,0.0f,0.0f);
	glPointSize(5.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,640.0,0.0,480.0);
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	glVertex2i(289,190);
	glVertex2i(320,128);
	glVertex2i(239,67);
	glVertex2i(194,101);
	glVertex2i(129,83);
	glVertex2i(75,73);
	glVertex2i(74,74);
	glVertex2i(20,10);
	glEnd();
	glFlush();
}

int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(100,150);
	glutCreateWindow("My first attempt by LHG");
	glutDisplayFunc(myDisplay);
	myInit();
	glutMainLoop();
	return 0;
}

