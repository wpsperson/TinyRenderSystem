/**
*	实现一个纹理贴图场景;
*	动态运动的茶壶;
*	简单的交互操作;
**/
#include <iostream>
#include <string>
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <GLAUX.H>
#include <glut.h>

//----第一部分：全局变量-----------------------------------------------------------------;

GLfloat dist=1.5f; //distance from the eye
GLfloat x_angle=0.0f, y_angle=0.0f; //angle of eye

GLint mx,my; //position of mouse

GLfloat potx = 0.0f, poty =  0.0f, potz = 0.0f;
GLfloat pot_speed_x = 0.01, pot_speed_y = 0.015, pot_speed_z = 0.017;
GLfloat pot_y_angle = 0.0;
GLfloat pot_y_angle_speed = 2.0f;

GLuint* frontalTextureIndex, *sideTextureIndex, *groundTextureIndex, *ceilTextureIndex;//两种纹理贴图;

//GLdouble VertexArray[8][3] = { {-2.0, -1.0, -2.0},  {2.0, -1.0, -2.0},  {2.0, 1.0, -2.0},  {-2.0, 1.0, -2.0},  
//							{-2.0, -1.0, 2.0},  {2.0, -1.0, 2.0},  {2.0, 1.0, 2.0},  {-2.0, 1.0, 2.0} };
GLdouble VertexArray[8][3] = { {-20.0, -10.0, -20.0},  {20.0, -10.0, -20.0},  {20.0, 10.0, -20.0},  {-20.0, 10.0, -20.0},  
								{-20.0, -10.0, 20.0},  {20.0, -10.0, 20.0},  {20.0, 10.0, 20.0},  {-20.0, 10.0, 20.0} };
int   tetra[6][4] = { {1,2,3,0}, {0,1,5,4}, {1,2,6,5}, {2,3,7,6}, {3,0,4,7}, {4,5,6,7} };

//----第二部分：便利函数-----------------------------------------------------------------;
//根据一个文件名生成纹理;
GLuint* generateTextureFromPicture(const std::string& picture_name)
{
	GLuint* texture_index_ptr = new GLuint;
	AUX_RGBImageRec *MyPicture;
	MyPicture = auxDIBImageLoad(picture_name.c_str() );
	glGenTextures(1, texture_index_ptr);   //（glGenTextures就是用来产生你要操作的纹理对象的索引的，比如你告诉OpenGL，我需要5个纹理对象，它会从没有用到的整数里返回5个给你）;
	glBindTexture(GL_TEXTURE_2D, *texture_index_ptr);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, MyPicture->sizeX, MyPicture->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, MyPicture->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	free(MyPicture->data);
	free(MyPicture);
	return texture_index_ptr;
}

