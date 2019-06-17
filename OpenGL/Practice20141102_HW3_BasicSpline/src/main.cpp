/*
Function: draw basic spline by a series of control point.
Author:	  wpsperson
Data:	  20141102
Notes:	  All rights reserved
*/

#include<windows.h>
#include<gl/Gl.h>
#include<gl/glu.h>
#include"glut.h"
#include<iostream>
#include<vector>

#define MinPointNum 4					//3��B����������Ҫ4�����Ƶ���ܻ���һ��������
#define AmplificationCoefficient 100	//�����Ŀ��Ƶ��������1����Ҫ�Ŵ������Opengl�л��ơ�
#define NodeStep 0.005					//�ڵ�仯�Ĳ���



struct MyPoint
{
	MyPoint(){}
	MyPoint(double _x, double _y):x(_x),y(_y){}
	double x;
	double y;
};
std::vector<MyPoint> myPointVector;

MyPoint findPointByProportion(const MyPoint &startPoint, const MyPoint &endPoint, double proportion)
{
	double xCoordinate,yCoordinate;
	xCoordinate = startPoint.x*(1-proportion) + endPoint.x*proportion;
	yCoordinate = startPoint.y*(1-proportion) + endPoint.y*proportion;
	return MyPoint(xCoordinate, yCoordinate);
}

//��������Ŀ��Ƶ�������B��������
void drawBasicSpline(const std::vector<MyPoint> &vect)
{
	if (vect.size()< 4)
	{
		std::cout<<"�����Ŀ��Ƶ���Ŀ̫�٣�����ʵ�ֻ��ƣ�"<<std::endl;
	}
	else
	{
		//ÿ��ʹ��4�����Ƶ�����һС��B������
		std::vector<MyPoint>::const_iterator constItrStart = vect.begin();
		std::vector<MyPoint>::const_iterator constItrEnd = vect.begin()+3;
		//
		std::vector<MyPoint>::const_iterator constItrActive;
		while (constItrEnd != vect.end())
		{			
			//����ÿһ�ε��ĸ����Ƶ㣬��t=0 ��t=1���е²����㷨��
			for (double t = 0.0; t <= 1; t += NodeStep)
			{
				constItrActive = constItrStart;
				//��һ�ε�����
				MyPoint temp1_1 = findPointByProportion(*(constItrActive),*(constItrActive+1),(2+t)/3);
				constItrActive ++;
				MyPoint temp1_2 = findPointByProportion(*(constItrActive),*(constItrActive+1),(1+t)/3);
				constItrActive ++;
				MyPoint temp1_3 = findPointByProportion(*(constItrActive),*(constItrActive+1),(t)/3);
				//�ڶ��ε���
				MyPoint temp2_1 = findPointByProportion(temp1_1, temp1_2, (1+t)/2);
				MyPoint temp2_2 = findPointByProportion(temp1_2, temp1_3, t/2);
				//�����ε���
				MyPoint temp3 = findPointByProportion(temp2_1, temp2_2, t);
				glVertex2d(temp3.x, temp3.y);

			}
			
			//����������������Լӣ�ʹ����һ��ѭ���д������ĸ����Ƶ��B������
			constItrStart ++;
			constItrEnd ++;
		}
	}
}

void drawControlPoint(const std::vector<MyPoint> &vect)
{
	std::vector<MyPoint>::const_iterator constItr = vect.begin();
	while (constItr != vect.end())
	{
		glVertex2d((*constItr).x,(*constItr).y);
		constItr ++;
	}
}

void customInputControlPoint()
{
	std::cout<<"������� 1 ���ڵ�ĺ�����"<<std::endl;
	double temp,pointX,PointY;
	int num = 0;
	while (std::cin>>temp)
	{
		num++;//�����ж�����ż��
		if (num%2 == 1)
		{
			pointX = temp;
			std::cout<<"�������"<<(int)((num/2)+1)<<"���ڵ��������"<<std::endl;
		}
		else
		{
			PointY = temp;
			myPointVector.push_back(MyPoint(pointX*AmplificationCoefficient,PointY*AmplificationCoefficient));
			std::cout<<"�ɹ������"<<(int)((num/2))<<"���ڵ� ��"<<pointX<<","<<PointY<<")"<<std::endl;
			std::cout<<"�������"<<(int)((num/2)+1)<<"���ڵ�ĺ�����"<<std::endl;

		}

	}
}

