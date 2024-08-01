/*
*    Copyright (c) 2013 eryar All Rights Reserved.
*
*           File : DxfReader.cpp
*         Author : eryar@163.com
*           Date : 2013-12-22 14:40
*        Version : 1.0v
*
*    Description : Use dxflib to import entities from DXF to OpenCascade.
*
*      Key Words : OpenCascade, dxflib, DXF, DataExchange
*                  
*/

#include "DxfReader.h"

//#include <Degrees.hxx>

#include <gp_Circ.hxx>
#include <gp_Elips.hxx>

#include <GC_MakeSegment.hxx>
#include <GC_MakeCircle.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeEllipse.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>a
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <math.h>


DxfReader::DxfReader(const std::string& fileName)
: mDxf(new DL_Dxf())
{
    mBuilder.MakeCompound(mShape);

    mDxf->in(fileName, this);  
}

DxfReader::~DxfReader(void)
{
}

const TopoDS_Shape& DxfReader::GetShape() const
{
    return mShape;
}

void DxfReader::addPoint(const DL_PointData& point)
{
    mBuilder.Add(mShape, BRepBuilderAPI_MakeVertex(gp_Pnt(point.x, point.y, point.z)));
}

void DxfReader::addLine(const DL_LineData& line)
{
    Handle_Geom_Curve theSegment = GC_MakeSegment(gp_Pnt(line.x1, line.y1, line.z1), gp_Pnt(line.x2, line.y2, line.z2)).Value();

    mBuilder.Add(mShape, BRepBuilderAPI_MakeEdge(theSegment));
}

void DxfReader::addArc(const DL_ArcData &arc)
{
    gp_Circ theCircle;
    theCircle.SetRadius(arc.radius);
    theCircle.SetLocation(gp_Pnt(arc.cx, arc.cy, arc.cz));

    Handle_Geom_Curve theArc = GC_MakeArcOfCircle(theCircle, DEG(arc.angle1), DEG(arc.angle2), false).Value();

    mBuilder.Add(mShape, BRepBuilderAPI_MakeEdge(theArc));
}

void DxfReader::addCircle(const DL_CircleData& circle)
{
    gp_Circ aCircle;
    aCircle.SetRadius(circle.radius);
    aCircle.SetLocation(gp_Pnt(circle.cx, circle.cy, circle.cz));

    Handle_Geom_Curve theCircle = GC_MakeCircle(aCircle).Value();

    mBuilder.Add(mShape, BRepBuilderAPI_MakeEdge(theCircle));
}

void DxfReader::addEllipse(const DL_EllipseData& ellipse)
{
}

void DxfReader::addPolyline(const DL_PolylineData& polyline)
{
}

void DxfReader::addVertex(const DL_VertexData& vertex)
{
}

void DxfReader::addSpline(const DL_SplineData& spline)
{
}

void DxfReader::addKnot(const DL_KnotData& knot)
{
}

void DxfReader::addControlPoint(const DL_ControlPointData& cp)
{
}

void DxfReader::add3dFace(const DL_3dFaceData& face)
{
    BRepBuilderAPI_MakePolygon polygon(
        gp_Pnt(face.x[0], face.y[0], face.z[0]), 
        gp_Pnt(face.x[1], face.y[1], face.z[1]),
        gp_Pnt(face.x[2], face.y[2], face.z[2]), 
        gp_Pnt(face.x[3], face.y[3], face.z[3]), true);

    BRepBuilderAPI_MakeFace makeFace(polygon.Wire());

    if (makeFace.IsDone())
    {
        mBuilder.Add(mShape, makeFace.Face());
    }

    //mBuilder.Add(mShape, polygon.Wire());
}

void DxfReader::addSolid(const DL_SolidData &solid)
{
    add3dFace(solid);
}

double DxfReader::DEG(double angle)
{
    double Arc=angle * 180 / pi1;
    return Arc;
}
