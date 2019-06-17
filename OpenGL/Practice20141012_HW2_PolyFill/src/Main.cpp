/*
*author:	wpsperson
*date:		20141012
*function:	polyfill
*ps��		������������ҵ����ø�һ�ģ�����Ϊ������ԭ���ߣ��벻Ҫ��������ȥ��
**/

#include <iostream>
#include <vector>
#include <list>
#include<windows.h>
#include<gl/Gl.h>
#include<gl/glu.h>
#include"glut.h"
//����ɨ���ߵķ�Χ��
#define  YMAX 500   
#define  YMIN 0
#define  precision 0.01 //�������ж����
//��ʾ�����Ķ��㡣
class Point
{
public:
	Point(){}
	Point(float x,float y):_xCoordinate(x),_yCoordinate(y){}
	float _xCoordinate;
	float _yCoordinate;
};

//��ʾ����Ρ���һ�鶥������ʾһ������Σ�Ҫ������Ķ�����밴�ն��������˳������
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
			std::cout<<"��"<<i<<"������������ǣ� "<<(*itr)._yCoordinate<<"�� "<<(*itr)._yCoordinate<<std::endl;
		}
	}
};

//��ʾһ����ɫ��
class ColorClass
{
public:
	ColorClass(float red, float green, float blue):_red(red),_green(green),_blue(blue){}
	float _red,_green,_blue;
};

//���ǵ����Ա߱���±߱��Ǳ߱�ͨ�øýṹ��
struct ET
{
	ET(){}
	ET(float x, float dx, int ymax):xBeginCord(x),dxPerY(dx),yMaxCord(ymax){}
	float xBeginCord,dxPerY;
	int yMaxCord;
	ET* pNextET;
};

//ʵ�ֽ�һ��Ԫ�ز��뵽������ȥ,������ֻ�ǰ�����ӵ��Ⱥ�˳��
void insertET(ET* &et_list, ET* &_et)
{
	if (et_list == NULL)
	{
		et_list = _et;
	}
	else
	{
		ET* rear = et_list;
		while (rear->pNextET != NULL)//ʹ��rearָ��β�ڵ㣨�����ڶ��ڵ��ָ�벿�֣���
		{
			rear = rear->pNextET;
		}
		//����Ǹı�����ĺ�����䡣
		rear ->pNextET = _et;
	}
	
}

//�жϵ�һ��Ԫ�رȵڶ���С��
bool littleThan(ET* firstET, ET* secondET)
{
	if (firstET == NULL)
	{
		std::cout<<"����littleThan�е�1����ΪNULL������ "<<std::endl;
	}
	if (secondET == NULL)//���ղ���Ĺ������x�������������ôNULL���
	{
		std::cout<<"����littleThan�е�2����ΪNULL������ "<<std::endl;
	}
	if ( (firstET->xBeginCord < secondET->xBeginCord) || 
		( fabs(firstET->xBeginCord - secondET->xBeginCord) < precision && (firstET->dxPerY < secondET->dxPerY) ))//��ָ�ڵ����һ��С��
	{
		return true;		

	}
	return false;
}

//ʵ�ֽ�һ������NET_YSCAN�е����нڵ㰴�� x ֵ������뵽���Ա���
void insertAETwithRank(ET* &AET, ET* &NET_YSCAN)
{
	//NET_YSCAN = NULLʲô����ִ��
	while (NET_YSCAN != NULL)
	{
		ET* temp_NET_YScan = NET_YSCAN;//temp_NET_YScan ��NET_YSCAN��ָ�Ľڵ㡰ժ����������Ϊһ�������Ľڵ㡣
		NET_YSCAN = NET_YSCAN->pNextET;//����NET_YSCAN��ָ�ڵ��NET_YSCANָ����һ��,����NET_YSCAN�Ի١�
		temp_NET_YScan->pNextET = NULL;

		
		if (AET == NULL)//�����ų���AETΪNULL�������
		{
			AET = temp_NET_YScan;//�Ѳ��룬����β����
			continue;
		}

		ET* temp = AET;//ֻҪAET����NULL������ȫ��tempʤ�Ρ�
		if (littleThan(temp_NET_YScan,AET))//�ж��׽ڵ㡣
		{
			temp_NET_YScan->pNextET = AET;
			AET = temp_NET_YScan;//�Ѳ��룬���뵽�����ײ���
			continue;
		}
		else
		{
			while(temp->pNextET != NULL)//�ж��м�ĳ����֮��ڵ㡣
			{
				if (littleThan(temp,temp_NET_YScan)&&littleThan(temp_NET_YScan,temp->pNextET))
				{
					//��ʱtemp��ָ�Ľڵ���һ��λ�ò���temp_NET_YScan
					temp_NET_YScan->pNextET = temp->pNextET;
					temp->pNextET = temp_NET_YScan;
					break;
				}
				temp = temp->pNextET;					
			}
			//���е���һ����temp_NET_YScan��β���ڵ㻹Ҫ��
			if (temp->pNextET == NULL)//�ж�β�ڵ㡣
			{			
				temp->pNextET = temp_NET_YScan;	//�Ѳ��룬���뵽����β������Ӧ���ڴ˸�á�				
				continue;
			}
		}


		
	}
}

