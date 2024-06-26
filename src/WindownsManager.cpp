#include "WindownsManager.h"

WindownsManager::WindownsManager(QWidget* parent)
{
	parent = parent;


}

int WindownsManager::CreateNewWindown(string WindownName)
{
	if (WindownName.empty())
	{

	}

	//formDoc = new PartSolution(parent); //不指定父窗口，单独用show()方法显示
	windowns_name[WindownName]= new PartSolution(parent); //不指定父窗口，单独用show()方法显示
	windowns_name[WindownName]->setAttribute(Qt::WA_DeleteOnClose); //关闭时自动删除
	return true;
}