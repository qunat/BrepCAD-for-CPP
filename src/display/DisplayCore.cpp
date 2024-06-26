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
using namespace std;



Display_Core::Display_Core(OccView*parent)
{
	Context =parent->getContext();
	TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(100.0, 100.0, 100.0).Shape();
	Quantity_Color color(0.5,0.6,0.7, Quantity_TOC_RGB) ;
	/*
	// 创建 STEP 文件读取器
	STEPControl_Reader reader;

	// 加载 STEP 文件
	IFSelect_ReturnStatus status = reader.ReadFile("C:\\Users\\Administrator\\Desktop\\NL-3B 后保雷达支架.stp");

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
	*/
	Read_step_file_with_names_colors1("C:\\Users\\Administrator\\Desktop\\078.505.9.0100.00.stp");
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

void Display_Core::Read_step_file_with_names_colors1(string filename)
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

