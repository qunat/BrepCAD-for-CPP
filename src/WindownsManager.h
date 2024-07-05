#pragma once
#include <QtWidgets>
#include<string>
#include <PartSolution.h>
#include<qobject.h>

using namespace std;
class MainWindow;

class WindownsManager
{
	
public:
	
	WindownsManager(MainWindow * par);

	int CreateNewWindown(string WindownName);
	int CreateNewWindown();
	int TabwidgetChangeEvent();

	QString GetCurrentWindown();

public:

	QWidget* parent;
	list<string> windowns_id_list{1};
	map<string, PartSolution*> windowns_name;
	QTabWidget *multiwindowstabwidget;
	string current_windown;
};

