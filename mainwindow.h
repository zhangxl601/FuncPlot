#ifndef UNTITLED_MAINWINDOW_H
#define UNTITLED_MAINWINDOW_H

#include <QWidget>
#include "Func/FuncListModel.h"
#include "Computor.h"
#include <QMenu>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QWidget
{
Q_OBJECT
friend class Painter;
public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow() override;
	bool eventFilter(QObject * obj, QEvent * e) override;//过滤 范围选择控件的窗口
	void updateXYrange();
private:
	void moveXYrange(QPoint releasePos);
	void stretchXYrange(int d);
private slots:
	void gotoFuncEditor();
    void delFuncMenu(QPoint point);
	void delFunc();
	void updateFunc();
	void updateFuncSetting(const QModelIndex &index);
	void updateXyGrid(int x);
signals:
	void calc(Func* func);
private:
	Ui::MainWindow *ui;
	FuncListModel m_funcList;
	double oldPosX;
	double oldPosY;
    DrawFunc* drawFunc;
	QMenu* menu;
};


#endif //UNTITLED_MAINWINDOW_H
