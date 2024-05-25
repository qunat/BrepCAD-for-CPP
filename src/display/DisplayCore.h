#pragma once
#include <occView.h>
#include <AIS_Shape.hxx>
#include <TopoDS_Shape.hxx>
class Display_Core
{
public:
	Handle(AIS_InteractiveContext) Context;
public:
	Display_Core(OccView*parent);
	~Display_Core();
	void DisplayShape(TopoDS_Shape shape, Quantity_Color color, double transparency,Graphic3d_MaterialAspect material = Graphic3d_NOM_STEEL,Standard_Boolean theToUpdateViewer=true);
	void DisplayColorShape(TopoDS_Shape shape, Quantity_Color color, double transparency, Graphic3d_MaterialAspect material = Graphic3d_NOM_STEEL,Standard_Boolean theToUpdateViewer = true);


};