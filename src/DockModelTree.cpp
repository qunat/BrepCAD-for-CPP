#include "DockModelTree.h"
#include <QDockWidget>
#include <QtCore/qobject.h>
#include <QFile>
#include <QTreeWidget>
#include <QtWidgets>
#include <QStyleFactory>
#include <QDebug>
#include"GuiManeger.h"

DockModelTree::DockModelTree(MainWindow* parent)
{	
	parent = parent;
	int dock_width = 360;
	dock_tabWidget=new QTabWidget(parent->windownsmanager->dock);
	dock_tabWidget->setLayoutDirection(Qt::LeftToRight);
	dock_tabWidget->setTabPosition(QTabWidget::North);
	tab_1 = new QWidget();
	tab_1->setObjectName(QStringLiteral("tab"));
	dock_tabWidget->addTab(tab_1, QString());
	tab_2 = new QWidget();
	tab_2->setObjectName(QStringLiteral("tab_2"));
	dock_tabWidget->setAttribute(Qt::WA_StyledBackground);
	dock_tabWidget->setIconSize(QSize(20, 20));
	dock_tabWidget->addTab(tab_2, QString());
	dock_tabWidget->setTabText(0, "");
	dock_tabWidget->setTabText(1, "");
	dock_tabWidget->setGeometry(QRect(0,30, dock_width, 864));
	dock_tabWidget->setTabIcon(0, QPixmap(":/icon/icons/零件.png"));
	dock_tabWidget->setTabIcon(1, QPixmap(":/icon/icons/装配.png"));
	this->CreateModelTree();

	/*设置qss*/
	QFile  qss(".\\Qss\\docktawidget.qss");
	if (qss.open(QFile::ReadOnly))
	{
		QString style = QLatin1String(qss.readAll());
		dock_tabWidget->setStyleSheet(style);
		qss.close();
	}
}
void DockModelTree::CreateModelTree()
{
	QTreeWidget* modeltree = new QTreeWidget(this->tab_1);
	ModelTree = modeltree;
	qDebug() << ModelTree << modeltree;
	ModelTree->setStyle(QStyleFactory::create("windows"));
	ModelTree->setColumnCount(2);//设置列数
	ModelTree->setHeaderLabels(QStringList() << "名称" << "附件" << "最新");//  设置树形控件头部的标题
	ModelTree->setAlternatingRowColors(	true);
	ModelTree->setColumnWidth(0,120);

	QTreeWidgetItem* history_model_root = new QTreeWidgetItem(ModelTree);
	history_model_root->setText(0, "历史模型记录");
	//history_model_root->setIcon(0, QIcon('sync.ico'));
	history_model_root->setCheckState(0, Qt::Checked);
}
void DockModelTree::UpdateGeometry()
{
	QTabBar* tabBar0 = this->dock_tabWidget->findChild<QTabBar*>();//获取tavwidge的标题
	QTabBar* tabBar1 = dock->findChild<QTabBar*>();//获取dock的标题
	int dock_width = this->dock_tabWidget->geometry().width();
	int dock_height = this->dock_tabWidget->geometry().height();
	int dock_tabWidget_QTabBar_height= tabBar0->geometry().height();
	int dock_QTabBar_height= tabBar1->geometry().height();
	this->ModelTree->setGeometry(0, 0, dock_width,dock_height-dock_tabWidget_QTabBar_height- dock_QTabBar_height);//重行设置模型树大小
	qDebug()<< this->dock_tabWidget->geometry().height();
	qDebug() << tabBar0->geometry().height();
	qDebug() << tabBar1->geometry().height();
	//dock_tabWidget->setGeometry(0, 30, width, height);
	


}