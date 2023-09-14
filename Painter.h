//
// Created by 24100 on 2022/3/14.
//

#ifndef FOOPLOT_PAINTER_H
#define FOOPLOT_PAINTER_H
#include <QFrame>
#include <QPainter>
#include <QModelIndex>
#include "MainWindow.h"
#include "Func/FuncListModel.h"

class Painter : public QFrame
{
	Q_OBJECT
	friend class MainWindow;
public:
	explicit Painter(QWidget * parent= nullptr);
	virtual void paintEvent(QPaintEvent *event)override;
	virtual void resizeEvent(QResizeEvent *event)override;
	int LxtoDx(double x) const;//逻辑坐标转设备坐标
	int LytoDy(double y) const;
	double DxtoLx(int x) const;//设备坐标转逻辑坐标
	double DytoLy(int y) const;
public:
	bool canDrawXy;
	bool canDrawGrid;
private:
	FuncListModel* m_pfuncList;
	double m_Xmin,m_Ymin,m_Xmax,m_Ymax;//函数数值范围（逻辑坐标范围）
	int m_top,m_bottom,m_left,m_right;//设备左边 上下左右范围 （设备坐标范围）
private:
	void drawFunc(QPainter& painter);//绘制函数
	void drawScale(QPainter& painter);//绘制刻度,刻度数值
	void drawAxis(QPainter& painter);//绘制坐标轴
	void drawGrid(QPainter& painter);//会hi在网格
};

#endif //FOOPLOT_PAINTER_H
