#pragma once
#include<qobject.h>
class Test : public QObject
{
    Q_OBJECT
signals:
    void testsignal();
    // 参数的作用是数据传递, 谁调用信号函数谁就指定实参
    // 实参最终会被传递给槽函数
    void testsignal(int a);
};