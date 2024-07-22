#pragma once
#include <occView.h>
#include <AIS_Shape.hxx>
#include <TopoDS_Shape.hxx>
#include<AIS_Trihedron.hxx>
#include<AIS_Plane.hxx>
#include <AIS_ViewCube.hxx>
#include<string.h>
#include <any>
#include <variant>
using namespace std;

class shape 
{

public:
	shape(Handle(AIS_Shape) aisShape);
	shape(Handle(AIS_Trihedron) aisrihedron);
	shape(Handle(AIS_Plane) aisplane);
	shape(Handle(AIS_ViewCube) aisviewcube);

	~shape();
	void * Value();

public:
	Handle(AIS_Shape) AisShape;
	Handle(AIS_Trihedron) Aisrihedron;
	Handle(AIS_Plane) AisPlane;
	Handle(AIS_ViewCube) AISViewCube;
	

	shape() = default;
};


class DisplayCore
{
public:
	Handle(AIS_InteractiveContext) Context;
public:
	DisplayCore(OccView*parent);
	~DisplayCore();
	void DisplayShape(TopoDS_Shape shape, Quantity_Color color, double transparency,Graphic3d_MaterialAspect material = Graphic3d_NOM_STEEL,Standard_Boolean theToUpdateViewer=true);
	//void DisplayColorShape(TopoDS_Shape shape, Quantity_Color color, double transparency, Graphic3d_MaterialAspect material = Graphic3d_NOM_STEEL,Standard_Boolean theToUpdateViewer = true);
	void Read_step_file_with_names_colors1(string filename);
	int Read_step_file(string filename);
	int Displaytriehedron();
	int Displayplane();
	int DisplayCube();

public:
	map<string, shape*> ShapeManeger;
};