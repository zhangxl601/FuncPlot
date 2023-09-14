//
// Created by 24100 on 2022/3/17.
//

#ifndef FUNCPLOT_FUNCCREATE_H
#define FUNCPLOT_FUNCCREATE_H

#include <QDialog>
#include "Func.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class FuncCreate;
}
QT_END_NAMESPACE

class FuncCreate : public QDialog
{
Q_OBJECT

public:
	explicit FuncCreate(QWidget *parent = nullptr);
	~FuncCreate() override;
	bool eventFilter(QObject *, QEvent *) override;
public slots:
	void setFunc(std::string& expr,std::vector<std::any>&  post);
signals:
	 void onOK(Func* func);
private:
	Ui::FuncCreate *ui;
	Func* func;
};


#endif //FUNCPLOT_FUNCCREATE_H
