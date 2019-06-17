/*
*author:	wpsperson
*date:		20141012
*function:	polyfill
*ps：		可以用来交作业（最好改一改），但为了尊重原作者，请不要传到网上去。
**/

#include <iostream>
#include <vector>
#include <list>
#include<windows.h>
#include<gl/Gl.h>
#include<gl/glu.h>
#include"glut.h"
//定义扫描线的范围。
#define  YMAX 500   
#define  YMIN 0
#define  precision 0.01 //浮点数判断相等
//表示单个的顶点。
class Point
{
public:
	Point(){}
	Point(float x,float y):_xCoordinate(x),_yCoordinate(y){}
	float _xCoordinate;
	float _yCoordinate;
};

//表示多边形。用一组顶点来表示一个多边形，要求输入的顶点必须按照多边形连线顺序输入
class ClassMyPolygon
{	
public:
	ClassMyPolygon(){}
	std::vector<Point> _polygonPointlist; 
	void pushOnePoint(float x, float y)
	{
		_polygonPointlist.push_back(Point(x,y));
	}
	std::vector<Point> getPointList() const
	{
		return _polygonPointlist;
	}
	void printPolygon()
	{
		int i = 0;
		for (std::vector<Point>::iterator itr=_polygonPointlist.begin(); itr!=_polygonPointlist.end(); itr++)
		{	
			i++;
			std::cout<<"第"<<i<<"个顶点的坐标是： "<<(*itr)._yCoordinate<<"， "<<(*itr)._yCoordinate<<std::endl;
		}
	}
};

//表示一种颜色。
class ColorClass
{
public:
	ColorClass(float red, float green, float blue):_red(red),_green(green),_blue(blue){}
	float _red,_green,_blue;
};

//考虑到活性边表和新边表都是边表，通用该结构体
struct ET
{
	ET(){}
	ET(float x, float dx, int ymax):xBeginCord(x),dxPerY(dx),yMaxCord(ymax){}
	float xBeginCord,dxPerY;
	int yMaxCord;
	ET* pNextET;
};

//实现将一个元素插入到链表中去,不排序，只是按照添加的先后顺序。
void insertET(ET* &et_list, ET* &_et)
{
	if (et_list == NULL)
	{
		et_list = _et;
	}
	else
	{
		ET* rear = et_list;
		while (rear->pNextET != NULL)//使得rear指向尾节点（倒数第二节点的指针部分）。
		{
			rear = rear->pNextET;
		}
		//这才是改变链表的核心语句。
		rear ->pNextET = _et;
	}
	
}

//判断第一个元素比第二个小？
bool littleThan(ET* firstET, ET* secondET)
{
	if (firstET == NULL)
	{
		std::cout<<"程序littleThan中第1参数为NULL，出错！ "<<std::endl;
	}
	if (secondET == NULL)//按照插入的规则，如果x最大，则插在最后，那么NULL最大
	{
		std::cout<<"程序littleThan中第2参数为NULL，出错！ "<<std::endl;
	}
	if ( (firstET->xBeginCord < secondET->xBeginCord) || 
		( fabs(firstET->xBeginCord - secondET->xBeginCord) < precision && (firstET->dxPerY < secondET->dxPerY) ))//所指节点比另一个小，
	{
		return true;		

	}
	return false;
}

//实现将一个链表NET_YSCAN中的所有节点按照 x 值排序插入到活性表中
void insertAETwithRank(ET* &AET, ET* &NET_YSCAN)
{
	//NET_YSCAN = NULL什么都不执行
	while (NET_YSCAN != NULL)
	{
		ET* temp_NET_YScan = NET_YSCAN;//temp_NET_YScan 将NET_YSCAN所指的节点“摘”出来，作为一个独立的节点。
		NET_YSCAN = NET_YSCAN->pNextET;//用完NET_YSCAN所指节点后，NET_YSCAN指向下一个,最终NET_YSCAN自毁。
		temp_NET_YScan->pNextET = NULL;

		
		if (AET == NULL)//首先排除掉AET为NULL的情况。
		{
			AET = temp_NET_YScan;//已插入，插在尾部。
			continue;
		}

		ET* temp = AET;//只要AET不是NULL，则完全用temp胜任。
		if (littleThan(temp_NET_YScan,AET))//判断首节点。
		{
			temp_NET_YScan->pNextET = AET;
			AET = temp_NET_YScan;//已插入，插入到链表首部。
			continue;
		}
		else
		{
			while(temp->pNextET != NULL)//判断中间某两个之间节点。
			{
				if (littleThan(temp,temp_NET_YScan)&&littleThan(temp_NET_YScan,temp->pNextET))
				{
					//此时temp所指的节点下一个位置插入temp_NET_YScan
					temp_NET_YScan->pNextET = temp->pNextET;
					temp->pNextET = temp_NET_YScan;
					break;
				}
				temp = temp->pNextET;					
			}
			//运行到此一定是temp_NET_YScan比尾部节点还要大。
			if (temp->pNextET == NULL)//判断尾节点。
			{			
				temp->pNextET = temp_NET_YScan;	//已插入，插入到链表尾部，则应该在此搞好。				
				continue;
			}
		}


		
	}
}

//活性边表的更新
void updateAET(ET* &AET)
{
	ET* temp = AET;
	while(temp != NULL)
	{
		temp->xBeginCord += temp->dxPerY;
		temp = temp->pNextET;
	}
}

