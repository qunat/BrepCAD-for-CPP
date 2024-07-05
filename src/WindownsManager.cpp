#include "WindownsManager.h"
#include <string>
#include<QString>
#include<qobject.h>
#include<GuiManege.h>
using namespace std;
WindownsManager::WindownsManager(MainWindow* par)
{
	

	parent = par;
	multiwindowstabwidget = par->MultiWindowsTabWidget;
	//connect(multiwindowstabwidget, SIGNAL(multiwindowstabwidget->currentChanged()), this, SLOT(TabwidgetChangeEvent()));
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
	return 0;
}

int WindownsManager::TabwidgetChangeEvent()
{	

	qDebug() << 123;
	return 0;
}

QString WindownsManager::GetCurrentWindown()
{
	int index = multiwindowstabwidget->currentIndex();
	QString name = multiwindowstabwidget->tabText(index);
	return name;
}