//���Ա߱�ĸ���
void updateAET(ET* &AET)
{
	ET* temp = AET;
	while(temp != NULL)
	{
		temp->xBeginCord += temp->dxPerY;
		temp = temp->pNextET;
	}
}

//����Է����л�����Ӧ�еĵ㡣
void drawScan(ColorClass color,int YScan, ET* AET)
{
	glColor3f(color._red, color._green, color._blue); //ָ����ɫ
	glPointSize(1.0f); //���С
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
			tempBack = tempBack->pNextET;//��ǰ��������
			tempBack = tempBack->pNextET;
			tempFore = tempFore->pNextET;//��ʱ����������ͬһ��λ�á�
			if (tempFore == NULL)//�������Ա�ͷ�ˡ�
			{
				break;
			}
			tempFore = tempFore->pNextET;
		}
	}
	std::cout<<"�������˵� "<<YScan<<" �У�  "<<std::endl;
	glEnd();
}

//ɾ��AET�� ֵ��YScan�Ľڵ㡣
void deleteETnode(int YScan,ET* &AET)
{
	if (AET != NULL)
	{		
		ET*  tempBack= AET;
		ET*  tempFore= AET->pNextET;
		while (tempFore != NULL)//�ӵڶ����ڵ�һֱ�����
		{
			if (tempFore->yMaxCord == YScan)
			{
				tempBack->pNextET = tempFore->pNextET;//�˴�ɾ���˸ýڵ㡣
				delete(tempFore);
				tempFore = tempBack->pNextET;
			}
			else
			{
				tempBack = tempFore;
				tempFore = tempFore->pNextET;
			}
		}
		//������жϵ�һ���ڵ��Ƿ�����������
		if (AET->yMaxCord == YScan)
		{	
			ET* temp = AET;			
			AET = AET->pNextET;//ɾ����һ���ڵ�
			delete(temp); //�ͷ��ڴ档
		}

	}

	
}


//���ĳ���---ɨ�����㷨
void polyfill(ClassMyPolygon polygon, ColorClass color)
{
	//�����ǽ����±߱�	
	//�±߱��ʼ��ΪNull(�ж���ɨ���߾��ж����±߱�)
	std::vector<ET*> NET(YMAX-YMIN+1,NULL);
	std::vector<Point> polygonPointList = polygon.getPointList();	
	int lineNum = polygonPointList.size();
	//i��j�ֱ��ʾ�����ÿһ���������˵����š��ر�أ����һ����͵�һ����������߶Ρ�
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
			//�Ƿ�NET[pos]Ϊ�գ��ں����ڲ����жϡ�
			ET* temp = new ET(tempx, tempDx, tempYMax);
			temp->pNextET = NULL;//���³�ʼ������ڵ㣬���봦���ָ�����⡣
			insertET(NET[pos], temp);			
		}
		else if(polygonPointList[i]._yCoordinate > polygonPointList[j]._yCoordinate)
		{
			int pos = polygonPointList[j]._yCoordinate;
			float tempx = polygonPointList[j]._xCoordinate;
			float tempDx = (polygonPointList[j]._xCoordinate-polygonPointList[i]._xCoordinate)/
				(polygonPointList[j]._yCoordinate-polygonPointList[i]._yCoordinate);
			int tempYMax = polygonPointList[i]._yCoordinate;
			//�Ƿ�NET[pos]Ϊ�գ��ں����ڲ����жϡ�
			ET* temp = new ET(tempx, tempDx, tempYMax);
			temp->pNextET = NULL;
			insertET(NET[pos], temp);
		}		
	}

	std::cout<<"�������е���  "<<std::endl;

	//���Ա߱�
	ET* AET = NULL;//C++���������ָ�룬һ��Ҫ��ָ��NULL���Է���һ��
	int YScan = YMIN;
	for (; YScan <= YMAX; YScan++)
	{
		if (NET[YScan] != NULL)//�������±߱�ʱ
		{
			insertAETwithRank(AET, NET[YScan]);	
			//���YScan == ���Ա߱��е�ET����ڵ��YMax����ɾ�����Ա߱��еĸýڵ㡣
			deleteETnode(YScan,AET); //��ɾ������ƣ���ֹ�����ػ���
			drawScan(color,YScan,AET);
		}
		else
		{
			drawScan(color,YScan,AET);
			deleteETnode(YScan,AET);
		}
		

		//���Ա߱���x��Ӧ�仯�������ס�
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

	ColorClass myFavoriteColor(1.0,0.0,0.0);	//�ú�ɫ����
	ClassMyPolygon pentagram;					//�����õ��������״
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
	glutCreateWindow("ZY14DF118 ����˧ HomeWork2 ");
	glutDisplayFunc(myDisplay);
	myInit();
	glutMainLoop();
	return 0;
}

