#ifndef FUNCLISTMODEL_H
#define FUNCLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include "Func.h"
class FuncListModel : public QAbstractListModel
{
    Q_OBJECT
public:
	explicit FuncListModel(QObject *parent = nullptr);
	virtual ~FuncListModel();
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	void addFunc(Func* func);
	void rmFunc(int row);
	std::vector<Func*> funcList;
};

#endif // FUNCLISTMODEL_H