void drawBox(void)
{

	glDisable(GL_TEXTURE_1D);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, *frontalTextureIndex);	
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3dv(VertexArray[tetra[0][0]]);
	glTexCoord2f(0.0f, 1.0f); glVertex3dv(VertexArray[tetra[0][1]]);
	glTexCoord2f(1.0f, 1.0f); glVertex3dv(VertexArray[tetra[0][2]]);
	glTexCoord2f(1.0f, 0.0f); glVertex3dv(VertexArray[tetra[0][3]]);
	glEnd();
	glDisable(GL_TEXTURE_2D);


	glDisable(GL_TEXTURE_1D);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, *groundTextureIndex);	
	glBegin(GL_QUADS);	
	glTexCoord2f(0.0f, 0.0f); glVertex3dv(VertexArray[tetra[1][0]]);
	glTexCoord2f(1.0f, 0.0f); glVertex3dv(VertexArray[tetra[1][1]]);
	glTexCoord2f(1.0f, 1.0f); glVertex3dv(VertexArray[tetra[1][2]]);
	glTexCoord2f(0.0f, 1.0f); glVertex3dv(VertexArray[tetra[1][3]]);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_TEXTURE_1D);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, *sideTextureIndex);	
	glBegin(GL_QUADS);	
	glTexCoord2f(0.0f, 0.0f); glVertex3dv(VertexArray[tetra[2][0]]);
	glTexCoord2f(1.0f, 0.0f); glVertex3dv(VertexArray[tetra[2][1]]);
	glTexCoord2f(1.0f, 1.0f); glVertex3dv(VertexArray[tetra[2][2]]);
	glTexCoord2f(0.0f, 1.0f); glVertex3dv(VertexArray[tetra[2][3]]);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	

	glDisable(GL_TEXTURE_1D);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, *ceilTextureIndex);	
	glBegin(GL_QUADS);	
	glTexCoord2f(0.0f, 0.0f); glVertex3dv(VertexArray[tetra[3][0]]);
	glTexCoord2f(1.0f, 0.0f); glVertex3dv(VertexArray[tetra[3][1]]);
	glTexCoord2f(1.0f, 1.0f); glVertex3dv(VertexArray[tetra[3][2]]);
	glTexCoord2f(0.0f, 1.0f); glVertex3dv(VertexArray[tetra[3][3]]);
	glEnd();
	glDisable(GL_TEXTURE_2D);


	glDisable(GL_TEXTURE_1D);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, *sideTextureIndex);	
	glBegin(GL_QUADS);	
	glTexCoord2f(0.0f, 0.0f); glVertex3dv(VertexArray[tetra[4][0]]);
	glTexCoord2f(1.0f, 0.0f); glVertex3dv(VertexArray[tetra[4][1]]);
	glTexCoord2f(1.0f, 1.0f); glVertex3dv(VertexArray[tetra[4][2]]);
	glTexCoord2f(0.0f, 1.0f); glVertex3dv(VertexArray[tetra[4][3]]);
	glEnd();
	glDisable(GL_TEXTURE_2D);


	glDisable(GL_TEXTURE_1D);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, *sideTextureIndex);	
	glBegin(GL_QUADS);	
	glTexCoord2f(0.0f, 0.0f); glVertex3dv(VertexArray[tetra[5][0]]);
	glTexCoord2f(1.0f, 0.0f); glVertex3dv(VertexArray[tetra[5][1]]);
	glTexCoord2f(1.0f, 1.0f); glVertex3dv(VertexArray[tetra[5][2]]);
	glTexCoord2f(0.0f, 1.0f); glVertex3dv(VertexArray[tetra[5][3]]);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void setRenderState()
{
	// render state
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	// enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// light source attributes
	GLfloat LightAmbient[]= {
		0.4f, 0.4f, 0.4f, 1.0f 
	};
	GLfloat LightDiffuse[]= {
		0.3f, 0.3f, 0.3f, 1.0f 
	};
	GLfloat LightSpecular[]= {
		0.4f, 0.4f, 0.4f, 1.0f 
	};
	GLfloat LightPosition[] = {
		5.0f, 5.0f, 5.0f, 1.0f 
	};

	glLightfv(GL_LIGHT0, GL_AMBIENT , LightAmbient );
	glLightfv(GL_LIGHT0, GL_DIFFUSE , LightDiffuse );
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

	// surface material attributes
	GLfloat material_Ka[]= {
		0.11f, 0.06f, 0.11f, 1.0f 
	};
	GLfloat material_Kd[]= {
		0.43f, 0.47f, 0.54f, 1.0f 
	};
	GLfloat material_Ks[]= {
		0.33f, 0.33f, 0.52f, 1.0f 
	};
	GLfloat material_Ke[]= {
		0.1f , 0.0f , 0.1f , 1.0f 
	};
	GLfloat material_Se= 10;

	glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ka);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_Kd);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_Ks);
	glMaterialfv(GL_FRONT, GL_EMISSION, material_Ke);
	glMaterialf (GL_FRONT, GL_SHININESS, material_Se);
}

