#pragma once
#include <QDockWidget>
#include <qtabwidget.h>
#include <QTreeWidget>
#include <QObject>
#include <QDebug>
#include <QScrollArea>
class MainWindow;

class DockModelTree
{
public:
	DockModelTree(MainWindow*par);
	~DockModelTree();
	void UpdateGeometry();
	void CreateModelTree();
public slots:
	void mySlot() {
		qDebug() << "Slot called!";
	}
public:
	MainWindow* parent;
	//QDockWidget  *dock;
	QTabWidget* dock_tabWidget;
	QScrollArea* scroll;
	QWidget* part_tab;
	QWidget* assemble_tab;
	QTreeWidget* modeltree;
	QTreeWidgetItem* history_model_root;
	QTreeWidgetItem* wsc_root;
	QTreeWidgetItem* datum_root_X;
	QTreeWidgetItem* datum_root_Y;
	QTreeWidgetItem* datum_root_Z;


};
