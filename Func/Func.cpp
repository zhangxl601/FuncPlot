//
// Created by 24100 on 2022/3/15.
//

#include <cfloat>
#include <QDebug>
#include "Func.h"

Func::Func()
{
	this->m_color="#000";
	this->m_style=1;
	this->m_xMax=10;
	this->m_xMin=-10;
	this->m_step=10000;
	this->m_width=2;
	this->canDraw=false;
	type=Type::Unknown;
}
NormalFunc::NormalFunc():Func::Func()
{
	type=Type::Normal;
}

void NormalFunc::calculate()
{
	this->canDraw=false;
	double step=(m_xMax-m_xMin)/m_step;
	m_pointList.clear();
	for(double x=m_xMin;x<m_xMax;x+=step)
	{
		double y;
		if(yxqin::getResult(m_postExpr,y,'x',x))
			m_pointList.push_back(std::make_pair(x,y));
		else
			m_pointList.push_back(std::make_pair(DBL_MAX,DBL_MIN));
	}
	this->canDraw=true;
}
