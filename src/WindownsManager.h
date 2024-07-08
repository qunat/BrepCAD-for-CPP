#pragma once
#include <QtWidgets>
#include<string>
#include <PartSolution.h>
#include<qobject.h>
#include <QDockWidget>
using namespace std;
class MainWindow;

class WindownsManager:public QObject
{
	Q_OBJECT
public:
	
	WindownsManager(MainWindow * par);
	int CreateNewWindown(string WindownName);
	int CreateNewWindown();
	
	QString GetCurrentWindown();

public:
	MainWindow* parent;
	QDockWidget * dock=nullptr;
	list<string> windowns_id_list{1};
	map<string, PartSolution*> windowns_name;
	QTabWidget *multiwindowstabwidget;
	string current_windown;

public slots:

	int TabwidgetChangeEvent();
	int TabwidgetClickedEvent();

};

