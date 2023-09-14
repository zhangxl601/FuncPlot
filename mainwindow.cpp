//
// Created by 24100 on 2022/3/9.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"
#include "Func/FuncCreate.h"
#include <QDebug>
#include <QWheelEvent>
#include <QColorDialog>
#include "Painter.h"

MainWindow::MainWindow(QWidget *parent) :
		QWidget(parent), ui(new Ui::MainWindow), oldPosX(0), oldPosY(0)
{
	ui->setupUi(this);
    setWindowTitle("函数绘图");
	ui->sdbXmin->installEventFilter(this);
	ui->sdbXMax->installEventFilter(this);
	ui->sdbYmin->installEventFilter(this);
	ui->sdbYMax->installEventFilter(this);
	ui->wdgFrame->installEventFilter(this);
	ui->sdbCXmin->installEventFilter(this);
	ui->sdbCXMax->installEventFilter(this);
	ui->listFunc->setModel(&m_funcList);
	connect(ui->pbtnAddfoo, &QPushButton::clicked, this, &MainWindow::gotoFuncEditor);
	connect(ui->pbtnReset, &QPushButton::clicked, this, [this]()
	{
		ui->wdgFrame->m_Ymin=-5;
		ui->wdgFrame->m_Ymax=5;
		ui->wdgFrame->m_Xmin=-10;
		ui->wdgFrame->m_Xmax=10;
		ui->sdbXmin->setValue(-10);
		ui->sdbXMax->setValue(10);
		ui->sdbYmin->setValue(-5);
		ui->sdbYMax->setValue(5);
		ui->wdgFrame->update();
	});
    drawFunc = new DrawFunc();
//    drawFunc->start();
//    drawFunc->setFlag(true);
    connect(this, &MainWindow::calc, drawFunc, &DrawFunc::calc);
    connect(drawFunc, &DrawFunc::over, this, [this]()
	{
		ui->wdgFrame->update();
	});
	//右键删除函数菜单
	ui->listFunc->setContextMenuPolicy(Qt::CustomContextMenu);
	menu = new QMenu(this);
	auto *act = new QAction(this);
	act->setText(tr("删除该函数"));
	menu->addAction(act);
	connect(ui->listFunc, &QListView::customContextMenuRequested, this, &MainWindow::delFuncMenu);
	connect(act, &QAction::triggered, this, &MainWindow::delFunc);
	//颜色数据处理
	ui->edtColor->installEventFilter(this);
	connect(ui->pbtnOK,&QPushButton::clicked,this,&MainWindow::updateFunc);
	//修改 函数设置显示
	connect(ui->listFunc,&QListView::clicked,this,&MainWindow::updateFuncSetting);//更新设置
	//是否绘制网格 和坐标
	connect(ui->cbGrid,&QCheckBox::stateChanged,this,&MainWindow::updateXyGrid);
	connect(ui->cbXy,&QCheckBox::stateChanged,this,&MainWindow::updateXyGrid);

	ui->listFunc->setCurrentIndex(m_funcList.index(0));
}


MainWindow::~MainWindow()
{
	delete ui;
    delete drawFunc;
}

void MainWindow::gotoFuncEditor()
{
	auto *funCreator = new FuncCreate(this);
	connect(funCreator, &FuncCreate::onOK, this, [this](Func *func)
	{
		ui->sdbCXMax->setValue(func->m_xMax);
		ui->sdbCXmin->setValue(func->m_xMin);
		ui->edtColor->setText(QString::fromStdString(func->m_color));
		ui->sbStep->setValue(func->m_step);
		ui->cbStyle->setCurrentIndex(func->m_style - 1);
		ui->sbWidth->setValue(func->m_width);
		ui->cbFuncCanDraw->setCheckState(Qt::Checked);
		m_funcList.addFunc(func);
		if(m_funcList.funcList.size()==1)//大小为1的时候更新设置
		{
			ui->listFunc->setCurrentIndex(m_funcList.index(0));
			updateFuncSetting(m_funcList.index(1));
		}

		emit calc(func);
	});
	funCreator->open();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *e)
{
	switch (e->type())
	{
		case QEvent::StyleAnimationUpdate://限制 范围选择
		{
			if (obj == ui->sdbXMax || obj == ui->sdbXmin || obj == ui->sdbYmin || obj == ui->sdbYMax)//限制绘图范围选择
			{
				if (ui->sdbXMax->value() - ui->sdbXmin->value() <= 1)
					ui->sdbXmin->setValue(ui->sdbXMax->value() - 1);
				if (ui->sdbYMax->value() - ui->sdbYmin->value() <= 1)
					ui->sdbYmin->setValue(ui->sdbYMax->value() - 1);
				updateXYrange();
			} else if (obj == ui->sdbCXMax || obj == ui->sdbCXmin)//限制计算范围选择
			{
				if( ui->sdbCXMax->value()-ui->sdbCXmin->value()<5)
				{
					ui->sdbCXmin->setValue(ui->sdbCXMax->value() - 5);
				}
			}
			return true;
		}
		case QEvent::Wheel://滚轮放大
		{
			if (obj == ui->wdgFrame)
			{
				auto *ee = static_cast<QWheelEvent *>(e);
				stretchXYrange(ee->angleDelta().y());
				return true;
			}
			break;
		}
		case QEvent::MouseButtonPress:
		{
			if (obj == ui->wdgFrame)//拖动
			{
				auto *me = static_cast<QMouseEvent *>(e);
				oldPosX = ui->wdgFrame->DxtoLx(me->x());
				oldPosY = ui->wdgFrame->DytoLy(me->y());
			} else if (obj == ui->edtColor)//颜色选择框
			{
				auto c = QColorDialog::getColor();
				if (c.isValid())
					ui->edtColor->setText(c.name());
			}
			return true;
		}
		case QEvent::MouseButtonRelease://拖动
			if (obj == ui->wdgFrame)
			{
				auto *me = static_cast<QMouseEvent *>(e);
				moveXYrange(me->pos());
				return true;
			}
			break;
		case QEvent::MouseMove:
			if (obj == ui->wdgFrame)
			{
				auto *me = static_cast<QMouseEvent *>(e);
				ui->edt_X->setText(QString::asprintf("%.2f", ui->wdgFrame->DxtoLx(me->x())));
				ui->edt_Y->setText(QString::asprintf("%.2f", ui->wdgFrame->DytoLy(me->y())));
				return true;
			}
			break;
	}
	return QWidget::eventFilter(obj, e);
}

