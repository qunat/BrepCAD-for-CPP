#include "Sketchermanager.h"
#include "../DockModelTree.h"

Sketchermanager::Sketchermanager(MainWindow*par)
{
	parent = par;
}

Sketchermanager::Sketchermanager()
{
}



void Sketchermanager::Showgui()
{
	auto dockmodeltree =parent->dockmodeltreemap[parent->windownsmanager->current_windown];
	auto dock_table = dockmodeltree->dock_tabWidget;
	dock_table->setCurrentIndex(2);
	
}
