#include "GuiManege.h"
#include <QApplication>
#include <QFile>
#include <QTextEdit>
#include <QAbstractButton>
#include <QFileDialog>
#include "SARibbonBar.h"
#include "SARibbonCategory.h"
#include <QPushButton>
#include <QMessageBox>
#include "SARibbonPannel.h"
#include "SARibbonToolButton.h"
#include <QAction>
#include <QMenu>
#include <QStatusBar>
#include <QDebug>
#include <QElapsedTimer>
#include <QRadioButton>
#include <QButtonGroup>
#include "SARibbonMenu.h"
#include "SARibbonComboBox.h"
#include "SARibbonLineEdit.h"
#include "SARibbonGallery.h"
#include "SARibbonCheckBox.h"
#include "SARibbonQuickAccessBar.h"
#include "SARibbonButtonGroupWidget.h"
#include "SARibbonApplicationButton.h"
#include "SARibbonCustomizeWidget.h"
#include "SARibbonElementManager.h"
#include <QCalendarWidget>
#include "SARibbonCustomizeDialog.h"
#include <QXmlStreamWriter>
#include <QTextStream>
#include <QFontComboBox>
#include <QLabel>
#include "SAFramelessHelper.h"
#include <occView.h>
#include <display/DisplayCore.h>
#include <chrono>
#include <iostream>
#include"RibbonManeger.h"
#include"DockModelTree.h"
#include<QScreen>
#include<map>
#include <PartSolution.h>
#include <QDebug>
#include"WindownsManager.h"
#define PRINT_COST_START()                                                                                             \
    QElapsedTimer __TMP_COST;                                                                                          \
    __TMP_COST.start();                                                                                                \
    int __TMP_LASTTIMES = 0

#define PRINT_COST(STR)                                                                                                \
    do {                                                                                                               \
        int ___TMP_INT = __TMP_COST.elapsed();                                                                         \
        qDebug() << STR << " cost " << ___TMP_INT - __TMP_LASTTIMES << " ms (" << ___TMP_INT << ")";                   \
        m_edit->append(QString("%1 cost %2 ms(%3)").arg(STR).arg(___TMP_INT - __TMP_LASTTIMES).arg(___TMP_INT));       \
        __TMP_LASTTIMES = ___TMP_INT;                                                                                  \
    } while (0)

MainWindow::MainWindow(QWidget* par) : SARibbonMainWindow(par), m_customizeWidget(nullptr)
{
    PRINT_COST_START();
    par = par;
    SAFramelessHelper* helper = framelessHelper();
    helper->setRubberBandOnResize(false);
    setWindowTitle(("BrepCAD"));
    m_edit = new QTextEdit(this);
    setCentralWidget(m_edit);
    setStatusBar(new QStatusBar());

	//建立显示零件的窗口
	//myOccView = new OccView(this);
	//myOccView->reset();
    //myOccView->show();
    //建立左侧dock 模型树
    MultiWindowsTabWidget = new QTabWidget(this);
    myRibbonManeger = new RibbonManeger();
    windownsmanager = new WindownsManager(this);
    this->InitWindow();
    
   
    
}
void MainWindow::InitWindow()
{
    PRINT_COST_START();
    ribbon = ribbonBar();
    //通过setContentsMargins设置ribbon四周的间距
    ribbon->setContentsMargins(0.0, 0, 0.0, 0);
    //设置applicationButton
    PRINT_COST("setCentralWidget & setWindowTitle");
    ribbon->applicationButton()->setText(("文件"));
    QPushButton* button_document = qobject_cast<QPushButton*>(ribbon->applicationButton());  //向下转型为按钮类型

    QAction* new_document = new QAction("新建 N", this);
    QIcon new_document_icon(":/icon/icons/新建.png");
    new_document->setIcon(new_document_icon);
    new_document->setShortcut(QKeySequence(tr("Ctrl+N")));// 设置快捷键

    QAction* open_document = new QAction("打开 O", this);
    QIcon open_document_icon(":/icon/icons/打开.png");
    open_document->setIcon(open_document_icon);
    open_document->setShortcut(QKeySequence(tr("Ctrl+O")));// 设置快捷键

    QAction* close_document = new QAction("关闭 C", this);
    close_document->setShortcut(QKeySequence(tr("Ctrl+C")));// 设置快捷键

    QAction* separator = new QAction(" ", this);
    separator->setSeparator(true);

    QAction* save_document = new QAction("保存 S", this);
    QIcon save_document_icon(":/icon/icons/保存.png");
    save_document->setIcon(save_document_icon);
    save_document->setShortcut(QKeySequence(tr("Ctrl+S")));// 设置快捷键


    QMenu* home_menu = new QMenu;
    home_menu->addAction(new_document);
    home_menu->addAction(open_document);
    home_menu->addAction(close_document);
    home_menu->addAction(save_document);
    button_document->setMenu(home_menu);
  

    //添加主标签页 - 通过addCategoryPage工厂函数添加
    CategoryPageMap["Init_home_page"]= ribbon->addCategoryPage(tr("主页"));
    CategoryPageMap["Init_home_page"]->setObjectName(("categoryMain"));
    createCategoryInitHome(CategoryPageMap["Init_home_page"]);
    PRINT_COST("new main page");

    
    //添加其他标签页 - 直接new SARibbonCategory添加
    SARibbonCategory* categoryTool = new SARibbonCategory();
    CategoryPageMap["categorycategoryTool"]= categoryTool;
    CategoryPageMap["categorycategoryTool"]->setCategoryName(tr("工具"));
    CategoryPageMap["categorycategoryTool"]->setObjectName(("categoryOther"));
    createCategoryCurve(CategoryPageMap["categorycategoryTool"]);
    ribbon->addCategoryPage(CategoryPageMap["categorycategoryTool"]);
    PRINT_COST("add other page");

    //交互界面初始化
    QWidget* interactionwidget = new QWidget();
    interactionwidget->setStyleSheet("background-color: rgb(237, 237, 237);");
    QLabel* soft_name_Label = new QLabel(interactionwidget);
    QLabel* soft_version_Label = new QLabel(interactionwidget);

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect mm = screen->availableGeometry();
    int screen_width = mm.width();
    int screen_height = mm.height();

    //初始化界面文字 软件名称
    soft_name_Label->setText("BrepCAD");
    soft_version_Label->setText("2023");
    soft_name_Label->setGeometry(interactionwidget->size().width()/2+150, interactionwidget->size().height()/2+90, 1500, 130);
    soft_version_Label->setGeometry(interactionwidget->size().width()+650, interactionwidget->size().height()/2+240, 1500, 90);
    QFont ft;
    ft.setPointSize(120);
    soft_name_Label->setStyleSheet("color: rgb(134, 136, 139);");
    soft_version_Label->setStyleSheet("color: rgb(134, 136, 139);");
    soft_name_Label->setFont(ft);
    ft.setPointSize(40);
    soft_version_Label->setFont(ft);

    setCentralWidget(interactionwidget);
    //createContextCategory1();

    createContextCategory2();

    //建立快速访问工具栏
    SARibbonQuickAccessBar* quickAccessBar = ribbon->quickAccessBar();
    createQuickAccessBar(quickAccessBar);
    SARibbonButtonGroupWidget* rightBar = ribbon->rightButtonGroup();
    createRightButtonGroup(rightBar);
 

    addSomeOtherAction();
    setMinimumWidth(500);
    showMaximized();
    setWindowIcon(QIcon(":/icon/icon/SA.svg"));
    connect(new_document, &QAction::triggered, this, &MainWindow::NewDocument);
    int asd=ribbon->geometry().width();
}
void MainWindow::NewDocument()
{

    //移除初始化界面的panel
    ribbon->removeCategory(CategoryPageMap["Init_home_page"]);
    ribbon->removeCategory(CategoryPageMap["categorycategoryTool"]);
    //生成多窗口的QTabWidget
    
    
    windownsmanager->CreateNewWindown();
    windownsmanager->windowns_name[windownsmanager->current_windown]->setAttribute(Qt::WA_DeleteOnClose);
    //formDoc = new PartSolution(this); //不指定父窗口，单独用show()方法显示
    //PartSolution* formDoc1 = new PartSolution(this); //不指定父窗口，单独用show()方法显示
    //PartSolution* formDoc2 = new PartSolution(this); //不指定父窗口，单独用show()方法显示
    //formDoc->setAttribute(Qt::WA_DeleteOnClose); //关闭时自动删除
    MultiWindowsTabWidget->setTabsClosable(true);
    // 建立显示零件的窗口
    // myOccView = new OccView(this);
    // myOccView->reset();
    // myOccView->show();
    setCentralWidget(MultiWindowsTabWidget);

    QString winwownname = QString::fromStdString(windownsmanager->current_windown);
    MultiWindowsTabWidget->addTab(windownsmanager->windowns_name[windownsmanager->current_windown], winwownname);
    MultiWindowsTabWidget->setCurrentIndex(1);

    QFile  qss(".\\Qss\\docktawidget.qss");

    if (qss.open(QFile::ReadOnly))

    {
        QString style = QLatin1String(qss.readAll());
        MultiWindowsTabWidget->setStyleSheet(style);
        qss.close();
    }


    //通过setContentsMargins设置ribbon四周的间距
    ribbon->setContentsMargins(0.0, 0, 0.0, 0);
 
    //添加主标签页 - 通过addCategoryPage工厂函数添加
    SARibbonCategory* categoryMain = ribbon->addCategoryPage(tr("主页"));
    CategoryPageMap["categoryMain"] = categoryMain;
    CategoryPageMap["categoryMain"]->setObjectName(("categoryMain"));
    createCategoryMain(CategoryPageMap["categoryMain"]);


    //添加其他标签页 - 直接new SARibbonCategory添加
    SARibbonCategory* categoryCurve = new SARibbonCategory();
    CategoryPageMap["categoryCurve"] = categoryCurve;
    CategoryPageMap["categoryCurve"]->setCategoryName(tr("曲线"));
    CategoryPageMap["categoryCurve"]->setObjectName(("categoryOther"));
    createCategoryCurve(CategoryPageMap["categoryCurve"]);
    ribbon->addCategoryPage(CategoryPageMap["categoryCurve"]);
 

    //添加其他标签页 - 直接new SARibbonCategory添加
    SARibbonCategory* categorySurface = new SARibbonCategory();
    CategoryPageMap["categorySurface"] = categorySurface;
    CategoryPageMap["categorySurface"]->setCategoryName(("曲面"));
    CategoryPageMap["categorySurface"]->setObjectName(("categorySurface"));
    ribbon->addCategoryPage(CategoryPageMap["categoryDelete"]);
    createCategoryDelete(CategoryPageMap["categorySurface"]);
    
    //添加其他标签页 - 直接new SARibbonCategory添加
    SARibbonCategory* categoryAssembly = new SARibbonCategory();
    CategoryPageMap["categoryAssembly"] = categoryAssembly;
    CategoryPageMap["categoryAssembly"]->setCategoryName(("装配"));
    CategoryPageMap["categoryAssembly"]->setObjectName(("categoryDelete"));
    ribbon->addCategoryPage(CategoryPageMap["categoryAssembly"]);
    createCategoryDelete(CategoryPageMap["categoryAssembly"]);

    //添加其他标签页 - 直接new SARibbonCategory添加
    SARibbonCategory* categoryAnylyse = new SARibbonCategory();
    CategoryPageMap["categoryAnylyse"] = categoryAnylyse;
    CategoryPageMap["categoryAnylyse"]->setCategoryName(("分析"));
    CategoryPageMap["categoryAnylyse"]->setObjectName(("categoryDelete"));
    ribbon->addCategoryPage(CategoryPageMap["categoryAnylyse"]);
    createCategoryDelete(CategoryPageMap["categoryAnylyse"]);

    //添加其他标签页 - 直接new SARibbonCategory添加
    SARibbonCategory* categoryViewer = new SARibbonCategory();
    CategoryPageMap["categoryViewer"] = categoryViewer;
    CategoryPageMap["categoryViewer"]->setCategoryName(("视图"));
    CategoryPageMap["categoryViewer"]->setObjectName(("categoryDelete"));
    ribbon->addCategoryPage(CategoryPageMap["categoryViewer"]);
    createCategoryDelete(CategoryPageMap["categoryViewer"]);

    //添加其他标签页 - 直接new SARibbonCategory添加
    SARibbonCategory* categorySelect = new SARibbonCategory();
    CategoryPageMap["categorySelect"] = categorySelect;
    CategoryPageMap["categorySelect"]->setCategoryName(("选择"));
    CategoryPageMap["categorySelect"]->setObjectName(("categoryDelete"));
    ribbon->addCategoryPage(CategoryPageMap["categorySelect"]);
    createCategoryDelete(CategoryPageMap["categorySelect"]);

    //添加其他标签页 - 直接new SARibbonCategory添加
    SARibbonCategory* categoryRender = new SARibbonCategory();
    CategoryPageMap["categoryRender"] = categoryRender;
    CategoryPageMap["categoryRender"]->setCategoryName(("渲染"));
    CategoryPageMap["categoryRender"]->setObjectName(("categoryDelete"));
    ribbon->addCategoryPage(CategoryPageMap["categoryRender"]);
    createCategoryDelete(CategoryPageMap["categoryRender"]);


    //添加其他标签页 - 直接new SARibbonCategory添加
    SARibbonCategory* categoryTools = new SARibbonCategory();
    CategoryPageMap["categoryTools"] = categoryTools;
    CategoryPageMap["categoryTools"]->setCategoryName(("工具"));
    CategoryPageMap["categoryTools"]->setObjectName(("categoryDelete"));
    ribbon->addCategoryPage(CategoryPageMap["categoryTools"]);
    createCategoryDelete(CategoryPageMap["categoryTools"]);


    //添加其他标签页 - 直接new SARibbonCategory添加
    SARibbonCategory* categoryApplication = new SARibbonCategory();
    CategoryPageMap["categoryApplication"] = categoryApplication;
    CategoryPageMap["categoryApplication"]->setCategoryName(("应用模块"));
    CategoryPageMap["categoryApplication"]->setObjectName(("categoryDelete"));
    ribbon->addCategoryPage(CategoryPageMap["categoryApplication"]);
    createCategoryDelete(CategoryPageMap["categoryApplication"]);


    //先将标签先换到2 不然切换到1时 不会刷新界面
    ribbon->setCurrentIndex(2);
   // Display_Core* mydisplaycore = new Display_Core(formDoc->myOccView);
    mydockmodeltree = new DockModelTree(par);
    addDockWidget(Qt::LeftDockWidgetArea, mydockmodeltree->dock);
    ribbon->setCurrentIndex(0);
    ribbon->updateRibbonGeometry();
    this -> mydockmodeltree->UpdateGeometry();
    //connect(formDoc->myOccView, SIGNAL(mySignal(int)), this, SLOT(mySlot()));
}
void  MainWindow::mySlot()
{
    qDebug() << "The value is:";
}
void MainWindow::UpdateGeometry()
{
    this->mydockmodeltree->UpdateGeometry();
}
void MainWindow::onShowContextCategory(bool on)
{
    if (m_contextCategory == nullptr) {
        createContextCategory1();
    }
    if (on) {
        this->ribbonBar()->showContextCategory(m_contextCategory);
    } else {
        this->ribbonBar()->hideContextCategory(m_contextCategory);
    }
}

