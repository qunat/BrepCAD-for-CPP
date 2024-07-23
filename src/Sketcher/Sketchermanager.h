#pragma once
#include"../GuiManager.h"
#include"../WindownsManager.h"
#define DLLEXPORT __declspec(dllexport)

class MainWindow;
class DLLEXPORT Sketchermanager 
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

