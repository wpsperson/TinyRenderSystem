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

#define MinPointNum 4					//3次B样条最少需要4个控制点才能绘制一段样条。
#define AmplificationCoefficient 100	//给定的控制点的坐标是1，需要放大才能在Opengl中绘制。
#define NodeStep 0.005					//节点变化的步长



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

//根据输入的控制点来绘制B样条曲线
void drawBasicSpline(const std::vector<MyPoint> &vect)
{
	if (vect.size()< 4)
	{
		std::cout<<"给定的控制点数目太少，不能实现绘制！"<<std::endl;
	}
	else
	{
		//每次使用4个控制点生成一小段B样条。
		std::vector<MyPoint>::const_iterator constItrStart = vect.begin();
		std::vector<MyPoint>::const_iterator constItrEnd = vect.begin()+3;
		//
		std::vector<MyPoint>::const_iterator constItrActive;
		while (constItrEnd != vect.end())
		{			
			//对于每一次的四个控制点，从t=0 到t=1进行德布尔算法。
			for (double t = 0.0; t <= 1; t += NodeStep)
			{
				constItrActive = constItrStart;
				//第一次迭代。
				MyPoint temp1_1 = findPointByProportion(*(constItrActive),*(constItrActive+1),(2+t)/3);
				constItrActive ++;
				MyPoint temp1_2 = findPointByProportion(*(constItrActive),*(constItrActive+1),(1+t)/3);
				constItrActive ++;
				MyPoint temp1_3 = findPointByProportion(*(constItrActive),*(constItrActive+1),(t)/3);
				//第二次迭代
				MyPoint temp2_1 = findPointByProportion(temp1_1, temp1_2, (1+t)/2);
				MyPoint temp2_2 = findPointByProportion(temp1_2, temp1_3, t/2);
				//第三次迭代
				MyPoint temp3 = findPointByProportion(temp2_1, temp2_2, t);
				glVertex2d(temp3.x, temp3.y);

			}
			
			//最后将这两个迭代器自加，使得下一个循环中处理下四个控制点的B样条。
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
	std::cout<<"请输入第 1 个节点的横坐标"<<std::endl;
	double temp,pointX,PointY;
	int num = 0;
	while (std::cin>>temp)
	{
		num++;//用于判断奇数偶数
		if (num%2 == 1)
		{
			pointX = temp;
			std::cout<<"请输入第"<<(int)((num/2)+1)<<"个节点的纵坐标"<<std::endl;
		}
		else
		{
			PointY = temp;
			myPointVector.push_back(MyPoint(pointX*AmplificationCoefficient,PointY*AmplificationCoefficient));
			std::cout<<"成功插入第"<<(int)((num/2))<<"个节点 （"<<pointX<<","<<PointY<<")"<<std::endl;
			std::cout<<"请输入第"<<(int)((num/2)+1)<<"个节点的横坐标"<<std::endl;

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
	std::cout<<"请选择您要进行的操作："<<std::endl;
	std::cout<<"按数字\"1\"将给定的节点绘制出 均匀B样条"<<std::endl;
	std::cout<<"按数字\"2\"将给定的节点绘制出 准均匀B样条"<<std::endl;
	std::cout<<"按数字\"3\"将给定的节点绘制出 尖角B样条"<<std::endl;
	std::cout<<"按数字\"4\"将请输入节点绘制出 均匀B样条"<<std::endl;
	std::cout<<"按数字\"5\"退出该程序！"<<std::endl;
	int OptionNum;
	std::cin>>OptionNum;
	switch (OptionNum)
	{
	case 1:
		std::cout<<"给定的节点绘制均匀B样条图形如下："<<std::endl;
		myPointVector.push_back(MyPoint(0,0));
		myPointVector.push_back(MyPoint(0,1*AmplificationCoefficient));
		myPointVector.push_back(MyPoint(1*AmplificationCoefficient,1*AmplificationCoefficient));
		myPointVector.push_back(MyPoint(1*AmplificationCoefficient,0));
		myPointVector.push_back(MyPoint(2*AmplificationCoefficient,0));
		myPointVector.push_back(MyPoint(2*AmplificationCoefficient,1*AmplificationCoefficient));		
		break;
	case 2:
		std::cout<<"给定的节点绘制准均匀B样条图形如下："<<std::endl;
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
		std::cout<<"给定的节点绘制带尖角的B样条图形如下："<<std::endl;
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
		std::cout<<"自定义输入节点绘制均匀B样条，请输入控制点坐标（范围0.0-10.0浮点数）："<<std::endl;
		customInputControlPoint();
		break;
	case 5:
		return 0;
		break;
	default:
		std::cout<<"您输入的指令错误，请重新来过！"<<std::endl;		
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












