void MainWindow::onStyleClicked(int id)
{
    ribbonBar()->setRibbonStyle(static_cast< SARibbonBar::RibbonStyle >(id));
}

void MainWindow::onActionCustomizeTriggered(bool b)
{
    Q_UNUSED(b);
    if (nullptr == m_customizeWidget) {
        m_customizeWidget = new SARibbonCustomizeWidget(this, this, Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint | Qt::Dialog);
        m_customizeWidget->setWindowModality(Qt::ApplicationModal);  //设置阻塞类型
        m_customizeWidget->setAttribute(Qt::WA_ShowModal, true);     //属性设置 true:模态 false:非模态
        m_customizeWidget->setupActionsManager(m_actMgr);
    }
    m_customizeWidget->show();
    m_customizeWidget->applys();
}

void MainWindow::onActionCustomizeAndSaveTriggered(bool b)
{
    Q_UNUSED(b);
    SARibbonCustomizeDialog dlg(this);
    dlg.setupActionsManager(m_actMgr);
    dlg.fromXml("customize.xml");
    if (SARibbonCustomizeDialog::Accepted == dlg.exec()) {
        dlg.applys();
        QByteArray str;
        QXmlStreamWriter xml(&str);
        xml.setAutoFormatting(true);
        xml.setAutoFormattingIndent(2);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)  // QXmlStreamWriter always encodes XML in UTF-8.
        xml.setCodec("utf-8");
#endif
        xml.writeStartDocument();
        bool isok = dlg.toXml(&xml);
        xml.writeEndDocument();
        if (isok) {
            QFile f("customize.xml");
            if (f.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)) {
                QTextStream s(&f);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)  // QTextStream always encodes XML in UTF-8.
                s.setCodec("utf-8");
#endif
                s << str;
                s.flush();
            }
            m_edit->append("write xml:");
            m_edit->append(str);
        }
    }
}

void MainWindow::onActionHelpTriggered()
{
    QMessageBox::information(this,
                             tr("infomation"),
                             tr("\n ==============="
                                "\n SARibbonBar version:%1"
                                "\n Author:czy"
                                "\n Email:czy.t@163.com"
                                "\n ===============")
                             .arg(SARibbonBar::versionString()));

    this->mydockmodeltree->UpdateGeometry();
   
}

void MainWindow::onActionRemoveAppBtnTriggered(bool b)
{
    if (b) {
        ribbonBar()->setApplicationButton(nullptr);
    } else {
        SARibbonApplicationButton* actionRemoveAppBtn = new SARibbonApplicationButton();
        actionRemoveAppBtn->setText(tr("文件"));
        this->ribbonBar()->setApplicationButton(actionRemoveAppBtn);

    }
}

