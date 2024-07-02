#include "WindownsManager.h"
#include <string>
#include<QString>
#include<qobject.h>
using namespace std;
WindownsManager::WindownsManager(QWidget* par, QTabWidget* MultiWindowsTabWidget)
{
	parent = par;
	multiwindowstabwidget = MultiWindowsTabWidget;

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

string WindownsManager::GetCurrentWindown()
{
	int index = multiwindowstabwidget->setCurrentIndex();
	return "123";
}