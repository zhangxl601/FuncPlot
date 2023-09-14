//
// Created by 24100 on 2022/3/14.
//

#ifndef FOOPLOT_FUNCEDITOR_H
#define FOOPLOT_FUNCEDITOR_H

#include <QDialog>
#include <QSet>
#include <QMessageBox>
#include "Func/FuncParse.h"
#include "FuncCreate.h"
QT_BEGIN_NAMESPACE
namespace Ui
{
class FuncEditor;
}
QT_END_NAMESPACE
/**
 *@brief 函数编辑器
 *
 * 输入函数表达式并且检查表达式是否错误，做出相应的限制
 */
class FuncEditor : public QDialog
{
Q_OBJECT
friend class FuncCreate;
public:
	explicit FuncEditor(QWidget *parent = nullptr);
	~FuncEditor() override;
	bool eventFilter(QObject * obj, QEvent * e) override;//将 LineEdit 所有键盘输入全部过滤掉
private slots:
	void backspace();
	void  btnFuncClick();
signals:
	void onOK(std::string& expr,std::vector<std::any>&  post);
private:
	Ui::FuncEditor *ui;
	//对输入进行判断一定的限制
	void  inputFilter(const QString& in);//输入过滤器
	int   brackets;//右括号 还没有匹配的个数
	static QSet<QString> numSet;
	static QSet<QString> xySet;//其他符号集合
	static QSet<QString> funcSet;
	static QSet<QString> optSet;
	static QSet<QString> num1Set;//可能为值的set用于 自动添加*
};


#endif //FOOPLOT_FUNCEDITOR_H