void MainWindow::onActionUseQssTriggered()
{
    QFile f("ribbon.qss");
    if (!f.exists()) {
        QString fdir = QFileDialog::getOpenFileName(this, tr("select qss file"));
        if (fdir.isEmpty()) {
            return;
        }
        f.setFileName(fdir);
    }
    if (!f.open(QIODevice::ReadWrite)) {
        return;
    }
    QString qss(f.readAll());
    m_edit->setText(qss);
    this->ribbonBar()->setStyleSheet(qss);
}

void MainWindow::onActionLoadCustomizeXmlFileTriggered()
{
    //只能调用一次
    static bool has_call = false;
    if (!has_call) {
        has_call = sa_apply_customize_from_xml_file("customize.xml", this, m_actMgr);
    }
}

void MainWindow::onActionWindowFlagNormalButtonTriggered(bool b)
{
    if (b) {
        //最大最小关闭按钮都有
        Qt::WindowFlags f = windowFlags();
        f |= (Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
        updateWindowFlag(f);
    } else {
        //由于已经处于frameless状态，这个最大最小设置是无效的
        // setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint&~Qt::WindowMinimizeButtonHint);
        Qt::WindowFlags f = windowFlags();
        f &= ~Qt::WindowMinMaxButtonsHint & ~Qt::WindowCloseButtonHint;
        updateWindowFlag(f);
    }
}

void MainWindow::onFontComWidgetCurrentFontChanged(const QFont& f)
{
    qDebug() << "set font:" << f;
    ribbonBar()->setFont(f);
    update();
}

void MainWindow::onActionFontLargerTriggered()
{
    QFont f = font();
    f.setPointSize(f.pointSize() + 1);
    ribbonBar()->setFont(f);
    update();
    qDebug() << "set font:" << f;
}

void MainWindow::onActionFontSmallerTriggered()
{
    QFont f = font();
    f.setPointSize(f.pointSize() - 1);
    ribbonBar()->setFont(f);
    update();
    qDebug() << "set font:" << f;
}

void MainWindow::onActionwordWrapIn2rowTriggered(bool b)
{
    SARibbonToolButton::setLiteStyleEnableWordWrap(b);  //设置是否允许2行模式下文字换行，换行的话图标会较小
    //换行设定后需要重新计算样式尺寸
    RibbonSubElementStyleOpt.recalc();
    //通过setRibbonStyle来让ribbonbar重绘
    //由于关键尺寸变化了，需要重新布局
    ribbonBar()->updateRibbonGeometry();
}

/**
 * @brief 测试SARibbonButtonGroupWidget和标题对齐
 * @param act
 */
void MainWindow::onButtonGroupActionTriggered(QAction* act)
{
    QVariant v = act->property("align");
    if (v.isValid()) {
        Qt::Alignment al = static_cast< Qt::Alignment >(v.toInt());
        if (!ribbonBar()) {
            return;
        }
        ribbonBar()->setWindowTitleAligment(al);
        ribbonBar()->repaint();
    }
}
void MainWindow::createCategoryInitHome(SARibbonCategory* page)
{

    //使用addPannel函数来创建SARibbonPannel，效果和new SARibbonPannel再addPannel一样
    SARibbonPannel* pannelStructure = page->addPannel(("标准"));

    QAction* new_document = createAction(tr("新建"), ":/icon/icons/新建.png");
    new_document->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    pannelStructure->addLargeAction(new_document);
    connect(new_document, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });

    QAction* open_document = createAction(tr("打开"), ":/icon/icons/打开.png");
    open_document->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    pannelStructure->addLargeAction(open_document);
    connect(open_document, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });

    


}
void MainWindow::createCategoryMain(SARibbonCategory* page)
{
    //! 1主页
    //! pannel 1 start
    //!

    //使用addPannel函数来创建SARibbonPannel，效果和new SARibbonPannel再addPannel一样
    SARibbonPannel* pannelStructure = page->addPannel(("构造"));

    QAction* datum_plane = createAction(tr("基准平面"), ":/icon/icons/datumplane.png");
    datum_plane->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    pannelStructure->addLargeAction(datum_plane);
    connect(datum_plane, &QAction::triggered, this, [ this ](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });

    QAction* sketch = createAction(tr("草图"), ":/icon/icons/草绘(1).png");
    sketch->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    pannelStructure->addLargeAction(sketch);
    connect(sketch, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });


    SARibbonPannel* pannelBase = page->addPannel(("基本"));

    QAction* Prism = createAction(tr("拉伸"), ":/icon/icons/拉伸(1).png");
    Prism->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    pannelBase->addLargeAction(Prism);
    connect(Prism, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });

    QAction* rotate = createAction(tr("旋转"), ":/icon/icons/旋转(1).png");
    rotate->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    pannelBase->addLargeAction(rotate);
    connect(rotate, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });

    QAction* Hole = createAction(tr("孔"), ":/icon/icons/孔(1).png");
    Hole->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    pannelBase->addLargeAction(Hole);
    connect(Hole, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });

    QAction* Merge = createAction(tr("合并"), ":/icon/icons/布尔求和(1).png");
    Merge->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    pannelBase->addLargeAction(Merge);
    connect(Merge, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });

    QAction* Subtraction = createAction(tr("减去"), ":/icon/icons/布尔减(1).png");
    Subtraction->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    pannelBase->addLargeAction(Subtraction);
    connect(Subtraction, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });

    QAction* Trim_body = createAction(tr("修剪体"), ":/icon/icons/修剪体(1).png");
    Trim_body->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    pannelBase->addLargeAction(Trim_body);
    connect(Trim_body, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });

    QAction* Round = createAction(tr("边倒圆"), ":/icon/icons/倒圆角(1).png");
    Round->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    pannelBase->addLargeAction(Round);
    connect(Round, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });
    
    QAction* Draft = createAction(tr("拔模"), ":/icon/icons/拔模(1).png", "actHideRibbon");
    Draft->setCheckable(false);
    pannelBase->addMediumAction(Draft);
    connect(Draft, &QAction::triggered, this, [ this ](bool b) { this->ribbonBar()->setMinimumMode(b); });
    connect(ribbonBar(), &SARibbonBar::ribbonModeChanged, this, [Draft](SARibbonBar::RibbonMode nowNode) {
        Draft->setChecked(nowNode == SARibbonBar::MinimumRibbonMode);
    });

    QAction* Shell = createAction(tr("抽壳"), ":/icon/icons/抽壳(1).png", "show hide button");
    Shell->setCheckable(false);
    pannelBase->addMediumAction(Shell);
    connect(Shell, &QAction::triggered, this, [ this ](bool b) {
  
    });
    Shell->trigger();

    QAction* Array_features = createAction(tr("整列特征"), ":/icon/icons/阵列(1).png", "show hide button");
    Array_features->setCheckable(false);
    pannelBase->addMediumAction(Array_features);
    connect(Array_features, &QAction::triggered, this, [this](bool b) {
        this->ribbonBar()->showMinimumModeButton(b);  //显示ribbon最小化按钮
    });
    Array_features->trigger();

    QAction* Mirror_feature = createAction(tr("镜像特征"), ":/icon/icons/镜像(1).png", "show hide button");
    Mirror_feature->setCheckable(false);
    pannelBase->addMediumAction(Mirror_feature);
    connect(Mirror_feature, &QAction::triggered, this, [this](bool b) {
        this->ribbonBar()->showMinimumModeButton(b);  //显示ribbon最小化按钮
    });
    Mirror_feature->trigger();

    //pannelBase->addSeparator();
    QAction* act;
    SARibbonToolButton* btn;
    SARibbonMenu* menu = new SARibbonMenu(this);

    act = createAction(tr("用户自定义"), ":/icon/icons/用户自定义.png");
    act->setMenu(menu);
    btn = pannelBase->addLargeAction(act);
    btn->setPopupMode(QToolButton::DelayedPopup);
    connect(act, &QAction::triggered, this, &MainWindow::onDelayedPopupCheckabletriggered);


    QAction* optAct = new QAction(this);
    connect(optAct, &QAction::triggered, this, [this](bool on) {
        Q_UNUSED(on);
        QMessageBox::information(this, tr("Option Action Triggered"), tr("Option Action Triggered"));
    });
    pannelBase->addOptionAction(optAct);



    SARibbonPannel* Synchronous_modeling = page->addPannel(("同步建模"));

    QAction* Move_face = createAction(tr("移动面"), ":/icon/icons/移动面.png");
    Move_face->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    Synchronous_modeling->addLargeAction(Move_face);
    connect(Move_face, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });

    QAction* Delete_face = createAction(tr("删除面"), ":/icon/icons/删除.png");
    Delete_face->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    Synchronous_modeling->addLargeAction(Delete_face);
    connect(Delete_face, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });

    QAction* Replace_face = createAction(tr("替换面"), ":/icon/icons/替换.png");
    Replace_face->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    Synchronous_modeling->addLargeAction(Replace_face);
    connect(Replace_face, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });

    QAction* Offset_face = createAction(tr("偏置面"), ":/icon/icons/偏置面.png", "actHideRibbon");
    Offset_face->setCheckable(false);
    Synchronous_modeling->addMediumAction(Offset_face);
    connect(Offset_face, &QAction::triggered, this, [this](bool b) { this->ribbonBar()->setMinimumMode(b); });
    connect(ribbonBar(), &SARibbonBar::ribbonModeChanged, this, [Offset_face](SARibbonBar::RibbonMode nowNode) {
        Offset_face->setChecked(nowNode == SARibbonBar::MinimumRibbonMode);
    });

    QAction* Resize_blend = createAction(tr("调整倒圆角大小"), ":/icon/icons/调整倒圆角大小.png", "actHideRibbon");
    Resize_blend->setCheckable(false);
    Synchronous_modeling->addMediumAction(Resize_blend);
    connect(Resize_blend, &QAction::triggered, this, [this](bool b) { this->ribbonBar()->setMinimumMode(b); });
    connect(ribbonBar(), &SARibbonBar::ribbonModeChanged, this, [Resize_blend](SARibbonBar::RibbonMode nowNode) {
        Resize_blend->setChecked(nowNode == SARibbonBar::MinimumRibbonMode);
    });

    act = createAction(tr("更多"), ":/icon/icons/用户自定义.png");
    act->setMenu(menu);
    btn = Synchronous_modeling->addLargeAction(act);
    btn->setPopupMode(QToolButton::DelayedPopup);
    connect(act, &QAction::triggered, this, &MainWindow::onDelayedPopupCheckabletriggered);

    
    /*
    QAction* actwordWrapIn2row = createAction(tr("word wrap in 2row"), ":/icon/icon/wordwrap.svg");
    actwordWrapIn2row->setCheckable(true);
    pannelBase->addSmallAction(actwordWrapIn2row);
    connect(actwordWrapIn2row, &QAction::triggered, this, &MainWindow::onActionwordWrapIn2rowTriggered);

    QButtonGroup* g = new QButtonGroup(page);

    QRadioButton* r = new QRadioButton();
    r->setText(tr("use office style"));
    r->setObjectName(("use office style"));
    r->setChecked(true);
    pannelBase->addSmallWidget(r);
    g->addButton(r, SARibbonBar::OfficeStyle);

    r = new QRadioButton();
    r->setObjectName(("use wps style"));
    r->setText(tr("use wps style"));
    r->setChecked(false);
    pannelBase->addSmallWidget(r);
    g->addButton(r, SARibbonBar::WpsLiteStyle);

    r = new QRadioButton();
    r->setObjectName(("use office 2row style"));
    r->setText(tr("use office 2 row style"));
    r->setChecked(false);
    pannelBase->addSmallWidget(r);
    g->addButton(r, SARibbonBar::OfficeStyleTwoRow);

    r = new QRadioButton();
    r->setObjectName(("use wps 2row style"));
    r->setText(tr("use wps 2row style"));
    r->setChecked(false);
    pannelBase->addSmallWidget(r);
    g->addButton(r, SARibbonBar::WpsLiteStyleTwoRow);
   

//    connect(g, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &MainWindow::onStyleClicked);
   

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    connect(g, static_cast< void (QButtonGroup::*)(int) >(&QButtonGroup::idClicked), this, &MainWindow::onStyleClicked);
#else
    connect(g, static_cast< void (QButtonGroup::*)(int) >(&QButtonGroup::buttonClicked), this, &MainWindow::onStyleClicked);
#endif
 */
    /*
    SARibbonPannel* pannelToolButtonStyle = page->addPannel(("sa ribbon toolbutton style"));

    SARibbonToolButton* btn;
    SARibbonMenu* menu = new SARibbonMenu(this);
    QAction* a         = nullptr;
    {
        QIcon itemicon = QIcon(":/icon/icon/item.svg");
        for (int i = 0; i < 5; ++i) {
            a = menu->addAction(itemicon, tr("item %1").arg(i + 1));
            a->setObjectName(QStringLiteral("item %1").arg(i + 1));
        }
    }

    QAction* act = createAction(tr("test 1"), ":/icon/icon/test1.svg");
    act->setMenu(menu);
    act->setToolTip(tr("use QToolButton::MenuButtonPopup mode"));
    btn = pannelToolButtonStyle->addSmallAction(act);
    btn->setPopupMode(QToolButton::MenuButtonPopup);

    act = createAction(tr("test 2"), ":/icon/icon/test2.svg");
    act->setMenu(menu);
    act->setToolTip(tr("use QToolButton::InstantPopup mode"));
    btn = pannelToolButtonStyle->addSmallAction(act);
    btn->setPopupMode(QToolButton::InstantPopup);

    pannelToolButtonStyle->addSeparator();

    act = createAction(tr("Delayed Popup"), ":/icon/icon/folder-cog.svg");
    act->setMenu(menu);
    btn = pannelToolButtonStyle->addLargeAction(act);
    btn->setPopupMode(QToolButton::DelayedPopup);
    connect(act, &QAction::triggered, this, &MainWindow::onDelayedPopupCheckabletriggered);

    act = createAction(tr("Menu Button Popup"), ":/icon/icon/folder-star.svg");
    act->setMenu(menu);
    btn = pannelToolButtonStyle->addLargeAction(act);
    btn->setPopupMode(QToolButton::MenuButtonPopup);
    connect(act, &QAction::triggered, this, &MainWindow::onMenuButtonPopupCheckabletriggered);

    act = createAction(tr("Instant Popup"), ":/icon/icon/folder-stats.svg");
    act->setMenu(menu);
    btn = pannelToolButtonStyle->addLargeAction(act);
    btn->setPopupMode(QToolButton::InstantPopup);
    connect(act, &QAction::triggered, this, &MainWindow::onInstantPopupCheckabletriggered);

    act = createAction(tr("Delayed Popup checkable"), ":/icon/icon/folder-table.svg");
    act->setCheckable(true);
    act->setMenu(menu);
    btn = pannelToolButtonStyle->addLargeAction(act);
    btn->setPopupMode(QToolButton::DelayedPopup);
    connect(act, &QAction::triggered, this, &MainWindow::onDelayedPopupCheckableTest);

    act = createAction(tr("Menu Button Popup checkable"), ":/icon/icon/folder-checkmark.svg");
    act->setCheckable(true);
    act->setMenu(menu);
    btn = pannelToolButtonStyle->addLargeAction(act);
    btn->setPopupMode(QToolButton::MenuButtonPopup);
    connect(act, &QAction::triggered, this, &MainWindow::onMenuButtonPopupCheckableTest);

    act = createAction(tr("disable action"), ":/icon/icon/disable.svg");
    act->setCheckable(true);
    act->setMenu(menu);
    act->setEnabled(false);
    pannelToolButtonStyle->addLargeAction(act);

    QAction* optAct = new QAction(this);
    connect(optAct, &QAction::triggered, this, [ this ](bool on) {
        Q_UNUSED(on);
        QMessageBox::information(this, tr("Option Action Triggered"), tr("Option Action Triggered"));
    });
    pannelToolButtonStyle->addOptionAction(optAct);
    //! 2
    //! pannel 2 start
    //!
    SARibbonPannel* pannel2 = page->addPannel(("pannel 2"));

    QAction* actShowContext = createAction(tr("show Context"), ":/icon/icon/showContext.svg");
    actShowContext->setCheckable(true);
    pannel2->addLargeAction(actShowContext);
    connect(actShowContext, &QAction::triggered, this, &MainWindow::onShowContextCategory);

    QAction* actDeleteContext = createAction(tr("delete Context"), ":/icon/icon/deleteContext.svg");
    pannel2->addLargeAction(actDeleteContext);
    connect(actDeleteContext, &QAction::triggered, this, [ this, act ](bool on) {
        Q_UNUSED(on);
        if (this->m_contextCategory) {
            this->ribbonBar()->destroyContextCategory(this->m_contextCategory);
            this->m_contextCategory = nullptr;
            act->setDisabled(true);
        }
    });
  
    //! 3
    //! pannel 3 start -> widget test
    //!

    SARibbonPannel* pannelWidgetTest = page->addPannel(tr("widget test"));
    pannelWidgetTest->setObjectName(QStringLiteral(u"pannelWidgetTest"));

    SARibbonComboBox* com = new SARibbonComboBox(this);

    com->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    com->setWindowTitle(tr("ComboBox"));
    for (int i = 0; i < 40; ++i) {
        com->addItem(QString("SARibbonComboBox test%1").arg(i + 1));
    }
    com->setEditable(true);
    pannelWidgetTest->addSmallWidget(com);

    com = new SARibbonComboBox(this);
    com->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    com->setWindowTitle("ComboBox Editable");
    for (int i = 0; i < 40; ++i) {
        com->addItem(QString("item %1").arg(i + 1));
    }
    pannelWidgetTest->addSmallWidget(com);

    SARibbonLineEdit* lineEdit = new SARibbonLineEdit(this);

    lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    lineEdit->setWindowTitle("Line Edit");
    lineEdit->setText("SARibbonLineEdit");
    pannelWidgetTest->addSmallWidget(lineEdit);
    QWidget* w = lineEdit->parentWidget();

    while (w) {
        qDebug() << w->metaObject()->className();
        w = w->parentWidget();
    }
    SARibbonCheckBox* checkBox = new SARibbonCheckBox(this);

    checkBox->setText(tr("checkBox"));
    pannelWidgetTest->addSmallWidget(checkBox);
    pannelWidgetTest->addSeparator();

    QCalendarWidget* calendarWidget = new QCalendarWidget(this);
    calendarWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    calendarWidget->setObjectName(("calendarWidget"));
    pannelWidgetTest->addLargeWidget(calendarWidget);
    pannelWidgetTest->setExpanding();
    optAct = new QAction(this);
    connect(optAct, &QAction::triggered, this, [ this ](bool on) {
        Q_UNUSED(on);
        QMessageBox::information(this, tr("Option Action Triggered"), tr("Option Action Triggered"));
    });
    pannelWidgetTest->addOptionAction(optAct);

    pannelWidgetTest->setVisible(true);
      */
}
void MainWindow::createCategoryCurve(SARibbonCategory* page)
{
    //! 曲线
   //! pannel 1 start
   //!

   //使用addPannel函数来创建SARibbonPannel，效果和new SARibbonPannel再addPannel一样
    SARibbonPannel* pannelStructure = page->addPannel(("构造"));

    QAction* datum_plane = createAction(tr("基准平面"), ":/icon/icons/datumplane.png");
    datum_plane->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    pannelStructure->addLargeAction(datum_plane);
    connect(datum_plane, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });

    QAction* sketch = createAction(tr("草图"), ":/icon/icons/草绘(1).png");
    sketch->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    pannelStructure->addLargeAction(sketch);
    connect(sketch, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });


    SARibbonPannel* pannelBase = page->addPannel(("基本"));

    QAction* Point = createAction(tr("点"), ":/icon/icons/点(1).png", "actHideRibbon");
    Point->setCheckable(false);
    pannelBase->addSmallAction(Point);
    connect(Point, &QAction::triggered, this, [this](bool b) { this->ribbonBar()->setMinimumMode(b); });
    connect(ribbonBar(), &SARibbonBar::ribbonModeChanged, this, [Point](SARibbonBar::RibbonMode nowNode) {
        Point->setChecked(nowNode == SARibbonBar::MinimumRibbonMode);
    });

    QAction* Line = createAction(tr("线"), ":/icon/icons/线(1).png", "actHideRibbon");
    Line->setCheckable(false);
    pannelBase->addSmallAction(Line);
    connect(Line, &QAction::triggered, this, [this](bool b) { this->ribbonBar()->setMinimumMode(b); });
    connect(ribbonBar(), &SARibbonBar::ribbonModeChanged, this, [Line](SARibbonBar::RibbonMode nowNode) {
        Line->setChecked(nowNode == SARibbonBar::MinimumRibbonMode);
    });


    QAction* Arc = createAction(tr("圆弧/圆"), ":/icon/icons/圆弧(1).png", "actHideRibbon");
    Arc->setCheckable(false);
    pannelBase->addSmallAction(Arc);
    connect(Arc, &QAction::triggered, this, [this](bool b) { this->ribbonBar()->setMinimumMode(b); });
    connect(ribbonBar(), &SARibbonBar::ribbonModeChanged, this, [Arc](SARibbonBar::RibbonMode nowNode) {
        Arc->setChecked(nowNode == SARibbonBar::MinimumRibbonMode);
    });

    QAction* Prism = createAction(tr("拉伸"), ":/icon/icons/拉伸(1).png");
    Prism->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    pannelBase->addLargeAction(Prism);
    connect(Prism, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });

    QAction* rotate = createAction(tr("旋转"), ":/icon/icons/旋转(1).png");
    rotate->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    pannelBase->addLargeAction(rotate);
    connect(rotate, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });

    QAction* Hole = createAction(tr("孔"), ":/icon/icons/孔(1).png");
    Hole->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    pannelBase->addLargeAction(Hole);
    connect(Hole, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });

    QAction* Merge = createAction(tr("合并"), ":/icon/icons/布尔求和(1).png");
    Merge->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    pannelBase->addLargeAction(Merge);
    connect(Merge, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });

    QAction* Subtraction = createAction(tr("减去"), ":/icon/icons/布尔减(1).png");
    Subtraction->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    pannelBase->addLargeAction(Subtraction);
    connect(Subtraction, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });

    QAction* Trim_body = createAction(tr("修剪体"), ":/icon/icons/修剪体(1).png");
    Trim_body->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    pannelBase->addLargeAction(Trim_body);
    connect(Trim_body, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });

    QAction* Round = createAction(tr("边倒圆"), ":/icon/icons/倒圆角(1).png");
    Round->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    pannelBase->addLargeAction(Round);
    connect(Round, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });

    QAction* Draft = createAction(tr("拔模"), ":/icon/icons/拔模(1).png", "actHideRibbon");
    Draft->setCheckable(false);
    pannelBase->addMediumAction(Draft);
    connect(Draft, &QAction::triggered, this, [this](bool b) { this->ribbonBar()->setMinimumMode(b); });
    connect(ribbonBar(), &SARibbonBar::ribbonModeChanged, this, [Draft](SARibbonBar::RibbonMode nowNode) {
        Draft->setChecked(nowNode == SARibbonBar::MinimumRibbonMode);
    });

    QAction* Shell = createAction(tr("抽壳"), ":/icon/icons/抽壳(1).png", "show hide button");
    Shell->setCheckable(false);
    pannelBase->addMediumAction(Shell);
    connect(Shell, &QAction::triggered, this, [this](bool b) {

    });
    Shell->trigger();

    QAction* Array_features = createAction(tr("整列特征"), ":/icon/icons/阵列(1).png", "show hide button");
    Array_features->setCheckable(false);
    pannelBase->addMediumAction(Array_features);
    connect(Array_features, &QAction::triggered, this, [this](bool b) {
        this->ribbonBar()->showMinimumModeButton(b);  //显示ribbon最小化按钮
    });
    Array_features->trigger();

    QAction* Mirror_feature = createAction(tr("镜像特征"), ":/icon/icons/镜像(1).png", "show hide button");
    Mirror_feature->setCheckable(false);
    pannelBase->addMediumAction(Mirror_feature);
    connect(Mirror_feature, &QAction::triggered, this, [this](bool b) {
        this->ribbonBar()->showMinimumModeButton(b);  //显示ribbon最小化按钮
    });
    Mirror_feature->trigger();

    //pannelBase->addSeparator();
    QAction* act;
    SARibbonToolButton* btn;
    SARibbonMenu* menu = new SARibbonMenu(this);

    act = createAction(tr("用户自定义"), ":/icon/icons/用户自定义.png");
    act->setMenu(menu);
    btn = pannelBase->addLargeAction(act);
    btn->setPopupMode(QToolButton::DelayedPopup);
    connect(act, &QAction::triggered, this, &MainWindow::onDelayedPopupCheckabletriggered);


    QAction* optAct = new QAction(this);
    connect(optAct, &QAction::triggered, this, [this](bool on) {
        Q_UNUSED(on);
        QMessageBox::information(this, tr("Option Action Triggered"), tr("Option Action Triggered"));
    });
    pannelBase->addOptionAction(optAct);



    SARibbonPannel* Synchronous_modeling = page->addPannel(("同步建模"));

    QAction* Move_face = createAction(tr("移动面"), ":/icon/icons/移动面.png");
    Move_face->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    Synchronous_modeling->addLargeAction(Move_face);
    connect(Move_face, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });

    QAction* Delete_face = createAction(tr("删除面"), ":/icon/icons/删除.png");
    Delete_face->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    Synchronous_modeling->addLargeAction(Delete_face);
    connect(Delete_face, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });

    QAction* Replace_face = createAction(tr("替换面"), ":/icon/icons/替换.png");
    Replace_face->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    Synchronous_modeling->addLargeAction(Replace_face);
    connect(Replace_face, &QAction::triggered, this, [this](bool b) {
        Q_UNUSED(b);
        //this->m_edit->append("actSaveion clicked");
    });

    QAction* Offset_face = createAction(tr("偏置面"), ":/icon/icons/偏置面.png", "actHideRibbon");
    Offset_face->setCheckable(false);
    Synchronous_modeling->addMediumAction(Offset_face);
    connect(Offset_face, &QAction::triggered, this, [this](bool b) { this->ribbonBar()->setMinimumMode(b); });
    connect(ribbonBar(), &SARibbonBar::ribbonModeChanged, this, [Offset_face](SARibbonBar::RibbonMode nowNode) {
        Offset_face->setChecked(nowNode == SARibbonBar::MinimumRibbonMode);
    });

    QAction* Resize_blend = createAction(tr("调整倒圆角大小"), ":/icon/icons/调整倒圆角大小(1).png", "actHideRibbon");
    Resize_blend->setCheckable(false);
    Synchronous_modeling->addMediumAction(Resize_blend);
    connect(Resize_blend, &QAction::triggered, this, [this](bool b) { this->ribbonBar()->setMinimumMode(b); });
    connect(ribbonBar(), &SARibbonBar::ribbonModeChanged, this, [Resize_blend](SARibbonBar::RibbonMode nowNode) {
        Resize_blend->setChecked(nowNode == SARibbonBar::MinimumRibbonMode);
    });

    act = createAction(tr("更多"), ":/icon/icons/用户自定义.png");
    act->setMenu(menu);
    btn = Synchronous_modeling->addLargeAction(act);
    btn->setPopupMode(QToolButton::DelayedPopup);
    connect(act, &QAction::triggered, this, &MainWindow::onDelayedPopupCheckabletriggered);

}

