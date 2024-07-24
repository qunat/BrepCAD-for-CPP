#pragma once
#include"../GuiManager.h"
#include"../WindownsManager.h"
#include <QWidget>
#define DLLEXPORT __declspec(dllexport)

class MainWindow;
class DLLEXPORT Sketchermanager //:public QObject
{
	//Q_OBJECT
public:
	Sketchermanager(MainWindow*par);
	Sketchermanager();
	~Sketchermanager();


public:
	MainWindow *parent;

public:
	void Showgui();

};

