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

	//��10*10����
	glColor3f(0.0f,1.0f,0.0f); //��ɫ
	for(i=0; i<=10; i++) //10��ˮƽ��
	{
		glBegin(GL_LINES);
			glVertex2d(  0.0,i*10.0);
			glVertex2d(100.0, i*10.0);
		glEnd();
	}
	glBegin(GL_LINES); //10������
		for(i=0; i<=10; i++)
		{
			glVertex2d(i*10.0,  0.0);
			glVertex2d(i*10.0, 100.0);
		}
	glEnd();

	//�ڶԽ��߻���
	glColor3f(1.0f,1.0f,1.0f); //��ɫ
	glPointSize(10.0f); //���С
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
	//��Ϊ����һ����Ҫ��̭�Ŀ�glaux�������������Ͳ��������ĵġ�����
	auxInitWindow(TEXT("Draw the color sphere"));
	init();

	auxReshapeFunc(reshape);
	auxMainLoop(display);
}