void MainWindow::createCategoryOther(SARibbonCategory* page)
{

    SARibbonPannel* pannel1 = new SARibbonPannel(tr("pannel one"));
    pannel1->setObjectName("CategoryOther-pannel1");
    page->addPannel(pannel1);
    //按钮组
    SARibbonButtonGroupWidget* btnGroup1 = new SARibbonButtonGroupWidget(pannel1);
    btnGroup1->addAction(createAction(tr("Decrease Margin"), ":/icon/icon/Decrease-Margin.svg"));
    btnGroup1->addAction(createAction(tr("Decrease Indent"), ":/icon/icon/Decrease-Indent.svg"));
    btnGroup1->addAction(createAction(tr("Wrap Image Left"), ":/icon/icon/Wrap-Image Left.svg"));
    btnGroup1->addAction(createAction(tr("Wrap Image Right"), ":/icon/icon/Wrap-Image Right.svg"));
    pannel1->addWidget(btnGroup1, SARibbonPannelItem::Medium);
    SARibbonButtonGroupWidget* btnGroup2 = new SARibbonButtonGroupWidget(pannel1);
    QAction* titleAlgnment               = createAction(tr("Align Right"), ":/icon/icon/Align-Right.svg");
    titleAlgnment->setProperty("align", (int)Qt::AlignRight | Qt::AlignVCenter);
    btnGroup2->addAction(titleAlgnment);
    titleAlgnment = createAction(tr("Align Left"), ":/icon/icon/Align-Left.svg");
    titleAlgnment->setProperty("align", (int)Qt::AlignLeft | Qt::AlignVCenter);
    btnGroup2->addAction(titleAlgnment);
    titleAlgnment = createAction(tr("Align Center"), ":/icon/icon/Align-Center.svg");
    titleAlgnment->setProperty("align", (int)Qt::AlignCenter);
    btnGroup2->addAction(titleAlgnment);
    pannel1->addWidget(btnGroup2, SARibbonPannelItem::Medium);
    connect(btnGroup2, &SARibbonButtonGroupWidget::actionTriggered, this, &MainWindow::onButtonGroupActionTriggered);
    // Gallery
    SARibbonGallery* gallery = pannel1->addGallery();
    QList< QAction* > galleryActions;
    auto lambdaCreateGalleryAction = [ this ](const QString& text, const QString& iconurl) -> QAction* {
        QAction* act = this->createAction(text, iconurl);
        this->connect(act, &QAction::triggered, this, [ this, text ]() {
            if (this->m_edit) {
                this->m_edit->append(QString("%1 triggered").arg(text));
            }
        });
        return act;
    };
    galleryActions.append(lambdaCreateGalleryAction("Document File", ":/gallery-icon/icon/gallery/Document-File.svg"));
    galleryActions.append(lambdaCreateGalleryAction("Download File", ":/gallery-icon/icon/gallery/Download-File.svg"));
    galleryActions.append(lambdaCreateGalleryAction("Drive File Four Word", ":/gallery-icon/icon/gallery/Drive-File.svg"));
    galleryActions.append(lambdaCreateGalleryAction("Dropbox File", ":/gallery-icon/icon/gallery/Dropbox-File.svg"));
    galleryActions.append(lambdaCreateGalleryAction("Email File", ":/gallery-icon/icon/gallery/Email-File.svg"));
    galleryActions.append(lambdaCreateGalleryAction("Encode File", ":/gallery-icon/icon/gallery/Encode-File.svg"));
    galleryActions.append(lambdaCreateGalleryAction("Favorit File", ":/gallery-icon/icon/gallery/Favorit-File.svg"));
    galleryActions.append(lambdaCreateGalleryAction("File Error", ":/gallery-icon/icon/gallery/File-Error.svg"));
    galleryActions.append(lambdaCreateGalleryAction("File Read Only", ":/gallery-icon/icon/gallery/File-Readonly.svg"));
    galleryActions.append(lambdaCreateGalleryAction("File Settings", ":/gallery-icon/icon/gallery/File-Settings.svg"));
    galleryActions.append(lambdaCreateGalleryAction("Presentation File", ":/gallery-icon/icon/gallery/Presentation-File.svg"));
    SARibbonGalleryGroup* group1 = gallery->addCategoryActions(tr("Files"), galleryActions);
    group1->setGalleryGroupStyle(SARibbonGalleryGroup::IconWithWordWrapText);
    group1->setGridMinimumWidth(80);
    galleryActions.clear();
    galleryActions.append(lambdaCreateGalleryAction("Photoshop", ":/gallery-icon/icon/gallery/Photoshop.svg"));
    galleryActions.append(lambdaCreateGalleryAction("Internet-Explorer", ":/gallery-icon/icon/gallery/Internet-Explorer.svg"));
    galleryActions.append(lambdaCreateGalleryAction("Illustrator", ":/gallery-icon/icon/gallery/Illustrator.svg"));
    galleryActions.append(lambdaCreateGalleryAction("Google-Maps", ":/gallery-icon/icon/gallery/Google-Maps.svg"));
    galleryActions.append(lambdaCreateGalleryAction("Adobe", ":/gallery-icon/icon/gallery/Adobe.svg"));
    galleryActions.append(lambdaCreateGalleryAction("Word", ":/gallery-icon/icon/gallery/Word.svg"));
    SARibbonGalleryGroup* group2 = gallery->addCategoryActions(tr("Apps"), galleryActions);
    group2->setGridMinimumWidth(80);
    gallery->setCurrentViewGroup(group1);

    QAction* optAct = new QAction(this);

    optAct->setObjectName(("debug"));
    pannel1->addOptionAction(optAct);

    SARibbonPannel* pannel2 = new SARibbonPannel(tr("pannel two"));
    pannel2->setObjectName("CategoryOther-pannel2");
    page->addPannel(pannel2);
    QAction* actionRemoveAppBtn = createAction(tr("remove application button"), ":/icon/icon/remove-app-btn.svg");
    actionRemoveAppBtn->setCheckable(true);
    connect(actionRemoveAppBtn, &QAction::toggled, this, &MainWindow::onActionRemoveAppBtnTriggered);
    pannel2->addLargeAction(actionRemoveAppBtn);

    QAction* actionLongText = createAction(tr("show very long text in a button,balabalabala etc"), ":/icon/icon/long-text.svg", "long-text");
    pannel2->addLargeAction(actionLongText);

    SARibbonPannel* pannelStyle = new SARibbonPannel(tr("style"));
    pannelStyle->setObjectName("CategoryOther-pannelStyle");
    page->addPannel(pannelStyle);

    QAction* actionUseQss = createAction(tr("use qss"), ":/icon/icon/useqss.svg");
    connect(actionUseQss, &QAction::triggered, this, &MainWindow::onActionUseQssTriggered);
    pannelStyle->addSmallAction(actionUseQss);

    QAction* actionLoadCustomizeXmlFile = createAction(tr("load customize from xml file"), ":/icon/icon/useCustomize.svg");
    connect(actionLoadCustomizeXmlFile, &QAction::triggered, this, &MainWindow::onActionLoadCustomizeXmlFileTriggered);
    pannelStyle->addSmallAction(actionLoadCustomizeXmlFile);

    QAction* actionWindowFlagNormalButton = createAction(tr("window normal button"), ":/icon/icon/windowsflag-normal.svg");
    actionWindowFlagNormalButton->setCheckable(true);
    actionWindowFlagNormalButton->setChecked(true);
    pannelStyle->addSmallAction(actionWindowFlagNormalButton);
    connect(actionWindowFlagNormalButton, &QAction::triggered, this, &MainWindow::onActionWindowFlagNormalButtonTriggered);

    SARibbonPannel* pannelUtf8 = new SARibbonPannel(QStringLiteral(u"中文显示测试"));
    pannelUtf8->setObjectName("CategoryOther-pannelUtf8");
    page->addPannel(pannelUtf8);

    QAction* actionChangeText = createAction(QStringLiteral(u"改变显示为英文"), ":/icon/icon/chinese-char.svg", "actionChangeText");
    actionChangeText->setCheckable(true);
    actionChangeText->setChecked(false);
    pannelUtf8->addLargeAction(actionChangeText);
    connect(actionChangeText, &QAction::triggered, this, [ pannelUtf8, actionChangeText ](bool on) {
        if (on) {
            pannelUtf8->setPannelName(QStringLiteral(u"show chinese char"));
            actionChangeText->setText(QStringLiteral(u"show in chinese"));
        } else {
            pannelUtf8->setPannelName(QStringLiteral(u"中文显示测试"));
            actionChangeText->setText(QStringLiteral(u"改变显示为英文"));
        }
    });

    SARibbonPannel* pannelContextCategory = new SARibbonPannel(("Context Category"));
    page->addPannel(pannelContextCategory);
    QAction* a1 = createAction("Context Category 1", ":/icon/icon/ContextCategory.svg");
    a1->setCheckable(true);
    connect(a1, &QAction::triggered, this, [ this ](bool c) {
        this->ribbonBar()->setContextCategoryVisible(m_contextCategory, c);
    });

    QAction* a2 = createAction("Context Category 2", ":/icon/icon/ContextCategory.svg");
    a2->setCheckable(true);
    connect(a2, &QAction::triggered, this, [ this ](bool c) {
        this->ribbonBar()->setContextCategoryVisible(m_contextCategory2, c);
    });
    pannelContextCategory->addLargeAction(a1);
    pannelContextCategory->addLargeAction(a2);
}

