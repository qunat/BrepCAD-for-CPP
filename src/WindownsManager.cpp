#include "WindownsManager.h"
#include <string>
#include<QString>
#include<qobject.h>
#include<GuiManege.h>
#include"DockModelTree.h"
#include <QDockWidget>
using namespace std;

WindownsManager::WindownsManager(MainWindow* par)
{
	
	parent = par;
	multiwindowstabwidget = par->MultiWindowsTabWidget;
	connect(multiwindowstabwidget, SIGNAL(currentChanged(int)), this, SLOT(TabwidgetChangeEvent()));

	
}

int WindownsManager::CreateNewWindown(string WindownName)
{
	
	//formDoc = new PartSolution(parent); //不指定父窗口，单独用show()方法显示
	windowns_name[WindownName]= new PartSolution(parent); //不指定父窗口，单独用show()方法显示
	windowns_name[WindownName]->setAttribute(Qt::WA_DeleteOnClose); //关闭时自动删除
	return true;
}

int WindownsManager::CreateNewWindown()
{
	//formDoc = new PartSolution(parent); //不指定父窗口，单独用show()方法显示
	string WindownName = "零件" + to_string(windowns_id_list.size());
	current_windown=WindownName;
	windowns_name[WindownName] = new PartSolution(parent); //不指定父窗口，单独用show()方法显示
	windowns_name[WindownName]->setAttribute(Qt::WA_DeleteOnClose); //关闭时自动删除
	windowns_id_list.push_back(to_string(windowns_id_list.size()+1));

	int dock_width = 300;
	dock = new QDockWidget(QString::fromLocal8Bit("组合浏览器"), parent);
	dock->setMinimumSize(QSize(dock_width, 360));
	parent->dockmodeltreemap[current_windown]= new DockModelTree(parent);
	parent->addDockWidget(Qt::LeftDockWidgetArea, dock);
	//ribbon->setCurrentIndex(0);
	//ribbon->updateRibbonGeometry();
	//this->dockmodeltree->UpdateGeometry();
	return 0;
}

int WindownsManager::TabwidgetChangeEvent()
{	

	current_windown = this->GetCurrentWindown().toStdString();
	return 0;
}

QString WindownsManager::GetCurrentWindown()
{
	int index = multiwindowstabwidget->currentIndex();
	QString name = multiwindowstabwidget->tabText(index);
	return name;
}