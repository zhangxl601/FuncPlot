//
// Created by 24100 on 2022/3/14.
//

#include "Painter.h"

#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <cmath>
#include <QPainterPath>

Painter::Painter(QWidget *parent)
: QFrame(parent),m_bottom(0),m_left(0),m_top(0),m_right(0),
  m_Ymin(-10),m_Ymax(10),m_Xmin(-10),m_Xmax(10)
{
	m_pfuncList =&static_cast<MainWindow*>(parent)->m_funcList;//获取 函数文档 中的 绘图信息
	canDrawXy=true;
	canDrawGrid=true;
	setMouseTracking(true);//mousemove
}
void Painter::resizeEvent(QResizeEvent *event)
{
	//更新 绘图大小 和 转换比率
	m_top= static_cast<int>(round(rect().bottom()*0.08));
	m_bottom= static_cast<int>(round(rect().bottom()*0.92));
	m_left= static_cast<int>(round(rect().right()*0.08));
	m_right= static_cast<int>(round(rect().right()*0.92));
}
void Painter::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

    painter.fillRect(rect(), QColor(20, 20, 20));

	if(canDrawXy)
		drawAxis(painter);
	if(canDrawGrid)
		drawGrid(painter);
	drawScale(painter);
	drawFunc(painter);
}

void Painter::drawFunc(QPainter &painter)
{

	//绘制函数
	for(auto& item:m_pfuncList->funcList)
	{
		if(item->canDraw)
		{
			QPen pen(item->m_color.c_str());
			pen.setStyle(static_cast< Qt::PenStyle>(item->m_style));
			pen.setWidth(item->m_width);
			painter.setPen(pen);
			bool shouldMove=true;
			QPainterPath path;
			for(auto& i:item->m_pointList)
			{
				int x=LxtoDx(i.first);
				int y= LytoDy(i.second);
				if(shouldMove)
				{
					if(x>m_left&&x<m_right&&y>m_top&&y<m_bottom)
					{
						shouldMove= false;
						path.moveTo(x,y);
					}else
						shouldMove=true;
				}
				else if(x>m_left&&x<m_right&&y>m_top&&y<m_bottom)
				{
					path.lineTo(x,y);
				}else
					shouldMove=true;
			}
			painter.drawPath(path);
		}
	}
}

int Painter::LxtoDx(double x) const
{
	return static_cast<int>(m_left+(x-m_Xmin)*(m_right-m_left)/(m_Xmax-m_Xmin));
}

double Painter::DxtoLx(int x) const
{
	return m_Xmin+(x-m_left)*(m_Xmax-m_Xmin)/(m_right-m_left);
}
int Painter::LytoDy(double y) const
{
	return static_cast<int>(m_bottom - (y-m_Ymin)* (m_bottom-m_top) /(m_Ymax-m_Ymin));
}

double Painter::DytoLy(int y) const
{
	return m_Ymin+(m_bottom-y)*(m_Ymax-m_Ymin)/(m_bottom-m_top);
}

void Painter::drawGrid(QPainter &painter)
{
    painter.setPen(QPen(Qt::white,1.0,Qt::DashDotLine));
	for(int i=m_left;i<m_right;i+=50)
	{
		painter.drawLine(i,m_bottom,i,m_top);
	}
	//y 方向
	for(int i=m_bottom;i>m_top;i-=50)
	{
		painter.drawLine(m_left,i,m_right,i);
	}
}

void Painter::drawAxis(QPainter &painter)
{
    painter.setPen(QPen(Qt::green,2.0,Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin));
	painter.setFont(QFont("微软雅黑",15));
	//绘制轴
	int Ox= LxtoDx(0);
	int Oy= LytoDy(0);
	if(Oy>=m_top&&Oy<=m_bottom)
	{
		//x方向
		painter.drawLine(m_left,Oy,m_right+10,Oy);
		painter.drawText(m_right+10,Oy+10,"X轴");
		painter.drawLine(m_right+10,Oy,m_right-5,Oy+5);
		painter.drawLine(m_right+10,Oy,m_right-5,Oy-5);
	}

	if(Ox>=m_left&&Ox<=m_right)
	{
		//y方向
		painter.drawLine(Ox,m_top-10,Ox,m_bottom);
		painter.drawText(Ox+10,m_top-10,"Y轴");
		painter.drawLine(Ox,m_top-10,Ox+5,m_top-5);
		painter.drawLine(Ox,m_top-10,Ox-5,m_top-5);
	}
}

void Painter::drawScale(QPainter &painter)
{
    painter.setPen(QPen(Qt::white,1.0,Qt::DashDotLine));
    painter.setFont(QFont("微软雅黑",10));
	//x方向
	//painter.drawLine(m_left,m_bottom+5,m_right+20,m_bottom+5);
	painter.drawLine(m_left,m_bottom,m_right,m_bottom);
	for(int i=m_left;i<m_right;i+=50)
	{
		painter.drawLine(i,m_bottom,i,m_bottom+3);
		painter.drawText(i-8,m_bottom+20,QString::asprintf("%.2f",DxtoLx(i)));
	}
	//y 方向
	painter.drawLine(m_left,m_top,m_left,m_bottom);
	for(int i=m_bottom;i>m_top;i-=50)
	{
        painter.drawLine(m_left-5,i,m_left-2,i);
        painter.drawText(m_left-40,i+3,QString::asprintf("%.2f",DytoLy(i)));
	}
}