void MainWindow::createCategoryDelete(SARibbonCategory* page)
{
    SARibbonPannel* pannel1 = new SARibbonPannel(("pannel 1"));
    SARibbonPannel* pannel2 = new SARibbonPannel(("pannel 2"));

    QAction* actionRemovePanne = createAction(tr("remove pannel"), ":/icon/icon/remove.svg");

    connect(actionRemovePanne, &QAction::triggered, this, [ page, pannel2 ]() { page->removePannel(pannel2); });
    pannel1->addLargeAction(actionRemovePanne);

    QAction* actionRemoveCategory = createAction(tr("remove this category"), ":/icon/icon/removePage.svg");

    connect(actionRemoveCategory, &QAction::triggered, this, [ this, page, actionRemoveCategory ]() {
        this->ribbonBar()->removeCategory(page);
        page->hide();
        page->deleteLater();
        actionRemoveCategory->setDisabled(true);
    });
    pannel2->addLargeAction(actionRemoveCategory);

    page->addPannel(pannel1);
    page->addPannel(pannel2);
}

/**
 * @brief 构建尺寸页
 * @param page
 */
void MainWindow::createCategorySize(SARibbonCategory* page)
{
    QAction* act                      = nullptr;
    SARibbonPannel* pannel            = page->addPannel(tr("Font"));
    SARibbonButtonGroupWidget* group1 = new SARibbonButtonGroupWidget(pannel);
    group1->setObjectName(QStringLiteral(u"group1"));
    QLabel* labelFontSize = new QLabel(group1);
    labelFontSize->setText(tr("select font"));
    labelFontSize->setObjectName(QStringLiteral(u"labelFontSize"));
    group1->addWidget(labelFontSize);
    QFontComboBox* fontComWidget = new QFontComboBox(group1);
    fontComWidget->setObjectName(QStringLiteral(u"fontComboBox"));
    connect(fontComWidget, &QFontComboBox::currentFontChanged, this, &MainWindow::onFontComWidgetCurrentFontChanged);
    group1->addWidget(fontComWidget);
    act = pannel->addWidget(group1, SARibbonPannelItem::Medium);
    act->setObjectName(labelFontSize->objectName());

    SARibbonButtonGroupWidget* group2 = new SARibbonButtonGroupWidget(pannel);
    group2->setObjectName(QStringLiteral(u"group2"));
    group2->addAction(createAction("Bold", ":/icon/icon/bold.svg"));
    group2->addAction(createAction("Italic", ":/icon/icon/Italic.svg"));
    group2->addSeparator();
    group2->addAction(createAction("left alignment", ":/icon/icon/al-left.svg"));
    group2->addAction(createAction("center alignment", ":/icon/icon/al-center.svg"));
    group2->addAction(createAction("right alignment", ":/icon/icon/al-right.svg"));
    group2->addAction(createAction("line up on both sides", ":/icon/icon/al-bothside.svg"));
    act = pannel->addWidget(group2, SARibbonPannelItem::Medium);
    act->setObjectName(group2->objectName());

    pannel->addSeparator();

    QAction* actLargerFontSize = createAction(tr("Larger"), ":/icon/icon/largerFont.svg", "actLargerFontSize");
    pannel->addLargeAction(actLargerFontSize);

    QAction* actSmallFontSize = createAction(tr("Smaller"), ":/icon/icon/smallFont.svg", "actSmallFontSize");
    pannel->addLargeAction(actSmallFontSize);

    connect(actLargerFontSize, &QAction::triggered, this, &MainWindow::onActionFontLargerTriggered);
    connect(actSmallFontSize, &QAction::triggered, this, &MainWindow::onActionFontSmallerTriggered);
}