void MainWindow::updateXYrange()
{
	ui->wdgFrame->m_Xmax = ui->sdbXMax->value();
	ui->wdgFrame->m_Xmin = ui->sdbXmin->value();
	ui->wdgFrame->m_Ymax = ui->sdbYMax->value();
	ui->wdgFrame->m_Ymin = ui->sdbYmin->value();
	ui->wdgFrame->update();
}

//鼠标拖动逻辑
void MainWindow::moveXYrange(QPoint releasePos)
{

	double nowX = ui->wdgFrame->DxtoLx(releasePos.x());
	double nowY = ui->wdgFrame->DytoLy(releasePos.y());
	double dx = nowX - oldPosX;
	double dy = nowY - oldPosY;

	double maxX = ui->sdbXMax->value() - dx;
	double minX = ui->sdbXmin->value() - dx;
	double maxY = ui->sdbYMax->value() - dy;
	double minY = ui->sdbYmin->value() - dy;

	//没有越范围 才移动
	if (maxX <= 99 && minX >= -99 &&
	    maxY <= 99 && minY >= -99)
	{
		ui->sdbXmin->setValue(minX);
		ui->sdbXMax->setValue(maxX);
		ui->sdbYmin->setValue(minY);
		ui->sdbYMax->setValue(maxY);
		updateXYrange();
	}
}

//鼠标滚轮逻辑
void MainWindow::stretchXYrange(int d)
{

	double xMax = ui->sdbXMax->value();
	double xMin = ui->sdbXmin->value();
	double yMax = ui->sdbYMax->value();
	double yMin = ui->sdbYmin->value();
	d = d / 40;
	//当放大到差值 小于等于 1的时候停止放大
	if (d < 0 || ((xMax - xMin > 1 || yMax - yMin > 1) && d != 0))
	{
		xMax -= d * 0.1;
		xMin += d * 0.1;
		yMax -= d * 0.1;
		yMin += d * 0.1;
		if (xMax <= 100 && xMin >= -99 &&
		    yMax <= 100 && yMin > -99&&
			xMax>xMin&&yMax>yMin)
		{
			ui->sdbXmin->setValue(xMin);
			ui->sdbXMax->setValue(xMax);
			ui->sdbYmin->setValue(yMin);
			ui->sdbYMax->setValue(yMax);
			updateXYrange();
		}

	}
}

void MainWindow::delFuncMenu(QPoint point)
{
	QModelIndex index = ui->listFunc->indexAt(point);
	if (index.isValid())
		menu->exec(QCursor::pos());
}

void MainWindow::delFunc()
{
	QModelIndex index = ui->listFunc->currentIndex();
	if (index.isValid())
	{
		m_funcList.rmFunc(index.row());
		ui->wdgFrame->update();
	}
}

void MainWindow::updateFunc()
{
	int row=ui->listFunc->currentIndex().row();
	if(row<m_funcList.funcList.size())
	{
		Func* func=m_funcList.funcList[row];
		func->m_style= ui->cbStyle->currentIndex()+1;
		func->m_color=ui->edtColor->text().toStdString();
		func->m_width=ui->sbWidth->value();
		func->canDraw=(ui->cbFuncCanDraw->checkState()==Qt::Checked);
		if(func->m_xMax!=ui->sdbCXMax->value()||func->m_xMin!=ui->sdbCXmin->value()||func->m_step!=ui->sbStep->value())
		{
			func->m_xMin=ui->sdbCXmin->value();
			func->m_xMax=ui->sdbCXMax->value();
			func->m_step=ui->sbStep->value();
			emit calc(func);
		}
		ui->wdgFrame->update();
	}
}

void MainWindow::updateFuncSetting(const QModelIndex &index)
{
	if (index.isValid())
	{
		Func *func=m_funcList.funcList[index.row()];
		ui->sdbCXMax->setValue(func->m_xMax);
		ui->sdbCXmin->setValue(func->m_xMin);
		ui->edtColor->setText(QString::fromStdString(func->m_color));
		ui->sbStep->setValue(func->m_step);
		ui->cbStyle->setCurrentIndex(func->m_style - 1);
		ui->sbWidth->setValue(func->m_width);
		if(func->canDraw)
			ui->cbFuncCanDraw->setCheckState(Qt::Checked);
		else
			ui->cbFuncCanDraw->setCheckState(Qt::Unchecked);
	}
}

void MainWindow::updateXyGrid(int x)
{
	QCheckBox* cb=static_cast<QCheckBox*>(sender());
	if(cb->text()=="显示坐标")
		ui->wdgFrame->canDrawXy=(x==2);
	else
		ui->wdgFrame->canDrawGrid=(x==2);
	ui->wdgFrame->update();
}
