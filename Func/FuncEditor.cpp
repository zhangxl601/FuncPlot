//
// Created by 24100 on 2022/3/14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_FooEditor.h" resolved

#include "FuncEditor.h"
#include "ui_FuncEditor.h"
#include <QDebug>
#include <QKeyEvent>

QSet<QString> FuncEditor::numSet={"0","1","2","3","4","5","6","7","8","9"};
QSet<QString> FuncEditor::xySet={"x","pi","e"};
QSet<QString> FuncEditor::optSet={"+","-","*","/","^"};
QSet<QString> FuncEditor::num1Set={"x","i","e",")"};
QSet<QString> FuncEditor::funcSet={"cos","sin","tan","ln","lg"};
FuncEditor::FuncEditor(QWidget *parent) :
		QDialog(parent), ui(new Ui::FuncEditor)
{
	ui->setupUi(this);
	ui->lineEdit->installEventFilter(this);
	ui->lineEdit->setAttribute(Qt::WA_InputMethodEnabled,false);//禁用输入法输入
	auto btnList = findChildren<QPushButton*>();
	connect(ui->pbtnOk,&QPushButton::clicked,this,&FuncEditor::btnFuncClick);
	brackets=0;
	for(auto& btn:btnList)
	{
		if(btn->text()=="DEL")
			connect(btn,&QPushButton::clicked,this,&FuncEditor::backspace);
		else if(btn->text()=="AC")
			connect(btn,&QPushButton::clicked,this,[this]()
			{
				ui->lineEdit->setText("");
				brackets=0;
			});
		else
			connect(btn,&QPushButton::clicked,this,[this]()
			{
				auto* btn = dynamic_cast<QPushButton*>(sender());
				inputFilter(btn->text());
			});
	}
}

FuncEditor::~FuncEditor()
{
	delete ui;
}


bool FuncEditor::eventFilter(QObject *obj, QEvent *e)
{
	if(obj==ui->lineEdit)
	{
		if(e->type()==QEvent::KeyPress)
			return true;
	}
	return QDialog::eventFilter(obj,e);
}

void FuncEditor::backspace()
{
	QString text = ui->lineEdit->text();
	if(!text.isEmpty())
	{
		switch (text.rbegin()->toLatin1())
		{
			case 's'://cos
				text.remove(text.length()-3,3);
				break;
			case 'g'://lg
			case 'i'://pi
				text.remove(text.length()-2,2);
				break;
			case 'n'://tan sin ln
			{
				if(*(text.cend()-2)=="l")
					text.remove(text.length()-2,2);
				else
					text.remove(text.length()-3,3);
				break;
			}
			case '(':
			{
				text.remove(text.length()-1,1);
				--brackets;
				break;
			}
			case ')':
			{
				text.remove(text.length()-1,1);
				++brackets;
				break;
			}
			default://1 2 3 ......
				text.remove(text.length()-1,1);
				break;
		}
		ui->lineEdit->setText(text);
	}
}
void FuncEditor::inputFilter(const QString& in)
{
	QString cur=ui->lineEdit->text();
	QString back;
	if(!cur.isEmpty())
		back.push_back(cur.back());

	if(numSet.contains(in))//数 或者函数  前面存在元素 x pi e ) 等 值元素 自动填充 *
	{
		if(num1Set.contains(back))
			cur=cur+"*"+in;
		else
			cur+=in;

	}else if(in==".")//小数点 一个数不可以存在 多个小数点
	{
		if(numSet.contains(back))
		{
			bool f=true;
			for(auto i=cur.rbegin();i!=cur.rend();++i)
			{
				if(*i==".")
				{
					f=false;
					break;
				}
				if(!numSet.contains(*i))
					break;
			}
			cur=f?cur+".":cur;
		}
	}else if(optSet.contains(in))//除了 ‘-’ 前一个必须是数值    -如果前面前面是空or‘(’ 可以写入
	{
		if(in=="-")
		{
			if(back.isEmpty()||back=="(")
				cur+=in;
			else if(back=="+")
				cur.replace(cur.length()-1,"-");
			else if(numSet.contains(back)||num1Set.contains(back))
				cur+=in;
		}else if(numSet.contains(back)||num1Set.contains(back))
				cur+=in;
	}else if(funcSet.contains(in)||xySet.contains(in))//
	{
		if(num1Set.contains(back)||numSet.contains(back))
			cur=cur+"*"+in;
		else
			cur+=in;
	}else if(in=='(')// 右括号数+1 当前一个符号是数值的时候自动填写乘号
	{
		if(numSet.contains(back)||num1Set.contains(back))
			cur=cur+"*"+in;
		else
			cur+='(';
		++brackets;
	}else if(in==')')//只有没有匹配的括号数 大于1 并且 最后一个符号必须是数值
	{
		if(brackets>0&&(numSet.contains(back)||num1Set.contains(back)))
		{
			cur+=')';
			--brackets;
		}
	}
	ui->lineEdit->setText(cur);
}

void FuncEditor::btnFuncClick()
{
	if(brackets==0)
	{
		auto expr=ui->lineEdit->text().toStdString();
		std::vector<std::any> post;

		if(!yxqin::getPostfix(expr,post))
			QMessageBox::information(this,tr("提示"),tr("函数表达式有错误"));
		else
		{
			emit onOK(expr,post);
			close();
		}
	}
}
