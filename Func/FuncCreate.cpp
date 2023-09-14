//
// Created by 24100 on 2022/3/17.
//

// You may need to build the project (run Qt uic code generator) to get "ui_FuncCreate.h" resolved

#include "FuncCreate.h"
#include "ui_FuncCreate.h"
#include "FuncEditor.h"
#include <QColorDialog>
#include "ui_FuncEditor.h"
FuncCreate::FuncCreate(QWidget *parent) :
		QDialog(parent), ui(new Ui::FuncCreate)
{
	ui->setupUi(this);
	func= nullptr;
	ui->pushButton->installEventFilter(this);
	ui->pbtnOK->installEventFilter(this);
	ui->lineEdit->installEventFilter(this);
	ui->edtColor->installEventFilter(this);
	ui->sdbCXmin->installEventFilter(this);
	ui->sdbCXMax->installEventFilter(this);
}

bool FuncCreate::eventFilter(QObject *obj, QEvent *e)
{
	switch (e->type())
	{
		case QEvent::StyleAnimationUpdate:
		{
			if(obj==ui->sdbCXMax||ui->sdbCXmin)
			{
				if(ui->sdbCXMax->value() -ui->sdbCXmin->value()<5)
				{
					ui->sdbCXmin->setValue(ui->sdbCXMax->value()-5);
				}
			}
			return true;
		}
		case QEvent::MouseButtonPress:
		{
			if(obj==ui->edtColor)
			{
				auto c=QColorDialog::getColor();
				if(c.isValid())
					ui->edtColor->setText(c.name());
			}else if(obj==ui->pushButton||obj==ui->lineEdit)
			{
				FuncEditor* editor=new FuncEditor(this);
				editor->ui->lineEdit->setText(ui->lineEdit->text());
				connect(editor,&FuncEditor::onOK,this, &FuncCreate::setFunc);
				editor->open();
			}else if(obj==ui->pbtnOK)
			{

				if(func!= nullptr)
				{
					func->m_style= ui->cbStyle->currentIndex()+1;
					func->m_color=ui->edtColor->text().toStdString();
					func->m_xMin=ui->sdbCXmin->value();
					func->m_xMax=ui->sdbCXMax->value();
					func->m_step=ui->sbStep->value();
					func->m_width=ui->sbWidth->value();
					emit onOK(func);
				}
				close();
			}
			break;
		}
	}
	return QDialog::eventFilter(obj,e);
}

FuncCreate::~FuncCreate()
{
	delete ui;
}

void FuncCreate::setFunc(std::string &expr,std::vector<std::any>& post)
{
	func=new NormalFunc;
	func->m_expr=std::move(expr);
	func->m_postExpr=std::move(post);
	ui->lineEdit->setText(QString::fromStdString(func->m_expr));
}