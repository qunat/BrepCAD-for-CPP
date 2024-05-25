#pragma once
#include <QDockWidget>
#include <qtabwidget.h>
#include <QTreeWidget>
#include <QObject>
#include <QDebug>

class DockModelTree
{
public:
	DockModelTree(QWidget*parent);
	~DockModelTree();
	void UpdateGeometry();
	void CreateModelTree();
public slots:
	void mySlot() {
		qDebug() << "Slot called!";
	}
public:
	QDockWidget  *dock;
	QTabWidget* dock_tabWidget;
	QWidget* tab_1;
	QWidget* tab_2;
	QTreeWidget* ModelTree;

};
