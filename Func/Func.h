//
// Created by 24100 on 2022/3/15.
//

#ifndef FUNCPLOT_FUNC_H
#define FUNCPLOT_FUNC_H
#include "FuncParse.h"

//所有函数的基类
//存储计算的数据，函数划线的颜色 样式
class Func
{
public:
	Func();
//	Func(Func && other);
//	const Func& operator=(Func&&);
//	Func(const Func&)=delete;
//	const Func& operator=(const Func&)=delete;
	virtual void calculate()=0;//处理计算
public:
	enum class Type {
		Unknown=0,
		Normal
	};
	Type type;
 	std::string m_color;//划线的颜色
	std::string m_expr;//函数解析式
	int m_style;
	int m_width;//线宽度
	std::vector<std::pair<double,double>> m_pointList;
	std::vector<std::any> m_postExpr;//后缀表达式
	int m_step;//x方向 的步长
	double m_xMax;//计算范围
	double m_xMin;
	bool canDraw;
};

/*
 * 一般一元方程
 *
 */
class NormalFunc:public Func
{
public:
	NormalFunc();
	void calculate()override;
};
#endif //FUNCPLOT_FUNC_H
