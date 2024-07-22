#include "DockModelTree.h"
#include <QDockWidget>
#include <QtCore/qobject.h>
#include <QFile>
#include <QTreeWidget>
#include <QtWidgets>
#include <QStyleFactory>
#include <QDebug>
#include"GuiManager.h"

DockModelTree::DockModelTree(MainWindow* par)
{	
	parent = par;
	dock_tabWidget=new QTabWidget(parent->dock);
	scroll = new QScrollArea();// 创建滚动类
	scroll->setMaximumWidth(299);
	dock_tabWidget->setLayoutDirection(Qt::LeftToRight);
	dock_tabWidget->setTabPosition(QTabWidget::North);
	part_tab = new QWidget(dock_tabWidget);
	part_tab->setObjectName(QStringLiteral("part_tab"));
	dock_tabWidget->addTab(part_tab, QString());
	assemble_tab = new QWidget(dock_tabWidget);
	assemble_tab->setObjectName(QStringLiteral("assemble_tab"));
	dock_tabWidget->setAttribute(Qt::WA_StyledBackground);
	dock_tabWidget->setIconSize(QSize(20, 20));
	dock_tabWidget->addTab(assemble_tab, QString());
	dock_tabWidget->setTabText(0, "");
	dock_tabWidget->setTabText(1, "");
	int height = parent->windown_height - parent->ribbonbar_height-parent->statusBar()->geometry().height() - 48;//dock_tabWidget height 
	int width = 500;//dock_tabWidget width
	dock_tabWidget->setGeometry(QRect(0,0, width, height));
	dock_tabWidget->setTabIcon(0, QPixmap(":/icon/icons/零件.png"));
	dock_tabWidget->setTabIcon(1, QPixmap(":/icon/icons/装配.png"));
	scroll->setWidget(dock_tabWidget);
	//dock_tabWidget->show();
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
	modeltree = new QTreeWidget(this->part_tab);
	int width = dock_tabWidget->width();
	int height = dock_tabWidget->height();
	modeltree->setGeometry(0, 0, dock_tabWidget->width(), dock_tabWidget->height());
	//qDebug() << ModelTree << modeltree;
	modeltree->expandAll();
	modeltree->setStyle(QStyleFactory::create("windows"));
	modeltree->setColumnCount(2);//设置列数
	modeltree->setHeaderLabels(QStringList() << "名称" << "附件" << "最新");//  设置树形控件头部的标题
	modeltree->setAlternatingRowColors(	true);
	modeltree->setColumnWidth(0,150);
	

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
	//QTabBar* tabBar0 = this->dock_tabWidget->findChild<QTabBar*>();//获取tavwidge的标题
	//QTabBar* tabBar1 = dock->findChild<QTabBar*>();//获取dock的标题
	//int dock_width = this->dock_tabWidget->geometry().width();
	//int dock_height = this->dock_tabWidget->geometry().height();
	//int dock_tabWidget_QTabBar_height= tabBar0->geometry().height();
	//int dock_QTabBar_height= tabBar1->geometry().height();
	//this->modeltree->setGeometry(0, 0, dock_width,dock_height-dock_tabWidget_QTabBar_height- dock_QTabBar_height);//重行设置模型树大小
	//qDebug()<< this->dock_tabWidget->geometry().height();
	//qDebug() << tabBar0->geometry().height();
	//qDebug() << tabBar1->geometry().height();
	//dock_tabWidget->setGeometry(0, 30, width, height)
	//part_tab->setObjectName(QStringLiteral("part_tab"));
	/*
	dock_tabWidget->addTab(part_tab, QString());
	assemble_tab->setObjectName(QStringLiteral("assemble_tab"));;
	dock_tabWidget->addTab(assemble_tab, QString());
	dock_tabWidget->setTabText(0, "");
	dock_tabWidget->setTabText(1, "");;
	dock_tabWidget->setAttribute(Qt::WA_StyledBackground);
	dock_tabWidget->setIconSize(QSize(20, 20));
	dock_tabWidget->addTab(assemble_tab, QString());
	dock_tabWidget->setTabText(0, "");
	dock_tabWidget->setTabText(1, "");
	int height = parent->dock->height();
	dock_tabWidget->setGeometry(QRect(0, 0, parent->dock->width(), parent->dock->height()));
	dock_tabWidget->setTabIcon(0, QPixmap(":/icon/icons/零件.png"));
	dock_tabWidget->setTabIcon(1, QPixmap(":/icon/icons/装配.png"));
	*/


}