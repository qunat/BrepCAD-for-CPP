#pragma once
#include"../GuiManager.h"
#include"../WindownsManager.h"

class MainWindow;
class Sketchermanager
{
public:
	Sketchermanager(MainWindow*par);
	Sketchermanager();
	~Sketchermanager();


public:
	MainWindow *parent;

public:
	void Showgui();

};

