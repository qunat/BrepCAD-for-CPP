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
	return 0;
}

string WindownsManager::GetCurrentWindown()
{
	int index = multiwindowstabwidget->setCurrentIndex();
	return "123";
}