//
// Created by 24100 on 2022/3/17.
//

#include "Computor.h"


DrawFunc::DrawFunc(QObject *parent)
{}

void DrawFunc::calc(Func* func)
{
    start();
    m_func = func;
    setFlag(true);
}

void DrawFunc::setFlag(bool f)
{
    flag = f;
}

void DrawFunc::run()
{
    while (1) {
        if(flag){
            m_func->calculate();
            emit over();
            flag = false;
        }
        QThread::msleep(1000);
    }

}