/**
 * @brief 创建上下文标签
 */
void MainWindow::createContextCategory1()
{
    //上下文标签
    SARibbonBar* ribbon = ribbonBar();
    m_contextCategory   = ribbon->addContextCategory(tr("context"), QColor(), 1);

    SARibbonCategory* contextCategoryPage1 = m_contextCategory->addCategoryPage(tr("Page1"));
    createContextCategoryPage1(contextCategoryPage1);

    SARibbonCategory* contextCategoryPage2 = m_contextCategory->addCategoryPage(tr("Page2"));
    createContextCategoryPage2(contextCategoryPage2);
}

void MainWindow::createContextCategory2()
{
    SARibbonBar* ribbon = ribbonBar();
    m_contextCategory2  = ribbon->addContextCategory(("context2"), QColor(), 2);
    m_contextCategory2->addCategoryPage(("context2 Page1"));
    m_contextCategory2->addCategoryPage(("context2 Page2"));
}

void MainWindow::createContextCategoryPage1(SARibbonCategory* page)
{
    SARibbonPannel* pannel = page->addPannel(tr("show and hide test"));

    QAction* actionHidePannel = createAction("hide pannel", ":/icon/icon/hidePannel.svg");
    actionHidePannel->setCheckable(true);
    pannel->addLargeAction(actionHidePannel);

    QAction* actionDisable = createAction(tr("Disable"), ":/icon/icon/enableTest.svg");

    actionDisable->setDisabled(true);
    pannel->addLargeAction(actionDisable);
    connect(actionDisable, &QAction::triggered, this, [ actionDisable ](bool b) {
        Q_UNUSED(b);
        actionDisable->setDisabled(true);
    });

    QAction* actionUnlock = createAction(tr("unlock"), ":/icon/icon/unlock.svg");
    actionUnlock->setShortcut(QKeySequence(QLatin1String("Ctrl+E")));
    pannel->addLargeAction(actionUnlock);
    connect(actionUnlock, &QAction::triggered, this, [ actionDisable ](bool b) {
        Q_UNUSED(b);
        actionDisable->setEnabled(true);
        actionDisable->setText(("Enabled"));
    });

    QAction* actionSetTextTest = createAction("set text", ":/icon/icon/setText.svg");

    actionSetTextTest->setCheckable(true);
    actionSetTextTest->setShortcut(QKeySequence(QLatin1String("Ctrl+D")));
    pannel->addLargeAction(actionSetTextTest);

    connect(actionSetTextTest, &QAction::toggled, this, [ actionSetTextTest ](bool b) {
        if (b) {
            actionSetTextTest->setText(QStringLiteral(u"setText测试"));
        } else {
            actionSetTextTest->setText(QStringLiteral(u"set text"));
        }
    });
    //隐藏pannel
    QAction* actionShowTest = createAction("show beside pannel", ":/icon/icon/show.svg");

    actionShowTest->setCheckable(true);
    pannel->addLargeAction(actionShowTest);

    SARibbonPannel* pannel2 = page->addPannel(tr("show/hide"));

    pannel2->addLargeAction(actionSetTextTest);

    connect(actionShowTest, &QAction::toggled, this, [ actionShowTest, pannel2, this ](bool b) {
        pannel2->setVisible(!b);
        if (b) {
            actionShowTest->setText(tr("hide beside pannel"));
        } else {
            actionShowTest->setText(tr("show beside pannel"));
        }
        ribbonBar()->repaint();
    });

    SARibbonPannel* pannel3 = page->addPannel(("show/hide action test"));

    QAction* actionHideAction2 = createAction("hide action 2", ":/icon/icon/action.svg");
    QAction* actionHideAction4 = createAction("hide action 4", ":/icon/icon/action.svg");
    actionHideAction2->setCheckable(true);
    actionHideAction2->setChecked(true);
    actionHideAction4->setCheckable(true);
    actionHideAction4->setChecked(true);
    QAction* act2 = createAction("action2", ":/icon/icon/action2.svg");
    QAction* act3 = createAction("action3", ":/icon/icon/action3.svg");
    QAction* act4 = createAction("action4", ":/icon/icon/action4.svg");
    pannel3->addLargeAction(actionHideAction2);
    pannel3->addLargeAction(actionHideAction4);
    pannel3->addSmallAction(act2);
    pannel3->addSmallAction(act3);
    pannel3->addSmallAction(act4);
    connect(actionHideAction2, &QAction::triggered, this, [ actionHideAction2, act2 ](bool b) {
        if (b) {
            act2->setVisible(true);
            actionHideAction2->setText(tr("hide action2"));
        } else {
            act2->setVisible(false);
            actionHideAction2->setText(tr("show action2"));
        }
    });
    connect(actionHideAction4, &QAction::triggered, this, [ actionHideAction4, act4 ](bool b) {
        if (b) {
            act4->setVisible(true);
            actionHideAction4->setText(tr("hide action4"));
        } else {
            act4->setVisible(false);
            actionHideAction4->setText(tr("show action4"));
        }
    });
}

