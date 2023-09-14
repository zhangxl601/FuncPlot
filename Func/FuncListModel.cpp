#include "FuncListModel.h"


FuncListModel::FuncListModel(QObject *parent) : QAbstractListModel(parent)
{
	std::vector<std::any> post;
	Func* f=new NormalFunc();

    f->m_expr="sinx";
    f->m_color="#F00";
    yxqin::getPostfix(f->m_expr,post);
    f->m_postExpr=post;
    f->calculate();
    addFunc(f);

}

int FuncListModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	return funcList.size();
}

QVariant FuncListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
	{
		return {};
	}

	int nRow = index.row();
	if (nRow >= rowCount() || nRow < 0)
	{
		return {};
	}
	switch (role)
	{
		case Qt::DisplayRole:
			return QString("f(x)= " + QString::fromStdString(funcList[nRow]->m_expr));
	}
	return {};
}

void FuncListModel::addFunc(Func *func)
{
	int row = funcList.size();
	beginInsertRows(QModelIndex(), row, row);
	this->funcList.push_back(func);
	endInsertRows();
}

FuncListModel::~FuncListModel()
{
	for (auto &i: funcList)
	{
		delete i;
		i= nullptr;
	}
}

void FuncListModel::rmFunc(int row)
{
	if (row < funcList.size())
	{
		beginRemoveRows(QModelIndex(), row, row);
		Func *f = funcList[row];
		funcList.erase(funcList.begin() + row);
		delete f;
		endRemoveRows();
	}
}
