#pragma once
#include <QtWidgets>
#include<string>
#include <PartSolution.h>
using namespace std;

class WindownsManager
{
public:
	
	WindownsManager(QWidget * par, QTabWidget* MultiWindowsTabWidget);

	int CreateNewWindown(string WindownName);
	int CreateNewWindown();

	string GetCurrentWindown();

public:

	QWidget* parent;
	list<string> windowns_id_list{1};
	map<string, PartSolution*> windowns_name;
	QTabWidget *multiwindowstabwidget;
	string current_windown;
};

