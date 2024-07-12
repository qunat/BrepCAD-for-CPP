#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "SARibbonMainWindow.h"
#include <AIS_InteractiveContext.hxx>
#include "PartSolution.h"
#include <QDebug>
#include"WindownsManager.h"
#include<Qstring>
#include"display/DisplayCore.h"

class SARibbonCategory;
class SARibbonContextCategory;
class SARibbonCustomizeWidget;
class SARibbonActionsManager;
class SARibbonQuickAccessBar;
class SARibbonButtonGroupWidget;
class QTextEdit;
class DockModelTree;
class OccView;
class RibbonManeger;
class WindownsManager;

class MainWindow : public SARibbonMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* par = nullptr);
    QWidget* par = nullptr;

private:
    //初始化窗口 pannel 生成
    void createCategoryInitHome(SARibbonCategory* page);
    void createCategoryInitTool(SARibbonCategory* page);

    //新建零件串口 pannel 生成
    void createCategoryMain(SARibbonCategory* page);
    void createCategoryOther(SARibbonCategory* page);
    void createCategoryCurve(SARibbonCategory* page);//曲线面板
    void createCategoryDelete(SARibbonCategory* page);
    void createCategorySize(SARibbonCategory* page);
    void createContextCategory1();
    void createContextCategory2();
    void createContextCategoryPage1(SARibbonCategory* page);
    void createContextCategoryPage2(SARibbonCategory* page);
    void createQuickAccessBar(SARibbonQuickAccessBar* quickAccessBar);
    void createRightButtonGroup(SARibbonButtonGroupWidget* rightBar);
    void addSomeOtherAction();
    QAction* createAction(const QString& text, const QString& iconurl, const QString& objName);
    QAction* createAction(const QString& text, const QString& iconurl);
private slots:
    void onMenuButtonPopupCheckableTest(bool b);
    void onInstantPopupCheckableTest(bool b);
    void onDelayedPopupCheckableTest(bool b);
    void onMenuButtonPopupCheckabletriggered(bool b);
    void onInstantPopupCheckabletriggered(bool b);
    void onDelayedPopupCheckabletriggered(bool b);
    void onShowContextCategory(bool on);
    void onStyleClicked(int id);
    void onActionCustomizeTriggered(bool b);
    void onActionCustomizeAndSaveTriggered(bool b);
    void onActionHelpTriggered();
    void onActionRemoveAppBtnTriggered(bool b);
    void onActionUseQssTriggered();
    void onActionLoadCustomizeXmlFileTriggered();
    void onActionWindowFlagNormalButtonTriggered(bool b);

    void onFontComWidgetCurrentFontChanged(const QFont& f);
    void onActionFontLargerTriggered();
    void onActionFontSmallerTriggered();
    void onActionwordWrapIn2rowTriggered(bool b);
    void onButtonGroupActionTriggered(QAction* act);

private:

    SARibbonContextCategory* m_contextCategory;
    SARibbonContextCategory* m_contextCategory2;
    SARibbonCustomizeWidget* m_customizeWidget;
    QTextEdit* m_edit;
    SARibbonActionsManager* m_actMgr;
    int m_actionTagText;
    int m_actionTagWithIcon;
	//OccView* myOccView;
    RibbonManeger* myRibbonManeger;
    void  InitWindow();
    void  NewDocument();
    void  OpenDocument();
    void  CreateRibbon();
    SARibbonBar* ribbon;
    std::map<std::string, SARibbonCategory*>CategoryPageMap;
    //std::map<std::string, PartSolution*>formDoc;
    

public:
    void  UpdateGeometry();
    PartSolution* formDoc;
    QTabWidget* MultiWindowsTabWidget;
    WindownsManager* windownsmanager;
	DockModelTree * dockmodeltree;
	map <string, DockModelTree *>dockmodeltreemap;
    map <string, DisplayCore*>DisplayCoreManeger;
	int initializationsignal{0};//初始化界面完成信号
public slots:
    void mySlot();
};
 
#endif  // MAINWINDOW_H