//用配对法进行绘制相应行的点。
void drawScan(ColorClass color,int YScan, ET* AET)
{
	glColor3f(color._red, color._green, color._blue); //指定颜色
	glPointSize(1.0f); //点大小
	glBegin(GL_POINTS);
	if (AET != NULL)
	{
		ET* tempBack = AET;
		ET* tempFore = AET->pNextET;
		while (tempBack!=NULL&&tempFore!=NULL)
		{
			for(int i = tempBack->xBeginCord; i<tempFore->xBeginCord; i++)
			{
				glVertex2i(i,YScan);
				
			}
			tempBack = tempBack->pNextET;//向前传递两个
			tempBack = tempBack->pNextET;
			tempFore = tempFore->pNextET;//此时他们俩走在同一个位置。
			if (tempFore == NULL)//表明活性表到头了。
			{
				break;
			}
			tempFore = tempFore->pNextET;
		}
	}
	std::cout<<"绘制完了第 "<<YScan<<" 行！  "<<std::endl;
	glEnd();
}

//删除AET中 值是YScan的节点。
void deleteETnode(int YScan,ET* &AET)
{
	if (AET != NULL)
	{		
		ET*  tempBack= AET;
		ET*  tempFore= AET->pNextET;
		while (tempFore != NULL)//从第二个节点一直到最后
		{
			if (tempFore->yMaxCord == YScan)
			{
				tempBack->pNextET = tempFore->pNextET;//此处删除了该节点。
				delete(tempFore);
				tempFore = tempBack->pNextET;
			}
			else
			{
				tempBack = tempFore;
				tempFore = tempFore->pNextET;
			}
		}
		//最后再判断第一个节点是否满足条件。
		if (AET->yMaxCord == YScan)
		{	
			ET* temp = AET;			
			AET = AET->pNextET;//删除第一个节点
			delete(temp); //释放内存。
		}

	}

	
}


//核心程序---扫描线算法
void polyfill(ClassMyPolygon polygon, ColorClass color)
{
	//首先是建立新边表	
	//新边表初始化为Null(有多少扫面线就有多少新边表。)
	std::vector<ET*> NET(YMAX-YMIN+1,NULL);
	std::vector<Point> polygonPointList = polygon.getPointList();	
	int lineNum = polygonPointList.size();
	//i和j分别表示多边形每一条边两个端点的序号。特别地，最后一个点和第一个点来组成线段。
	int j = 0; 
	for (int i=0; i<lineNum; i++)
	{	
		j = i+1;
		if (j == lineNum)
		{
			j = 0;
		}
		if (polygonPointList[i]._yCoordinate<polygonPointList[j]._yCoordinate)
		{
			int pos = polygonPointList[i]._yCoordinate;
			float tempx = polygonPointList[i]._xCoordinate;
			float tempDx = (polygonPointList[j]._xCoordinate-polygonPointList[i]._xCoordinate)/
							(polygonPointList[j]._yCoordinate-polygonPointList[i]._yCoordinate);
			int tempYMax = polygonPointList[j]._yCoordinate;
			//是否NET[pos]为空，在函数内部有判断。
			ET* temp = new ET(tempx, tempDx, tempYMax);
			temp->pNextET = NULL;//最新初始的链表节点，必须处理好指针问题。
			insertET(NET[pos], temp);			
		}
		else if(polygonPointList[i]._yCoordinate > polygonPointList[j]._yCoordinate)
		{
			int pos = polygonPointList[j]._yCoordinate;
			float tempx = polygonPointList[j]._xCoordinate;
			float tempDx = (polygonPointList[j]._xCoordinate-polygonPointList[i]._xCoordinate)/
				(polygonPointList[j]._yCoordinate-polygonPointList[i]._yCoordinate);
			int tempYMax = polygonPointList[i]._yCoordinate;
			//是否NET[pos]为空，在函数内部有判断。
			ET* temp = new ET(tempx, tempDx, tempYMax);
			temp->pNextET = NULL;
			insertET(NET[pos], temp);
		}		
	}

	std::cout<<"程序运行到此  "<<std::endl;

	//活性边表。
	ET* AET = NULL;//C++程序如果有指针，一定要想指到NULL，以防万一。
	int YScan = YMIN;
	for (; YScan <= YMAX; YScan++)
	{
		if (NET[YScan] != NULL)//当插入新边表时
		{
			insertAETwithRank(AET, NET[YScan]);	
			//如果YScan == 活性边表中的ET任意节点的YMax，则删除活性边表中的该节点。
			deleteETnode(YScan,AET); //先删除后绘制，防止出现重画。
			drawScan(color,YScan,AET);
		}
		else
		{
			drawScan(color,YScan,AET);
			deleteETnode(YScan,AET);
		}
		

		//活性边表中x相应变化。此最易。
		updateAET(AET);
	}



}




void myInit(void)
{
	glClearColor(1.0,1.0,1.0,0.0);
	glColor3f(0.0f,0.0f,0.0f);
	glPointSize(1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,640.0,0.0,480.0);
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	ColorClass myFavoriteColor(1.0,0.0,0.0);	//用红色绘制
	ClassMyPolygon pentagram;					//测试用的五角星形状
	pentagram.pushOnePoint(181,26);
	pentagram.pushOnePoint(143,133);
	pentagram.pushOnePoint(19,133);
	pentagram.pushOnePoint(119,199);
	pentagram.pushOnePoint(82,306);
	pentagram.pushOnePoint(181,242);
	pentagram.pushOnePoint(279,305);
	pentagram.pushOnePoint(242,199);
	pentagram.pushOnePoint(342,133);
	pentagram.pushOnePoint(218,133);
	polyfill(pentagram,myFavoriteColor);




	glEnd();
	glFlush();
}

int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(100,150);
	glutCreateWindow("ZY14DF118 王培帅 HomeWork2 ");
	glutDisplayFunc(myDisplay);
	myInit();
	glutMainLoop();
	return 0;
}

