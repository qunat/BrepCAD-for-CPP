#include<display/DisplayCore.h>
#include <occView.h>
#include <AIS_InteractiveContext.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <AIS_Shape.hxx>
#include<Prs3d_LineAspect.hxx>
#include <Quantity_Color.hxx>



Display_Core::Display_Core(OccView*parent)
{
	Context =parent->getContext();
	TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(100.0, 100.0, 100.0).Shape();
	Quantity_Color color(0.5,0.6,0.7, Quantity_TOC_RGB) ;
	this->DisplayShape(aTopoBox, color, 1);
	//Context->SetDisplayMode(AIS_Shaded, Standard_True);
	//TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(100.0, 100.0, 100.0).Shape();
	//Handle(AIS_Shape) anAisBox = new AIS_Shape(aTopoBox);
	//anAisBox->SetMaterial(Graphic3d_MaterialAspect(Graphic3d_NOM_STEEL));
	// Set Display mode
	//anAisBox->SetDisplayMode((int)AIS_DisplayMode::AIS_Shaded);
	//Handle(Prs3d_LineAspect)    aEdgeAspt;		// Face boundary Aspect
	//aEdgeAspt = new Prs3d_LineAspect(Quantity_NOC_BLACK, Aspect_TypeOfLine::Aspect_TOL_SOLID, 1.2);
	// Set Edge Color
	//anAisBox->Attributes()->SetFaceBoundaryAspect(aEdgeAspt);
	//auto drawer = anAisBox->Attributes();
	//drawer->SetFaceBoundaryDraw(true);
	//Handle(Prs3d_LineAspect) aspect = new Prs3d_LineAspect(Quantity_NOC_BLACK,Aspect_TOL_SOLID,1.0);
	//drawer->SetFaceBoundaryAspect(aspect);
	// redisplay presentation to have applyed parameters of face boundary aspect
	//anAisBox->SetColor(Quantity_NOC_AZURE);
	//Context->Display(anAisBox,Standard_True);
	


}
void Display_Core::DisplayShape(TopoDS_Shape shape, Quantity_Color color, double transparency, Graphic3d_MaterialAspect material ,Standard_Boolean theToUpdateViewer)
{
	Handle(AIS_Shape) ais_shape = new AIS_Shape(shape);
	ais_shape->SetMaterial(Graphic3d_MaterialAspect(Graphic3d_NOM_STEEL));
	//Set Display color 
	ais_shape->SetColor(color);
	// Set Display mode
	ais_shape->SetDisplayMode((int)AIS_DisplayMode::AIS_Shaded);
	// Set Edge Color
	auto drawer = ais_shape->Attributes();
	drawer->SetFaceBoundaryDraw(true);
	Handle(Prs3d_LineAspect) aspect = new Prs3d_LineAspect(Quantity_NOC_BLACK, Aspect_TOL_SOLID, 1.0);
	drawer->SetFaceBoundaryAspect(aspect);
	// redisplay presentation to have applyed parameters of face boundary aspect
	Context->Display(ais_shape, Standard_True);
	/*
	* //get default drawer from context
		Handle(Prs3d_Drawer) drawer = myContext->DefaultDrawer();

		//set faces edges visible
		drawer->SetFaceBoundaryDraw(true);
	*/
}