void MainWindow::createContextCategoryPage2(SARibbonCategory* page)
{
    SARibbonPannel* pannel1 = page->addPannel(("删除CategoryPage测试"));

    QAction* actionDeleteThisCategory = createAction("delete this category", ":/icon/icon/delete.svg");
    pannel1->addLargeAction(actionDeleteThisCategory);
    connect(actionDeleteThisCategory, &QAction::triggered, this, [ this, page ]() {
        this->ribbonBar()->removeCategory(page);
        page->deleteLater();
    });
    SARibbonPannel* pannelLayout = page->addPannel(("特殊布局"));

    pannelLayout->addAction(createAction("Large", ":/icon/icon/layout.svg", "@Large1"), QToolButton::InstantPopup, SARibbonPannelItem::Large);
    pannelLayout->addAction(createAction("Small", ":/icon/icon/layout.svg", "@Small1"), QToolButton::InstantPopup, SARibbonPannelItem::Small);
    pannelLayout->addAction(createAction("Small", ":/icon/icon/layout.svg", "@Small2"), QToolButton::InstantPopup, SARibbonPannelItem::Small);
    pannelLayout->addSeparator();
    pannelLayout->addAction(createAction("Small", ":/icon/icon/layout.svg", "@Small3"), QToolButton::InstantPopup, SARibbonPannelItem::Small);
    pannelLayout->addAction(createAction("Small", ":/icon/icon/layout.svg", "@Small4"), QToolButton::InstantPopup, SARibbonPannelItem::Small);
    pannelLayout->addAction(createAction("Small", ":/icon/icon/layout.svg", "@Small5"), QToolButton::InstantPopup, SARibbonPannelItem::Small);
    pannelLayout->addAction(createAction("Small", ":/icon/icon/layout.svg", "@Small6"), QToolButton::InstantPopup, SARibbonPannelItem::Small);
    pannelLayout->addSeparator();
    pannelLayout->addAction(createAction("Large", ":/icon/icon/layout.svg", "@Large2"), QToolButton::InstantPopup, SARibbonPannelItem::Large);
    pannelLayout->addAction(createAction("Medium", ":/icon/icon/layout.svg", "@Medium1"),
                            QToolButton::InstantPopup,
                            SARibbonPannelItem::Medium);
    pannelLayout->addAction(createAction("Medium", ":/icon/icon/layout.svg", "@Medium2"),
                            QToolButton::InstantPopup,
                            SARibbonPannelItem::Medium);
    pannelLayout->addAction(createAction("Small", ":/icon/icon/layout.svg", "@Small7"), QToolButton::InstantPopup, SARibbonPannelItem::Small);
    pannelLayout->addAction(createAction("Medium", ":/icon/icon/layout.svg", "@Medium3"),
                            QToolButton::InstantPopup,
                            SARibbonPannelItem::Medium);
    pannelLayout->addAction(createAction("Large", ":/icon/icon/layout.svg", "@Large3"), QToolButton::InstantPopup, SARibbonPannelItem::Large);
    pannelLayout->addAction(createAction("Medium", ":/icon/icon/layout.svg", "@Medium4"),
                            QToolButton::InstantPopup,
                            SARibbonPannelItem::Medium);
    pannelLayout->addAction(createAction("Medium", ":/icon/icon/layout.svg", "@Medium5"),
                            QToolButton::InstantPopup,
                            SARibbonPannelItem::Medium);
    pannelLayout->addAction(createAction("Large", ":/icon/icon/layout.svg", "@Large4"), QToolButton::InstantPopup, SARibbonPannelItem::Large);
    pannelLayout->addSeparator();
    pannelLayout->addAction(createAction("Medium", ":/icon/icon/layout.svg", "@Medium6"),
                            QToolButton::InstantPopup,
                            SARibbonPannelItem::Medium);
    pannelLayout->addAction(createAction("Large", ":/icon/icon/layout.svg", "@Large5"), QToolButton::InstantPopup, SARibbonPannelItem::Large);
    pannelLayout->addAction(createAction("Medium", ":/icon/icon/layout.svg", "@Medium7"),
                            QToolButton::InstantPopup,
                            SARibbonPannelItem::Medium);
    pannelLayout->addAction(createAction("Small", ":/icon/icon/layout.svg", "@Small8"), QToolButton::InstantPopup, SARibbonPannelItem::Small);
}

