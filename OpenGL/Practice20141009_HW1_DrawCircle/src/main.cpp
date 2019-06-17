#include <stdio.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glaux.h>

void init(void);
void CALLBACK reshape(GLsizei w,GLsizei h);
void CALLBACK display(void);
void MidCircle(int x0, int y0, int r);

void init(void)
{
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
void CALLBACK reshape(GLsizei w,GLsizei h)
{
	glViewport(0,0,w,h);
}

//绘制一个整圆
void MidCircle(int x0, int y0, int r)
{
	int x,y;
	float func;
	x = 0;
	y = r;
	func = 1.25 - r;

	glColor3f(0.0f,1.0f,0.0f); //绿色
	glPointSize(1.0f); //点大小
	glBegin(GL_POINTS);
	while (x<=y)
	{
		glVertex2d(x0+x,y0+y);
		glVertex2d(x0+y,y0+x);
		glVertex2d(x0-x,y0+y);
		glVertex2d(x0-y,y0+x);
		glVertex2d(x0+x,y0-y);
		glVertex2d(x0+y,y0-x);
		glVertex2d(x0-x,y0-y);
		glVertex2d(x0-y,y0-x);

		if (func<=0)
		{
			func += 2*x + 3;
		}
		else
		{
			func += 2*(x-y) + 5;
			y--;
		}
		x++;
	}	
	glEnd();
}

void CALLBACK display(void)
{


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-200.0, 200.0, -200.0, 200.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	MidCircle(-30,-70,100);
	glFlush();
}

void main(void)
{
	auxInitDisplayMode(AUX_SINGLE|AUX_RGBA);
	auxInitPosition(0,0,500,500);
	auxInitWindow("wps");
	init();

	auxReshapeFunc(reshape);
	auxMainLoop(display);
}