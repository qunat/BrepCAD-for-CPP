#include "Sketchermanager.h"
#include "../DockModelTree.h"
#include <Geom_Plane.hxx>
#include <gp_Pln.hxx>
#include <AIS_InteractiveObject.hxx>
#include<AIS_KindOfInteractive.hxx>
#include<AIS_TypeOfPlane.hxx>
#include<./display/DisplayCore.h>
#include <AIS_Plane.hxx>
#include <AIS_Shape.hxx>
#include <Geom_Axis2Placement.hxx>
#include<stddef.h>
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
	auto father = parent->windownsmanager->windowns_name[parent->windownsmanager->current_windown]->myOccView;
	connect(father, SIGNAL(mySignal(int)), this, SLOT(mySlot()));
	
}
void Sketchermanager::mySlot()
{
	Handle(AIS_Plane) selected__ais_plane;
	parent->DisplayCoreManeger[parent->windownsmanager->current_windown]->Context->InitDetected();
	Handle(AIS_InteractiveObject) plane = parent->DisplayCoreManeger[parent->windownsmanager->current_windown]->Context->DetectedCurrentObject();// 通过此方法可以当前鼠标点击的ais_shape
	try
	{
		selected__ais_plane = Handle(AIS_Plane)::DownCast(plane);

		if (!selected__ais_plane)
		{
			goto end;
		}

	}
	catch (...)
	{
		
		
	}




	if (selected__ais_plane->Type() == AIS_KOI_Datum )//选择的为Datum  AIS_KindOfInteractive_Datum 
	{

		Handle(Geom_Plane) select_geomplane = selected__ais_plane->Component();
		gp_Dir direct = select_geomplane->Pln().Axis().Direction();
		auto x = direct.X();
		auto y = direct.Y();
		auto z = direct.Z();
		auto coord = direct.Coord(3);
		if (x == double(1))
		{
			parent->windownsmanager->windowns_name[parent->windownsmanager->current_windown]->myOccView->View_Top();
		}
		else if (y == double(1))
		{
			parent->windownsmanager->windowns_name[parent->windownsmanager->current_windown]->myOccView->View_Front();
		}

		else if (z == double(1))
		{
			parent->windownsmanager->windowns_name[parent->windownsmanager->current_windown]->myOccView->View_Left();

		}


	}
	else if (selected__ais_plane->Type() == AIS_KOI_Shape)////选择的为shape AIS_KindOfInteractive_Shape 	
	{

	}


	end:;
	
}