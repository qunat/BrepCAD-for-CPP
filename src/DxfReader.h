/*
*    Copyright (c) 2013 eryar All Rights Reserved.
*
*           File : DxfReader.h
*         Author : eryar@163.com
*           Date : 2013-12-22 14:40
*        Version : 1.0v
*
*    Description : Use dxflib to import entities from DXF to OpenCascade.
*
*      Key Words : OpenCascade, dxflib, DXF, DataExchange
*                  
*/

#pragma once

#define WNT
#include <TopoDS_Shape.hxx>
#include <BRep_Builder.hxx>

#include <memory>

#include "dl_dxf.h"
#include "dl_creationadapter.h"
#include <math.h>
double pi1 = M_PI;

/**
* @breif Facade dxflib for OpenCascade DataExchange with DXF.
*/
class DxfReader : public DL_CreationAdapter
{
public:
    /**
    * @brief constructor.
    * @param fileName [in] dxf file name with path.
    */
    DxfReader(const std::string& fileName);
    ~DxfReader(void);

    /**
    * @brief Get the shape of the dxf.
    * @return OpenCascade topology shape.
    */
    const TopoDS_Shape& GetShape(void) const;

public:
    virtual void addPoint(const DL_PointData&);
    virtual void addLine(const DL_LineData& line);
    virtual void addArc(const DL_ArcData& arc);
    virtual void addCircle(const DL_CircleData& circle);
    virtual void addEllipse(const DL_EllipseData&);

    virtual void addPolyline(const DL_PolylineData& polyline);
    virtual void addVertex(const DL_VertexData&);

    virtual void addSpline(const DL_SplineData&);
    virtual void addKnot(const DL_KnotData&);
    virtual void addControlPoint(const DL_ControlPointData&);

    virtual void add3dFace(const DL_3dFaceData&);
    virtual void addSolid(const DL_SolidData& solid);
    double DEG(double angle);

private:
    std::auto_ptr<DL_Dxf> mDxf;

    TopoDS_Compound mShape;

    BRep_Builder mBuilder;
};