void MainWindow::createQuickAccessBar(SARibbonQuickAccessBar* quickAccessBar)
{
    quickAccessBar->addAction(createAction("save", ":/icon/icons/保存.png", "save-quickbar"));
    quickAccessBar->addSeparator();
    quickAccessBar->addAction(createAction("undo", ":/icon/icon/undo.svg"));
    quickAccessBar->addAction(createAction("redo", ":/icon/icon/redo.svg"));
    quickAccessBar->addSeparator();
    QMenu* m = new QMenu("Presentation File", this);
    m->setIcon(QIcon(":/icon/icon/presentationFile.svg"));
    for (int i = 0; i < 10; ++i) {
        m->addAction(createAction(QString("file%1").arg(i + 1), ":/icon/icon/file.svg"));
    }
    quickAccessBar->addMenu(m);

    QAction* customize = createAction("customize", ":/icon/icon/customize0.svg", "customize2");
    quickAccessBar->addAction(customize);
    connect(customize, &QAction::triggered, this, &MainWindow::onActionCustomizeTriggered);

    QAction* actionCustomizeAndSave = createAction("customize and save", ":/icon/icon/customize.svg");
    quickAccessBar->addAction(actionCustomizeAndSave);
    connect(actionCustomizeAndSave, &QAction::triggered, this, &MainWindow::onActionCustomizeAndSaveTriggered);
}

void MainWindow::createRightButtonGroup(SARibbonButtonGroupWidget* rightBar)
{
    QAction* actionHelp = createAction(tr("help"), ":/icon/icon/help.svg");
    connect(actionHelp, &QAction::triggered, this, &MainWindow::onActionHelpTriggered);
    rightBar->addAction(actionHelp);
}

void MainWindow::addSomeOtherAction()
{
    //添加其他的action，这些action并不在ribbon管理范围，主要用于SARibbonCustomizeWidget自定义用
    QAction* acttext1 = new QAction(("text action1"), this);
    QAction* acttext2 = new QAction(("text action2"), this);
    QAction* acttext3 = new QAction(("text action3"), this);
    QAction* acttext4 = new QAction(("text action4"), this);
    QAction* acttext5 = new QAction(("text action5"), this);

    QAction* actIcon1 = new QAction(QIcon(":/icon/icon/layout.svg"), ("action with icon"), this);

    m_actionTagText     = SARibbonActionsManager::UserDefineActionTag + 1;
    m_actionTagWithIcon = SARibbonActionsManager::UserDefineActionTag + 2;

    m_actMgr = new SARibbonActionsManager(this);  //申明过程已经自动注册所有action

    //以下注册特别的action
    m_actMgr->registeAction(acttext1, SARibbonActionsManager::CommonlyUsedActionTag);
    m_actMgr->registeAction(acttext3, SARibbonActionsManager::CommonlyUsedActionTag);
    m_actMgr->registeAction(acttext5, SARibbonActionsManager::CommonlyUsedActionTag);
    m_actMgr->registeAction(actIcon1, SARibbonActionsManager::CommonlyUsedActionTag);

    m_actMgr->registeAction(acttext1, m_actionTagText);
    m_actMgr->registeAction(acttext2, m_actionTagText);
    m_actMgr->registeAction(acttext3, m_actionTagText);
    m_actMgr->registeAction(acttext4, m_actionTagText);
    m_actMgr->registeAction(acttext5, m_actionTagText);

    m_actMgr->registeAction(actIcon1, m_actionTagWithIcon);

    m_actMgr->setTagName(SARibbonActionsManager::CommonlyUsedActionTag, ("常用"));
    m_actMgr->setTagName(m_actionTagText, ("无图标action"));
    m_actMgr->setTagName(m_actionTagWithIcon, ("有图标action"));
}

QAction* MainWindow::createAction(const QString& text, const QString& iconurl, const QString& objName)
{
    QAction* act = new QAction(this);
    act->setText(text);
    act->setIcon(QIcon(iconurl));
    act->setObjectName(objName);
    return act;
}

QAction* MainWindow::createAction(const QString& text, const QString& iconurl)
{
    QAction* act = new QAction(this);
    act->setText(text);
    act->setIcon(QIcon(iconurl));
    act->setObjectName(text);
    return act;
}

void MainWindow::onMenuButtonPopupCheckableTest(bool b)
{
    m_edit->append(QString("MenuButtonPopupCheckableTest : %1").arg(b));
}

void MainWindow::onInstantPopupCheckableTest(bool b)
{
    m_edit->append(QString("InstantPopupCheckableTest : %1").arg(b));
}

void MainWindow::onDelayedPopupCheckableTest(bool b)
{
    m_edit->append(QString("DelayedPopupCheckableTest : %1").arg(b));
}

void MainWindow::onMenuButtonPopupCheckabletriggered(bool b)
{
    m_edit->append(QString("MenuButtonPopupCheckabletriggered : %1").arg(b));
}

void MainWindow::onInstantPopupCheckabletriggered(bool b)
{
    m_edit->append(QString("InstantPopupCheckabletriggered : %1").arg(b));
}

void MainWindow::onDelayedPopupCheckabletriggered(bool b)
{
    m_edit->append(QString("DelayedPopupCheckabletriggered : %1").arg(b));
}
