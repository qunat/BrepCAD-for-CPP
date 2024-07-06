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
	
	//formDoc = new PartSolution(parent); //��ָ�������ڣ�������show()������ʾ
	windowns_name[WindownName]= new PartSolution(parent); //��ָ�������ڣ�������show()������ʾ
	windowns_name[WindownName]->setAttribute(Qt::WA_DeleteOnClose); //�ر�ʱ�Զ�ɾ��
	return true;
}

int WindownsManager::CreateNewWindown()
{
	//formDoc = new PartSolution(parent); //��ָ�������ڣ�������show()������ʾ
	string WindownName = "���" + to_string(windowns_id_list.size());
	current_windown=WindownName;
	windowns_name[WindownName] = new PartSolution(parent); //��ָ�������ڣ�������show()������ʾ
	windowns_name[WindownName]->setAttribute(Qt::WA_DeleteOnClose); //�ر�ʱ�Զ�ɾ��
	windowns_id_list.push_back(to_string(windowns_id_list.size()+1));

	int dock_width = 300;
	dock = new QDockWidget(QString::fromLocal8Bit("��������"), parent);
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