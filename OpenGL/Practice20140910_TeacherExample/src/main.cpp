#include <stdio.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "glaux.h"

void init(void);
void CALLBACK reshape(GLsizei w,GLsizei h);
void CALLBACK display(void);

void init(void)
{
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void CALLBACK reshape(GLsizei w,GLsizei h)
{
	glViewport(0,0,w,h);
}

void CALLBACK display(void)
{
	int i;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-10.0, 110.0, -10.0, 110.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//画10*10网格
	glColor3f(0.0f,1.0f,0.0f); //绿色
	for(i=0; i<=10; i++) //10条水平线
	{
		glBegin(GL_LINES);
			glVertex2d(  0.0,i*10.0);
			glVertex2d(100.0, i*10.0);
		glEnd();
	}
	glBegin(GL_LINES); //10条竖线
		for(i=0; i<=10; i++)
		{
			glVertex2d(i*10.0,  0.0);
			glVertex2d(i*10.0, 100.0);
		}
	glEnd();

	//在对角线画点
	glColor3f(1.0f,1.0f,1.0f); //白色
	glPointSize(10.0f); //点大小
	glBegin(GL_POINTS);
		for(i=0; i<=10; i++) glVertex2d(i*10.0,i*10.0);
	glEnd();
	for(i=0; i<=10; i++)
	{
		glBegin(GL_POINTS);
			glVertex2d(i*10.0,  100.0-i*10.0);
		glEnd();
	}

	glFlush();
}

void main(void)
{
	auxInitDisplayMode(AUX_SINGLE|AUX_RGBA);
	auxInitPosition(0,0,500,500);
	//因为这是一个快要淘汰的库glaux，所以数据类型不是主流的的。。。
	auxInitWindow(TEXT("Draw the color sphere"));
	init();

	auxReshapeFunc(reshape);
	auxMainLoop(display);
}