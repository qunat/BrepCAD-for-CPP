#pragma once

#include <QMainWindow>
#include<QtCore/Qstring>

class OccView;
class PartSolution : public QMainWindow
{
    Q_OBJECT

public:
    PartSolution(QWidget *parent = nullptr);
	
	void createActions(void);

    ~PartSolution();

public slots:
	void open_part(void);

public:
	OccView* myOccView;
};
