#include "Sketchermanager.h"
#include "../DockModelTree.h"
#include <Geom_Plane.hxx>
#include <gp_Pln.hxx>

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
	parent->DisplayCoreManeger[parent->windownsmanager->current_windown]->Context->InitDetected();
	auto plane = parent->DisplayCoreManeger[parent->windownsmanager->current_windown]->Context->DetectedCurrentObject();// 通过此方法可以当前鼠标点击的ais_shape
	Handle(AIS_Plane) anAISShape = Handle(AIS_Plane)::DownCast(plane);
	Handle(Geom_Plane) geomplane = anAISShape->Component();
	gp_Pln pln = geomplane->Pln();
	gp_XYZ   asis = pln.Axis().Direction().XYZ();
	//auto plane = shape.DownCast;																								//auto shape = shape.get();
	//Handle(AIS_Shape) ais_shape = new AIS_Shape(shape.Reversed());
	//AIS_Plane plane = AIS_Plane::(shape); // get the TopoDS_Shape

}