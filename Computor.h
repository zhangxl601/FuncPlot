//
// Created by 24100 on 2022/3/17.
//

#ifndef FUNCPLOT_DrawFunc_H
#define FUNCPLOT_DrawFunc_H
#include <QObject>
#include "Func/Func.h"
#include <QThread>

class DrawFunc:public QThread
{
Q_OBJECT
public:
    explicit DrawFunc(QObject *parent = nullptr);
public slots:
	void calc(Func* func);

    void setFlag(bool f);
protected:
    virtual void run() override;
signals:
	void over();
private:
    bool flag = false;
    Func* m_func = nullptr;
};


#endif //FUNCPLOT_DrawFunc_H
