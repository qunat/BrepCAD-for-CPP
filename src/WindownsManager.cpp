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

	//formDoc = new PartSolution(parent); //��ָ�������ڣ�������show()������ʾ
	windowns_name[WindownName]= new PartSolution(parent); //��ָ�������ڣ�������show()������ʾ
	windowns_name[WindownName]->setAttribute(Qt::WA_DeleteOnClose); //�ر�ʱ�Զ�ɾ��
	return true;
}