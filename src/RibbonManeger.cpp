#include"RibbonManeger.h"
#include <qfile.h>
#include <fstream>
#include<QtCore/qfiledevice.h>
#include "GuiManeger.h"
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
#include <RibbonManeger.h>
#include <chrono>
#include <iostream>
using namespace std;


RibbonManeger::RibbonManeger()
{
    this->AutoCreateRibbon();
}
RibbonManeger::~RibbonManeger()
	{
	}

void RibbonManeger::AutoCreateRibbon()

{// ��.dat�ļ���ʹ�ö�����ģʽ��
    

    QFile file("./config/Ribbon_main.ini");

    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "�޷����ļ�" << std::endl;
        
    }

    else
    {
        std::cout << "���ļ��ɹ�" << std::endl;
		// �����ļ����뷽ʽ
        QTextStream in(&file);
        in.setCodec("UTF-8"); // ����.dat�ļ�����GBK���뱣���

        // ��ȡ��������
        QString content = in.readAll();

		// ��ʾ��������

		std::cout << content.toStdString() << std::endl;
        QStringList content_list = content.split("\n");
        for (int i = 0; i < content_list.size(); i++)
        {
			//std::cout << content_list[i].toStdString() << std::endl;
		}

		file.close();
    }

}
