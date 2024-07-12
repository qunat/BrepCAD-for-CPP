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
		QString::fromLocal8Bit("ѡ����ļ�"),
		"c:/",
		tr("step(*step *stp )"));
	
	QStringList fileNameList = fileName.split("/");
	//qDebug() << fileNameList[fileName.size()-1].toLocal8Bit();
	fileName = fileNameList[fileNameList.size() - 1];
	WindownName = fileName.toLocal8Bit();

	current_windown = WindownName;
	windowns_name[WindownName] = new PartSolution(parent); //��ָ�������ڣ�������show()������ʾ
	windowns_name[WindownName]->setAttribute(Qt::WA_DeleteOnClose); //�ر�ʱ�Զ�ɾ��
	windowns_id_list.push_back(to_string(windowns_id_list.size() + 1));

	//Create DisplayCoreManeger
	parent->DisplayCoreManeger[WindownName] = new DisplayCore(windowns_name[WindownName]->myOccView);

	int dock_width = 300;
	if (dock == nullptr)
	{
		dock = new QDockWidget(QString::fromLocal8Bit("��������"), parent);
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
	//formDoc = new PartSolution(parent); //��ָ�������ڣ�������show()������ʾ
	string WindownName = "���" + to_string(windowns_id_list.size());
	current_windown=WindownName;
	windowns_name[WindownName] = new PartSolution(parent); //��ָ�������ڣ�������show()������ʾ
	windowns_name[WindownName]->setAttribute(Qt::WA_DeleteOnClose); //�ر�ʱ�Զ�ɾ��
	windowns_id_list.push_back(to_string(windowns_id_list.size()+1));

	//Create DisplayCoreManeger
	parent->DisplayCoreManeger[WindownName] = new DisplayCore(windowns_name[WindownName]->myOccView);

	int dock_width = 300;
	if (dock == nullptr)
	{
		dock = new QDockWidget(QString::fromLocal8Bit("��������"), parent);
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
		dock->setWidget(parent->dockmodeltreemap[current_windown]->ModelTree);//modeltree �л�

	}
	return 0;

}

int WindownsManager::TabwidgetChangeEvent()
{	

	current_windown = this->GetCurrentWindown().toLocal8Bit();
	dock->setWidget(parent->dockmodeltreemap[current_windown]->ModelTree);//modeltree �л�
	multiwindowstabwidget->updateGeometry();
	
	
	return 0;
}

QString WindownsManager::GetCurrentWindown()
{
	int index = multiwindowstabwidget->currentIndex();
	QString name = multiwindowstabwidget->tabText(index);
	return name;
}