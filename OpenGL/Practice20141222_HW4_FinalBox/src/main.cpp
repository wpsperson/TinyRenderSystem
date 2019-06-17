/*
*Function:   ʵ�����������������浥ɫ���䣬�������й��ա��������ת������������
*Author:    ����˧ ZY14DF118
*Data:     2014��ʥ����ǰҹ��
*
*/


#include <stdio.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "glaux.h"

void init(void);
void CALLBACK reshape(GLsizei w,GLsizei h);
void drawBox(void);
void CALLBACK OnLButtonDown(AUX_EVENTREC *);
void CALLBACK OnRightButtonDown(AUX_EVENTREC *);
void CALLBACK OnLButtonMove(AUX_EVENTREC *);
void CALLBACK OnRightButtonMove(AUX_EVENTREC *);
void CALLBACK display(void);


GLint mx,my; //position of mouse
GLfloat x_angle=0.0f, y_angle=0.0f; //angle of eye
GLfloat dist=10.0f; //distance from the eye


double VertexArray[8][3] = { {0.0, 0.0, 0.0},  {1.0, 0.0, 0.0},  {1.0, 1.0, 0.0},  {0.0, 1.0, 0.0},  
{0.0, 0.0, 1.0},  {1.0, 0.0, 1.0},  {1.0, 1.0, 1.0},  {0.0, 1.0, 1.0} };
int   tetra[6][4] = { {0,3,2,1}, {0,1,5,4}, {1,2,6,5}, {2,3,7,6}, {3,0,4,7}, {4,5,6,7} };
// 0����ͼ��1����A��  2 ����B�� 3��ɫ����A�� 4��ɫ����B��  5wpsͷ��


GLuint myTexture, ChristmasTexture;

GLfloat lit_position[] = {1.5f, -0.5f, 0.5f, 0.0f};//ʹ�õ�ǡ���� 1,2�����������
GLfloat mat_yellow[] = {1.0f, 1.0f, 0.0f, 1.0f};
GLfloat mat_cyan[] = {0.0f, 1.0f, 1.0f, 1.0f};




