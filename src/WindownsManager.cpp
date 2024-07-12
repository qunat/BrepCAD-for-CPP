#include "WindownsManager.h"
#include <string>
#include<QString>
#include<qobject.h>
#include<GuiManeger.h>
#include"DockModelTree.h"
#include <QDockWidget>
#include<Qstring>
#include"display/DisplayCore.h"
#include <QFileDialog>
#include <qmessagebox.h>
using namespace std;

WindownsManager::WindownsManager(MainWindow* par)
{
	
	parent = par;
	multiwindowstabwidget = par->MultiWindowsTabWidget;
	connect(multiwindowstabwidget, SIGNAL(currentChanged(int)), this, SLOT(TabwidgetChangeEvent()));
	connect(multiwindowstabwidget, SIGNAL(tabBarClicked(int)), this, SLOT(TabwidgetClickedEvent()));

	
}

int WindownsManager::CreateNewWindown(string WindownName)
{
	
	QString fileName = QFileDialog::getOpenFileName(
		parent,
		QString::fromLocal8Bit("选择打开文件"),
		"c:/",
		tr("step(*step *stp )"));
	
	QStringList fileNameList = fileName.split("/");
	//qDebug() << fileNameList[fileName.size()-1].toLocal8Bit();
	fileName = fileNameList[fileNameList.size() - 1];
	WindownName = fileName.toLocal8Bit();

	current_windown = WindownName;
	windowns_name[WindownName] = new PartSolution(parent); //不指定父窗口，单独用show()方法显示
	windowns_name[WindownName]->setAttribute(Qt::WA_DeleteOnClose); //关闭时自动删除
	windowns_id_list.push_back(to_string(windowns_id_list.size() + 1));

	//Create DisplayCoreManeger
	parent->DisplayCoreManeger[WindownName] = new DisplayCore(windowns_name[WindownName]->myOccView);

	int dock_width = 300;
	if (dock == nullptr)
	{
		dock = new QDockWidget(QString::fromLocal8Bit("组合浏览器"), parent);
		dock->setMinimumSize(QSize(dock_width, 360));
		parent->dockmodeltreemap[current_windown] = new DockModelTree(parent);
		parent->addDockWidget(Qt::LeftDockWidgetArea, dock);
	}

	else
	{
		parent->dockmodeltreemap[current_windown] = new DockModelTree(parent);
		parent->addDockWidget(Qt::LeftDockWidgetArea, dock);
		dock->setWidget(parent->dockmodeltreemap[current_windown]->ModelTree);
	}

	//ribbon->setCurrentIndex(0);
	//ribbon->updateRibbonGeometry();
	//this->dockmodeltree->UpdateGeometry();
	
	return 0;
}

int WindownsManager::CreateNewWindown()
{
	//formDoc = new PartSolution(parent); //不指定父窗口，单独用show()方法显示
	string WindownName = "零件" + to_string(windowns_id_list.size());
	current_windown=WindownName;
	windowns_name[WindownName] = new PartSolution(parent); //不指定父窗口，单独用show()方法显示
	windowns_name[WindownName]->setAttribute(Qt::WA_DeleteOnClose); //关闭时自动删除
	windowns_id_list.push_back(to_string(windowns_id_list.size()+1));

	//Create DisplayCoreManeger
	parent->DisplayCoreManeger[WindownName] = new DisplayCore(windowns_name[WindownName]->myOccView);

	int dock_width = 300;
	if (dock == nullptr)
	{
		dock = new QDockWidget(QString::fromLocal8Bit("组合浏览器"), parent);
		dock->setMinimumSize(QSize(dock_width, 360));
		parent->dockmodeltreemap[current_windown] = new DockModelTree(parent);
		parent->addDockWidget(Qt::LeftDockWidgetArea, dock);
	}

	else
	{
		parent->dockmodeltreemap[current_windown] = new DockModelTree(parent);
		parent->addDockWidget(Qt::LeftDockWidgetArea, dock);
		dock->setWidget(parent->dockmodeltreemap[current_windown]->ModelTree);
	}
	
	//ribbon->setCurrentIndex(0);
	//ribbon->updateRibbonGeometry();
	//this->dockmodeltree->UpdateGeometry();
	return 0;
}
int WindownsManager::TabwidgetClickedEvent()
{
	if (parent->dockmodeltreemap.find(current_windown) == parent->dockmodeltreemap.end() && dock != nullptr)
	{
		dock->setWidget(parent->dockmodeltreemap[current_windown]->ModelTree);//modeltree 切换

	}
	return 0;

}

int WindownsManager::TabwidgetChangeEvent()
{	

	current_windown = this->GetCurrentWindown().toLocal8Bit();
	dock->setWidget(parent->dockmodeltreemap[current_windown]->ModelTree);//modeltree 切换
	multiwindowstabwidget->updateGeometry();
	
	
	return 0;
}

QString WindownsManager::GetCurrentWindown()
{
	int index = multiwindowstabwidget->currentIndex();
	QString name = multiwindowstabwidget->tabText(index);
	return name;
}