void myInit(void)
{
	glClearColor(1.0,1.0,1.0,0.0);
	//glColor3f(0.0f,0.0f,0.0f);
	//glPointSize(1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,640.0,0.0,480.0);
}

void myDisplay(void)
{
	
	glClear(GL_COLOR_BUFFER_BIT);

	glPointSize(1.0);
	glColor3f(0.0f,0.0f,1.0f);
	glBegin(GL_POINTS);
	drawBasicSpline(myPointVector);
	glEnd();

	glPointSize(5.0);
	glColor3f(1.0f,0.0f,0.0f);
	glBegin(GL_POINTS);	
	drawControlPoint(myPointVector);
	glEnd();

	glFlush();

}

int main(int argc,char **argv)
{
	std::cout<<"��ѡ����Ҫ���еĲ�����"<<std::endl;
	std::cout<<"������\"1\"�������Ľڵ���Ƴ� ����B����"<<std::endl;
	std::cout<<"������\"2\"�������Ľڵ���Ƴ� ׼����B����"<<std::endl;
	std::cout<<"������\"3\"�������Ľڵ���Ƴ� ���B����"<<std::endl;
	std::cout<<"������\"4\"��������ڵ���Ƴ� ����B����"<<std::endl;
	std::cout<<"������\"5\"�˳��ó���"<<std::endl;
	int OptionNum;
	std::cin>>OptionNum;
	switch (OptionNum)
	{
	case 1:
		std::cout<<"�����Ľڵ���ƾ���B����ͼ�����£�"<<std::endl;
		myPointVector.push_back(MyPoint(0,0));
		myPointVector.push_back(MyPoint(0,1*AmplificationCoefficient));
		myPointVector.push_back(MyPoint(1*AmplificationCoefficient,1*AmplificationCoefficient));
		myPointVector.push_back(MyPoint(1*AmplificationCoefficient,0));
		myPointVector.push_back(MyPoint(2*AmplificationCoefficient,0));
		myPointVector.push_back(MyPoint(2*AmplificationCoefficient,1*AmplificationCoefficient));		
		break;
	case 2:
		std::cout<<"�����Ľڵ����׼����B����ͼ�����£�"<<std::endl;
		myPointVector.push_back(MyPoint(0,0));
		myPointVector.push_back(MyPoint(0,0));
		myPointVector.push_back(MyPoint(0,0));
		myPointVector.push_back(MyPoint(0,0));
		myPointVector.push_back(MyPoint(0,1*AmplificationCoefficient));
		myPointVector.push_back(MyPoint(1*AmplificationCoefficient,1*AmplificationCoefficient));
		myPointVector.push_back(MyPoint(1*AmplificationCoefficient,0));
		myPointVector.push_back(MyPoint(2*AmplificationCoefficient,0));
		myPointVector.push_back(MyPoint(2*AmplificationCoefficient,1*AmplificationCoefficient));
		myPointVector.push_back(MyPoint(2*AmplificationCoefficient,1*AmplificationCoefficient));
		myPointVector.push_back(MyPoint(2*AmplificationCoefficient,1*AmplificationCoefficient));
		myPointVector.push_back(MyPoint(2*AmplificationCoefficient,1*AmplificationCoefficient));
		break;

	case 3:
		std::cout<<"�����Ľڵ���ƴ���ǵ�B����ͼ�����£�"<<std::endl;
		myPointVector.push_back(MyPoint(0,0));
		myPointVector.push_back(MyPoint(0,1*AmplificationCoefficient));
		myPointVector.push_back(MyPoint(1*AmplificationCoefficient,1*AmplificationCoefficient));
		myPointVector.push_back(MyPoint(1*AmplificationCoefficient,0));
		myPointVector.push_back(MyPoint(1*AmplificationCoefficient,0));
		myPointVector.push_back(MyPoint(1*AmplificationCoefficient,0));
		myPointVector.push_back(MyPoint(2*AmplificationCoefficient,0));
		myPointVector.push_back(MyPoint(2*AmplificationCoefficient,1*AmplificationCoefficient));
		break;
	case 4:
		std::cout<<"�Զ�������ڵ���ƾ���B��������������Ƶ����꣨��Χ0.0-10.0����������"<<std::endl;
		customInputControlPoint();
		break;
	case 5:
		return 0;
		break;
	default:
		std::cout<<"�������ָ�����������������"<<std::endl;		
		return 0;
		break;
	}




	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(100,150);
	glutCreateWindow("wpsperson");
	glutDisplayFunc(myDisplay);
	myInit();
	glutMainLoop();


	return 0;
}












