void init(void)
{
	AUX_RGBImageRec *MyPicture, *ChristmasPicture;
	MyPicture = auxDIBImageLoad("wps.bmp");
	glGenTextures(1, &myTexture);
	glBindTexture(GL_TEXTURE_2D, myTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, MyPicture->sizeX, MyPicture->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, MyPicture->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	free(MyPicture->data);
	free(MyPicture);

	ChristmasPicture = auxDIBImageLoad("Christmas.bmp");
	glGenTextures(1, &ChristmasTexture);
	glBindTexture(GL_TEXTURE_2D, ChristmasTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, ChristmasPicture->sizeX, ChristmasPicture->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, ChristmasPicture->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	free(ChristmasPicture->data);
	free(ChristmasPicture);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

}

void CALLBACK reshape(GLsizei w,GLsizei h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //��Ӧ��λ��I

	if(h < 1) h=1;
	gluPerspective(30.0, 1.0*w/h, 0.1, 20.0); //��Ӧ�任��T0

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); //��Ӧ��λ��I
}

void CALLBACK OnLButtonDown(AUX_EVENTREC *event)
{
	mx=event->data[AUX_MOUSEX];
	my=event->data[AUX_MOUSEY];
}

void CALLBACK OnRightButtonDown(AUX_EVENTREC *event)
{
	mx=event->data[AUX_MOUSEX];
	my=event->data[AUX_MOUSEY];
}

void CALLBACK OnLButtonMove(AUX_EVENTREC *event)
{
	GLint dx,dy; //offset of mouse;

	dx = event->data[AUX_MOUSEX]-mx;
	dy = event->data[AUX_MOUSEY]-my;
	mx=event->data[AUX_MOUSEX];
	my=event->data[AUX_MOUSEY];

	y_angle += dx*0.1f;
	x_angle += dy*0.1f;

	display();
}

void CALLBACK OnRightButtonMove(AUX_EVENTREC *event)
{

	GLint dx,dy; //offset of mouse;

	dx = event->data[AUX_MOUSEX]-mx;
	dy = event->data[AUX_MOUSEY]-my;
	mx=event->data[AUX_MOUSEX];
	my=event->data[AUX_MOUSEY];
	dist += (dx+dy)*0.01f;

	display();
}

void CALLBACK display(void)
{
	glClearColor(1.0f,1.0f,1.0f,0.0f);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
		glTranslated(0.0, 0.0, -dist); //��Ӧ�任��T1
		glRotatef(x_angle, 1.0f, 0.0f, 0.0f); //��Ӧ�任��T2
		glRotatef(y_angle, 0.0f, 1.0f, 0.0f); //��Ӧ�任��T3

		glDisable(GL_LIGHTING);
		glLightfv(GL_LIGHT0, GL_POSITION, lit_position);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		//glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_cyan); //��������ȫ�ֲ��ʵ���������ɫ��

		//glShadeModel(GL_SMOOTH);
		//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		//glEnable(GL_COLOR_MATERIAL);

		//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		//glEnable (GL_LIGHTING);
		//glEnable (GL_LIGHT0);
	
		drawBox();			

	glPopMatrix();
	glFlush();
	auxSwapBuffers();
}

void drawBox(void)
{
	glDisable(GL_TEXTURE_1D);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, myTexture);	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3dv(VertexArray[tetra[0][0]]);
		glTexCoord2f(0.0f, 1.0f); glVertex3dv(VertexArray[tetra[0][1]]);
		glTexCoord2f(1.0f, 1.0f); glVertex3dv(VertexArray[tetra[0][2]]);
		glTexCoord2f(1.0f, 0.0f); glVertex3dv(VertexArray[tetra[0][3]]);
	glEnd();
	glDisable(GL_TEXTURE_2D);





	glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_yellow);
	glMaterialfv(GL_BACK, GL_DIFFUSE, mat_cyan);
    glBegin(GL_QUADS);	
	glNormal3d(-1.0, -1.0, -1.0);   glVertex3dv(VertexArray[tetra[1][0]]);
	glNormal3d(1.0, -1.0, -1.0);   glVertex3dv(VertexArray[tetra[1][1]]);
	glNormal3d(1.0, -1.0, 1.0);   glVertex3dv(VertexArray[tetra[1][2]]);
	glNormal3d(-1.0, -1.0, 1.0);   glVertex3dv(VertexArray[tetra[1][3]]);
	glEnd();
	glDisable(GL_LIGHTING);



	//�����档

	glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_cyan);
	glMaterialfv(GL_BACK, GL_DIFFUSE, mat_yellow);
	glBegin(GL_QUADS);	
	glNormal3d(1.0, -1.0, -1.0); 	glVertex3dv(VertexArray[tetra[2][0]]);
	glNormal3d(1.0, 1.0, -1.0);  glVertex3dv(VertexArray[tetra[2][1]]);
	glNormal3d(1.0, 1.0, 1.0);  glVertex3dv(VertexArray[tetra[2][2]]);
	glNormal3d(1.0, -1.0, 1.0);  glVertex3dv(VertexArray[tetra[2][3]]);	
	glEnd();	
	glDisable(GL_LIGHTING);



	
	glBegin(GL_QUADS);		
	glColor3f(0.0f,1.0f,1.0f);	glVertex3dv(VertexArray[tetra[3][0]]);
	glColor3f(1.0f,0.0f,1.0f);	glVertex3dv(VertexArray[tetra[3][1]]);
	glColor3f(1.0f,1.0f,0.0f);	glVertex3dv(VertexArray[tetra[3][2]]);
	glColor3f(0.0f,0.0f,0.0f);	glVertex3dv(VertexArray[tetra[3][3]]);
	glEnd();


	glBegin(GL_QUADS);
	glColor3f(1.0f,0.0f,0.0f);	glVertex3dv(VertexArray[tetra[4][0]]);
	glColor3f(0.0f,1.0f,0.0f);	glVertex3dv(VertexArray[tetra[4][1]]);
	glColor3f(0.0f,0.0f,1.0f);	glVertex3dv(VertexArray[tetra[4][2]]);
	glColor3f(1.0f,1.0f,1.0f);	glVertex3dv(VertexArray[tetra[4][3]]);
	glEnd();


	glDisable(GL_TEXTURE_1D);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ChristmasTexture);	
	glBegin(GL_QUADS);	
	glTexCoord2f(0.0f, 0.0f); glVertex3dv(VertexArray[tetra[5][0]]);
	glTexCoord2f(1.0f, 0.0f); glVertex3dv(VertexArray[tetra[5][1]]);
	glTexCoord2f(1.0f, 1.0f); glVertex3dv(VertexArray[tetra[5][2]]);
	glTexCoord2f(0.0f, 1.0f); glVertex3dv(VertexArray[tetra[5][3]]);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void main(void)
{
	auxInitDisplayMode(AUX_DOUBLE|AUX_RGBA|AUX_DEPTH24);
	auxInitPosition(0,0,1500,1000);
	auxInitWindow("����˧ ZY14DF118 �����ͼ��ѧ���Ĵ���ҵ");
	
	init();

	auxReshapeFunc(reshape);
	auxMouseFunc(AUX_LEFTBUTTON, AUX_MOUSEDOWN, OnLButtonDown);
	auxMouseFunc(AUX_RIGHTBUTTON, AUX_MOUSEDOWN, OnRightButtonDown);
	auxMouseFunc(AUX_LEFTBUTTON, AUX_MOUSELOC, OnLButtonMove);
	auxMouseFunc(AUX_RIGHTBUTTON, AUX_MOUSELOC, OnRightButtonMove);
	auxMainLoop(display);

	glDeleteTextures(1, &myTexture);
	glDeleteTextures(1, &ChristmasTexture);
}






