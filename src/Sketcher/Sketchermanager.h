#pragma once
#include"../GuiManager.h"
#include"../WindownsManager.h"
#include <QWidget>
#include<qobject.h>
#define DLLEXPORT __declspec(dllexport)

class MainWindow;
class DLLEXPORT Sketchermanager :public QObject
{
	Q_OBJECT
public:
	Sketchermanager(MainWindow*par);
	Sketchermanager();
	


public:
	MainWindow *parent;

public:
	void Showgui();

public slots:
	void mySlot();


};

