#pragma once
#include "SARibbonMainWindow.h"
#include <AIS_InteractiveContext.hxx>
#include <occView.h>
class SARibbonCategory;
class SARibbonContextCategory;
class SARibbonCustomizeWidget;
class SARibbonActionsManager;
class SARibbonQuickAccessBar;
class SARibbonButtonGroupWidget;
class QTextEdit;

class RibbonManeger : public SARibbonMainWindow
{


public:
	RibbonManeger();
	~RibbonManeger();
	void AutoCreateRibbon();
	void CreateRibbon();


	
};