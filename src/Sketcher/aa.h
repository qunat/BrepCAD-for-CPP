#pragma once
#include<qobject.h>
class Test : public QObject
{
    Q_OBJECT
signals:
    void testsignal();
    // ���������������ݴ���, ˭�����źź���˭��ָ��ʵ��
    // ʵ�����ջᱻ���ݸ��ۺ���
    void testsignal(int a);
};