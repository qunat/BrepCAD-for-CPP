#pragma once
#include <QtWidgets>
#include<string>
#include <PartSolution.h>
using namespace std;

class WindownsManager
{
public:
	WindownsManager(QWidget* parent);

	int CreateNewWindown(string WindownName);

public:

	QWidget* parent;
	double windowns_id{0};
	map<string, PartSolution*> windowns_name;
};

