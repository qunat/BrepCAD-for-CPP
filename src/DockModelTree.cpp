﻿#include "DockModelTree.h"
#include <QDockWidget>
#include <QtCore/qobject.h>
#include <QFile>
#include <QTreeWidget>
#include <QtWidgets>
#include <QStyleFactory>
#include <QDebug>
#include"GuiManeger.h"

DockModelTree::DockModelTree(MainWindow* par)
{	
	parent = par;
	int dock_width = 360;
	dock_tabWidget=new QTabWidget(parent->windownsmanager->dock);
	dock_tabWidget->setLayoutDirection(Qt::LeftToRight);
	dock_tabWidget->setTabPosition(QTabWidget::North);
	tab_1 = new QWidget(dock_tabWidget);
	tab_1->setObjectName(QStringLiteral("tab"));
	dock_tabWidget->addTab(tab_1, QString());
	tab_2 = new QWidget(dock_tabWidget);
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
	modeltree = new QTreeWidget(this->tab_2);
	//qDebug() << ModelTree << modeltree;
	modeltree->expandAll();
	modeltree->setStyle(QStyleFactory::create("windows"));
	modeltree->setColumnCount(2);//设置列数
	modeltree->setHeaderLabels(QStringList() << "名称" << "附件" << "最新");//  设置树形控件头部的标题
	modeltree->setAlternatingRowColors(	true);
	modeltree->setColumnWidth(0,180);
	

	history_model_root = new QTreeWidgetItem(modeltree);
	history_model_root->setText(0, "历史模型记录");
	//history_model_root->setIcon(0, QIcon('sync.ico'));
	history_model_root->setCheckState(0, Qt::Checked);

	//坐标系
	wsc_root = new QTreeWidgetItem(history_model_root);
	wsc_root->setText(0,"坐标系");
	//wsc_root->setIcon(0, QPixmap(":/icon/icons/装配.png"));
	wsc_root->setCheckState(0, Qt::Checked);

	//基准面
	datum_root_X= new QTreeWidgetItem(history_model_root);
	datum_root_X->setText(0, "X基准平面");
	datum_root_X->setIcon(0, QPixmap(":/icon/icons/datumplane.png"));
	datum_root_X->setCheckState(0, Qt::Checked);

	//基准面
	datum_root_Y = new QTreeWidgetItem(history_model_root);
	datum_root_Y->setText(0, "Y基准平面");
	datum_root_Y->setIcon(0, QPixmap(":/icon/icons/datumplane.png"));
	datum_root_Y->setCheckState(0, Qt::Checked);

	//基准面
	datum_root_Z = new QTreeWidgetItem(history_model_root);
	datum_root_Z->setText(0, "Z基准平面");
	datum_root_Z->setIcon(0, QPixmap(":/icon/icons/datumplane.png"));
	datum_root_Z->setCheckState(0, Qt::Checked);

	modeltree->expandAll();

	

}
void DockModelTree::UpdateGeometry()
{
	QTabBar* tabBar0 = this->dock_tabWidget->findChild<QTabBar*>();//获取tavwidge的标题
	QTabBar* tabBar1 = dock->findChild<QTabBar*>();//获取dock的标题
	int dock_width = this->dock_tabWidget->geometry().width();
	int dock_height = this->dock_tabWidget->geometry().height();
	int dock_tabWidget_QTabBar_height= tabBar0->geometry().height();
	int dock_QTabBar_height= tabBar1->geometry().height();
	this->modeltree->setGeometry(0, 0, dock_width,dock_height-dock_tabWidget_QTabBar_height- dock_QTabBar_height);//重行设置模型树大小
	qDebug()<< this->dock_tabWidget->geometry().height();
	qDebug() << tabBar0->geometry().height();
	qDebug() << tabBar1->geometry().height();
	//dock_tabWidget->setGeometry(0, 30, width, height);
	


}