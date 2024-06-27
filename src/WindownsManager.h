#pragma once
#include <QtWidgets>
#include<string>
#include <PartSolution.h>
using namespace std;

class WindownsManager
{
public:
	WindownsManager(QWidget* par);

	int CreateNewWindown(string WindownName);
	int CreateNewWindown();

public:

	QWidget* parent;
	double windowns_id{1};
	map<string, PartSolution*> windowns_name;
};