void calcPotPosition()
{
	potx += pot_speed_x;
	poty += pot_speed_y;
	potz += pot_speed_z;
	GLfloat edgePos = 1.2f;
	if ( potx > edgePos )	pot_speed_x = -pot_speed_x;	
	if ( potx < -edgePos )  pot_speed_x = -pot_speed_x;	
	if ( poty > edgePos )	pot_speed_y = -pot_speed_y;	
	if ( poty < -edgePos )  pot_speed_y = -pot_speed_y;	
	if ( potz > edgePos )	pot_speed_z = -pot_speed_z;	
	if ( potz < -edgePos )  pot_speed_z = -pot_speed_z;	


	pot_y_angle += pot_y_angle_speed;

}

//----第三部分：初始函数和回调函数-----------------------------------------------------------------;
void myInit()
{
	glClearColor(1.0,1.0,1.0,0.0);
	frontalTextureIndex = generateTextureFromPicture("a.bmp");
	sideTextureIndex = generateTextureFromPicture("b.bmp");
	groundTextureIndex = generateTextureFromPicture("d.bmp");
	ceilTextureIndex = generateTextureFromPicture("ceil.bmp");
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
}

void myDisplay(void)
{
	glClearColor(1.0f,1.0f,1.0f,0.0f);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslated(0.0, 0.0, -dist); //对应变换阵T1
	glRotatef(x_angle, 1.0f, 0.0f, 0.0f); //对应变换阵T2
	glRotatef(y_angle, 0.0f, 1.0f, 0.0f); //对应变换阵T3
		
		glPushMatrix();
		glTranslated(0.0, -3.0, -10.0f ); //调整一下位置;
			glPushMatrix();
			calcPotPosition();
			glTranslated( potx, poty, potz );
			glRotatef(pot_y_angle, 0.0, 1.0, 1.0);
			setRenderState();
			glutSolidTeapot(0.5);
			glPopMatrix();
		glutWireCube(3.0);
		glPopMatrix();

	drawBox();
	glPopMatrix();
	glFlush();	

}

void reshape(int width, int height)
{
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //对应单位阵I

	if(height < 1) height=1;
	gluPerspective(45.0, 1.0*width/height, 0.01, 50.0); //对应变换阵T0

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); //对应单位阵I
	
}

void specialKeyFunction(int key, int x, int y)
{
	if (key == GLUT_KEY_UP )
	{
		std::cout<<"up"<<std::endl;
		dist -= 0.1;
		if ( dist<-10 )
		{
			dist = -10;
		}
		myDisplay();
	}
	if (key == GLUT_KEY_DOWN )
	{
		std::cout<<"down"<<std::endl;
		dist += 0.1;
		if (dist>=10)
		{
			dist = 10;
		}
		myDisplay();
	}

}

void mouseClickFunction(int button, int state, int x, int y)
{
	if ((button ==GLUT_LEFT_BUTTON ) && ( state == GLUT_DOWN ) )
	{
		mx = x;
		my = y;
	}
	myDisplay();
}

void mouseMoveFunction( int x, int y )
{
	y_angle -= ( x - mx )*0.05f;
	x_angle -= ( y - my )* 0.05f;
	mx = x;
	my = y;

	myDisplay();
}

void timer( int value ) {

	// render
	glutPostRedisplay();

	// reset timer
	// 16 ms per frame ( about 60 frames per second )
	glutTimerFunc( 16, timer, 0 );

}

//----第四部分：主函数-----------------------------------------------------------------;
int main(int argc, char** argv )
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(100,150);
	glutCreateWindow("一个OpenGL动态场景展示 ");
	glutDisplayFunc(myDisplay);
	glutReshapeFunc( reshape );
	glutMouseFunc( mouseClickFunction );
	glutMotionFunc( mouseMoveFunction );
	glutSpecialFunc( specialKeyFunction );
	glutTimerFunc( 16, timer, 0 );

	myInit();
	glutMainLoop();


	return 0;
}