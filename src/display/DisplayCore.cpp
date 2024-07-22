#include<display/DisplayCore.h>
#include <occView.h>
#include <AIS_InteractiveContext.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <AIS_Shape.hxx>
#include<Prs3d_LineAspect.hxx>
#include <Quantity_Color.hxx>
#include <STEPControl_Reader.hxx> 
#include <TopoDS_Shape.hxx> 
#include <BRepTools.hxx> 
#include<string.h>
//#include <TDocStd_Document.hxx>
//#include<XCAFDoc/XCAFDoc_ShapeTool.hxx>
//#include<XCAFDoc/XCAFDoc_ColorTool.hxx>
//#include<XCAFDoc/XCAFDoc_LayerTool.hxx>
//#include<XCAFDoc/XCAFDoc_MaterialTool.hxx>
//#include<XCAFDoc/XCAFDoc_DocumentTool.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <AIS_Trihedron.hxx>

#include <STEPControl_Reader.hxx>
#include <TopoDS_Shape.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Face.hxx>
#include <Quantity_Color.hxx>
//#include <XCAFDoc_ShapeTool.hxx>
//#include <XCAFDoc_ColorTool.hxx>
//#include <TDocStd_Document.hxx>
//#include <XCAFApp_Application.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <iostream>
#include <QFileDialog>
#include <qmessagebox.h>
#include <Geom_Axis2Placement.hxx>
#include <gp.hxx>
#include <Geom_Plane.hxx>
#include <Prs3d_LineAspect.hxx>
#include<Aspect_TypeOfLine.hxx>
#include <Prs3d_PlaneAspect.hxx>

using namespace std;



DisplayCore::DisplayCore(OccView*parent)
{
	Context =parent->getContext();
	Displaytriehedron();
	Displayplane();
	DisplayCube();
	TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(100.0, 100.0, 100.0).Shape();
	Quantity_Color color(0.5,0.6,0.7, Quantity_TOC_RGB) ;

}
void DisplayCore::DisplayShape(TopoDS_Shape shape, Quantity_Color color, double transparency, Graphic3d_MaterialAspect material ,Standard_Boolean theToUpdateViewer)
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

void DisplayCore::Read_step_file_with_names_colors1(string filename)
{
	
	list<int> assemble_relation_list;
	map<TopoDS_Shape, string> output_shapes;
	//create an handle to a document
	//TDocStd_Document doc = TDocStd_Document(TCollection_ExtendedString("pythonocc-doc"));

	//Get root assembly
	//XCAFDoc_ShapeTool shape_tool = XCAFDoc_ShapeTool();
	//XCAFDoc_ColorTool color_tool = XCAFDoc_ColorTool();
	//XCAFDoc_LayerTool layer_tool = XCAFDoc_LayerTool();
	
}

int DisplayCore::Read_step_file(string filename)
{

	TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(100.0, 100.0, 100.0).Shape();
	Quantity_Color color(0.5, 0.6, 0.7, Quantity_TOC_RGB);
	

	// 创建 STEP 文件读取器
	STEPControl_Reader reader;

	// 加载 STEP 文件
	IFSelect_ReturnStatus status = reader.ReadFile(filename.c_str());

	// 检查文件是否成功加载
	if (status != IFSelect_RetDone) {
		std::cerr << "Error: Unable to read the STEP file." << std::endl;

	}

	// 将 STEP 文件内容转换为 OpenCASCADE 的内部表示
	Standard_Integer nbr = reader.TransferRoots();
	if (nbr == 0) {
		std::cerr << "Error: No shapes found in the STEP file." << std::endl;

	}

	// 获取转换后的形状
	TopoDS_Shape shape = reader.OneShape();
	this->DisplayShape(shape, color, 1);
	return 0;
}

int DisplayCore::Displaytriehedron()
{
	Handle(Geom_Axis2Placement) axis = new Geom_Axis2Placement(gp::XOY());
	Handle(AIS_Trihedron) triehedron = new AIS_Trihedron(axis);
	triehedron->SetXAxisColor(Quantity_Color(Quantity_NOC_RED));
	triehedron->SetYAxisColor(Quantity_Color(Quantity_NOC_GREEN));
	triehedron->SetAxisColor(Quantity_Color(Quantity_NOC_BLUE1));
	triehedron->SetTextColor(Quantity_Color(Quantity_NOC_DARKGOLDENROD));
	Context->Display(triehedron, 0, 3, true);
	ShapeManeger["坐标系"] = new shape(triehedron);
	//Handle(drawer) = triehedron.Attributes();
	return 0;
}

int DisplayCore::Displayplane()
{	
	Handle(Geom_Plane)plane = new Geom_Plane(gp_Pnt(0.0, 0.0, 0.0), gp_Dir(0, 1, 0));
	Handle(AIS_Plane)ais_plane_xz = new AIS_Plane(plane, true);
	Handle(Prs3d_PlaneAspect) plan_asp = new Prs3d_PlaneAspect();
	plan_asp->SetPlaneLength(500, 500);
	Handle(Prs3d_LineAspect) line_asp = plan_asp->EdgesAspect();
	line_asp->SetColor(Quantity_Color(Quantity_NOC_GREEN));
	ais_plane_xz->Attributes()->SetPlaneAspect(plan_asp);
	ais_plane_xz->SetTypeOfSensitivity(Select3D_TOS_INTERIOR);
	Context->Display(ais_plane_xz, 0, 3, true);
	ShapeManeger["基准面XZ"] = new shape(ais_plane_xz);

	plane = new Geom_Plane(gp_Pnt(0.0, 0.0, 0.0), gp_Dir(1, 0, 0));
	Handle(AIS_Plane)ais_plane_yz = new AIS_Plane(plane, true);
	ais_plane_yz->Attributes()->SetPlaneAspect(plan_asp);
	ais_plane_yz->SetTypeOfSensitivity(Select3D_TOS_INTERIOR);
	Context->Display(ais_plane_yz, 0, 3, true);
	ShapeManeger["基准面YZ"] = new shape(ais_plane_yz);

	plane = new Geom_Plane(gp_Pnt(0.0, 0.0, 0.0), gp_Dir(0, 0, 1));
	Handle(AIS_Plane)ais_plane_xy = new AIS_Plane(plane);
	ais_plane_xy->Attributes()->SetPlaneAspect(plan_asp);
	ais_plane_xy->SetTypeOfSensitivity(Select3D_TOS_INTERIOR);
	Context->Display(ais_plane_xy, 0, 3, true);
	ShapeManeger["基准面XY"] = new shape(ais_plane_xy);
	return 0;
}

int DisplayCore::DisplayCube()
{	
	Handle(AIS_ViewCube) aisviewcube= new AIS_ViewCube();
	ShapeManeger["cube"] = new shape(aisviewcube);
	ShapeManeger["cube"]->AISViewCube->SetTransformPersistence(Graphic3d_TMF_TriedronPers, gp_Pnt(1, 1, 100));
	Context->Display(ShapeManeger["cube"]->AISViewCube, true);
	return 0;
}







shape::shape(Handle(AIS_Shape) aisShape)
{
	AisShape = aisShape;
}

shape::shape(Handle(AIS_Trihedron) aisrihedron)
{
	Aisrihedron = aisrihedron;
}

shape::shape(Handle(AIS_Plane) aisplane)
{
	AisPlane = aisplane;
}

shape::shape(Handle(AIS_ViewCube) aisviewcube)
{

	AISViewCube = aisviewcube;
}


void* shape::Value()
{
	return NULL;
}

