/*
*    Copyright (c) 2018 Shing Liu All Rights Reserved.
*
*           File : OccView.cpp
*         Author : Shing Liu(eryar@163.com)
*           Date : 2018-01-08 21:00
*        Version : OpenCASCADE7.2.0 & Qt5.7.1
*
*    Description : Qt widget for OpenCASCADE viewer.
*/

#include <OpenGl_GraphicDriver.hxx>

#include "occView.h"

#include <QMenu>
#include <QMouseEvent>
#include <QRubberBand>
#include <QStyleFactory>
#include<QtCore/qnamespace.h>
#include <V3d_View.hxx>

#include <Aspect_Handle.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <QDebug>

#ifdef WNT
    #include <WNT_Window.hxx>
#elif defined(__APPLE__) && !defined(MACOSX_USE_GLX)
    #include <Cocoa_Window.hxx>
#else
    #undef Bool
    #undef CursorShape
    #undef None
    #undef KeyPress
    #undef KeyRelease
    #undef FocusIn
    #undef FocusOut
    #undef FontChange
    #undef Expose
    #include <Xw_Window.hxx>
#endif


static Handle(Graphic3d_GraphicDriver)& GetGraphicDriver()
{
  static Handle(Graphic3d_GraphicDriver) aGraphicDriver;
  return aGraphicDriver;
}

OccView::OccView(QWidget* parent )
    : QWidget(parent),
    myXmin(0),
    myYmin(0),
    myXmax(0),
    myYmax(0),
    myCurrentMode(CurAction3d_DynamicRotation),
    myDegenerateModeIsOn(Standard_True),
    myRectBand(NULL)
{
    // No Background
    setBackgroundRole( QPalette::NoRole );

    // set focus policy to threat QContextMenuEvent from keyboard  
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    // Enable the mouse tracking, by default the mouse tracking is disabled.
    setMouseTracking( true );
    //set course  icons
    course["pan"] = QPixmap(":/icon/icons/cursor-pan.png");
    course["pan"].scaled(QSize(3,3),Qt::KeepAspectRatio);

    course["rotate"] = QPixmap(":/icon/icons/cursor-rotate.png");
    course["rotate"].scaled(QSize(3, 3), Qt::KeepAspectRatio);

    course["scale"] = QPixmap(":/icon/icons/ccursor-magnify.png");
    course["scale"].scaled(QSize(3, 3), Qt::KeepAspectRatio);

    //setCursor(QCursor(course["pan"]));
   // connect(this, SIGNAL(mySignal(int)), this, SLOT(mySlot()));
    init();
}

void OccView::init()
{
    // Create Aspect_DisplayConnection
    Handle(Aspect_DisplayConnection) aDisplayConnection =
            new Aspect_DisplayConnection();

    // Get graphic driver if it exists, otherwise initialise it
    if (GetGraphicDriver().IsNull())
    {
        GetGraphicDriver() = new OpenGl_GraphicDriver(aDisplayConnection);
    }

    // Get window handle. This returns something suitable for all platforms.
    WId window_handle = (WId) winId();

    // Create appropriate window for platform
    #ifdef WNT
        Handle(WNT_Window) wind = new WNT_Window((Aspect_Handle) window_handle);
    #elif defined(__APPLE__) && !defined(MACOSX_USE_GLX)
        Handle(Cocoa_Window) wind = new Cocoa_Window((NSView *) window_handle);
    #else
        Handle(Xw_Window) wind = new Xw_Window(aDisplayConnection, (Window) window_handle);
    #endif

    // Create V3dViewer and V3d_View

    myViewer = new V3d_Viewer(GetGraphicDriver(), Standard_ExtString("viewer3d"));
    myViewer->SetDefaultLights();
    myViewer->SetLightOn();
    myViewer->SetDefaultViewProj(V3d_XposYposZpos);
    myViewer->SetDefaultLights();
    myViewer->SetLightOn();
    myView = myViewer->CreateView();

    myView->SetWindow(wind);
    if (!wind->IsMapped()) wind->Map();

    // Create AISInteractiveContext
    myContext = new AIS_InteractiveContext(myViewer);

    // Set up lights etc

    myViewer->SetDefaultLights();
    myViewer->SetLightOn();
	double R1,G1,B1,R2,G2,B2;

	//R1 = 212/255.0, G1 = 212/255.0, B1 = 212/255.0;
	//R2 = 128/255.0, G2 = 128/255.0, B2 = 128/255.0;

    R1 = 135 / 255.0, G1 = 150 / 255.0, B1 = 170 / 255.0;
    R2 = 195 / 255.0, G2 = 200 / 255.0, B2 = 210 / 255.0;
    //R1 = 233 / 255.0, G1 = 235 / 255.0, B1 = 241 / 255.0;
   //R2 = 250 / 255.0, G2 = 251 / 255.0, B2 = 210 / 255;

	
	Quantity_Color background_1(R1,G1,B1, Quantity_TOC_RGB);
	Quantity_Color background_2(R2,G2,B2, Quantity_TOC_RGB);

    //myView->SetBackgroundColor(Quantity_NOC_BLACK);
	//背景渐变色设置

	myView->SetBgGradientColors(background_1, background_2, Aspect_GFM_VER, Standard_True);
    myView->SetBackgroundImage("F:\\宁波莱布尼茨项目\\BrepCAD\\src\\Images\\Cloudy.png",Aspect_FM_STRETCH,  true);
    myView->MustBeResized();
    myView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.2, V3d_ZBUFFER);

    myContext->SetDisplayMode(1, Standard_True);
}

const Handle(AIS_InteractiveContext)& OccView::getContext() const
{
    return myContext;
}

/*!
Get paint engine for the OpenGL viewer. [ virtual public ]
*/
QPaintEngine* OccView::paintEngine() const
{
    return 0;
}

void OccView::paintEvent( QPaintEvent* /*theEvent*/ )
{
    myView->Redraw();
}

void OccView::resizeEvent( QResizeEvent* /*theEvent*/ )
{
    if( !myView.IsNull() )
    {
        myView->MustBeResized();
    }
}

void OccView::fitAll( void )
{
    myView->FitAll();
    myView->ZFitAll();
    myView->Redraw();
}

void OccView::reset( void )
{
    myView->Reset();
}

void OccView::pan( void )
{
    myCurrentMode = CurAction3d_DynamicPanning;
}

void OccView::zoom( void )
{
    myCurrentMode = CurAction3d_DynamicZooming;
}

void OccView::rotate( void )
{
    myCurrentMode = CurAction3d_DynamicRotation;
}

void OccView::mousePressEvent( QMouseEvent* theEvent )
{		
    emit mySignal(42);
    if (theEvent->button() == Qt::LeftButton)
    {
        onLButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());

    }
    else if (theEvent->button() == Qt::MidButton && !(theEvent->buttons() & Qt::RightButton))
    {
            this->setCursor(QCursor(course["rotate"]));
            onMButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
    }
    else if (theEvent->button() == Qt::RightButton && !(theEvent->buttons() & Qt::MidButton))
    {
            onRButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
    }
}	

void OccView::mouseReleaseEvent( QMouseEvent* theEvent )
{
    
    if (theEvent->button() == Qt::LeftButton)
    {
        onLButtonUp(theEvent->buttons() | theEvent->modifiers(), theEvent->pos());
    }
    else if (theEvent->button() == Qt::MidButton)
    {
        onMButtonUp(theEvent->buttons() | theEvent->modifiers(), theEvent->pos());
    }
    else if (theEvent->button() == Qt::RightButton)
    {
        onRButtonUp(theEvent->buttons() | theEvent->modifiers(), theEvent->pos());
    }

    this->setCursor(Qt::ArrowCursor);
}

void OccView::mouseMoveEvent( QMouseEvent * theEvent )
{
    
    onMouseMove(theEvent, theEvent->pos());
}

void OccView::wheelEvent( QWheelEvent * theEvent )
{
    onMouseWheel(theEvent->buttons(), theEvent->delta(), theEvent->pos());
}

void OccView::onLButtonDown( const int /*theFlags*/, const QPoint thePoint )
{
    // Save the current mouse coordinate in min.
    myXmin = thePoint.x();
    myYmin = thePoint.y();
    myXmax = thePoint.x();
    myYmax = thePoint.y();
	
}

void OccView::onMButtonDown( const int /*theFlags*/, const QPoint thePoint )
{
    // Save the current mouse coordinate in min.
    myXmin = thePoint.x();
    myYmin = thePoint.y();
    myXmax = thePoint.x();
    myYmax = thePoint.y();
    mid_button_press_x= thePoint.x();
    mid_button_press_y= thePoint.y();


    if (myCurrentMode == CurAction3d_DynamicRotation)
    {
        myView->StartRotation(thePoint.x(), thePoint.y());
    }
}

void OccView::onRButtonDown( const int /*theFlags*/, const QPoint thePoint )
{
	// Save the current mouse coordinate in min.
	myXmin = thePoint.x();
	myYmin = thePoint.y();
	myXmax = thePoint.x();
	myYmax = thePoint.y();
}

void OccView::onMouseWheel( const int /*theFlags*/, const int theDelta, const QPoint thePoint )
{
    double aFactor = 5;
    Standard_Integer aX = thePoint.x();
    Standard_Integer aY = thePoint.y();
    

    if (theDelta < 0)//通过重写设置显示比例进行缩放
    {
        zoom_factor=1+aFactor/100;
    }
    else
    {
        zoom_factor = 1 - aFactor / 100;
       
    }
    myView->SetZoom(zoom_factor);
    //myView->Zoom(thePoint.x(), thePoint.y(), aX, aY);
}

void OccView::addItemInPopup( QMenu* /*theMenu*/ )
{
}

void OccView::popup( const int /*x*/, const int /*y*/ )
{
}

void OccView::onLButtonUp( const int theFlags, const QPoint thePoint )
{
    // Hide the QRubberBand
    if (myRectBand)
    {
        myRectBand->hide();
    }

    // Ctrl for multi selection.
    if (thePoint.x() == myXmin && thePoint.y() == myYmin)
    {
        if (theFlags & Qt::ControlModifier)
        {
            multiInputEvent(thePoint.x(), thePoint.y());
        }
        else
        {
            inputEvent(thePoint.x(), thePoint.y());
        }
    }

}

void OccView::onMButtonUp( const int /*theFlags*/, const QPoint thePoint )
{
    if (thePoint.x() == myXmin && thePoint.y() == myYmin)

    {
        if (thePoint.x()!=mid_button_press_x || thePoint.y()!=mid_button_press_y )
            panByMiddleButton(thePoint);
       
    }
}

void OccView::onRButtonUp( const int /*theFlags*/, const QPoint thePoint )
{
    popup(thePoint.x(), thePoint.y());
}

void OccView::onMouseMove(QMouseEvent* event, const QPoint thePoint )
{   
    
    // Draw the rubber band.
    if (event->buttons()==Qt::LeftButton)
    {
        drawRubberBand(myXmin, myYmin, thePoint.x(), thePoint.y());

        dragEvent(thePoint.x(), thePoint.y());
    }
	// Pan
	if (event->buttons() ==( Qt::RightButton|Qt::MidButton))
	{       
         this->setCursor(QCursor(course["pan"]));
		 myView->Pan(thePoint.x() - myXmax, myYmax - thePoint.y());
         myXmax = thePoint.x();
         myYmax = thePoint.y();
	}

    // Ctrl for multi selection.
    if (event->buttons() & Qt::ControlModifier)
    {
        multiMoveEvent(thePoint.x(), thePoint.y());
    }
    else
    {
        moveEvent(thePoint.x(), thePoint.y());
    }

    // Middle button.
    if (event->buttons()==Qt::MidButton && (event->buttons() != Qt::RightButton))
    {

        switch (myCurrentMode)
        {
        case CurAction3d_DynamicRotation:
            myView->Rotation(thePoint.x(), thePoint.y());
            break;

        case CurAction3d_DynamicZooming:
            myView->Zoom(myXmin, myYmin, thePoint.x(), thePoint.y());
            break;

        case CurAction3d_DynamicPanning:
            myView->Pan(thePoint.x() - myXmax, myYmax - thePoint.y());
            myXmax = thePoint.x();
            myYmax = thePoint.y();
            break;

         default:
            break;
        }
    }

}

void OccView::dragEvent( const int x, const int y )
{
    myContext->Select(myXmin, myYmin, x, y, myView, Standard_True);

    emit selectionChanged();
}

void OccView::multiDragEvent( const int x, const int y )
{
    myContext->ShiftSelect(myXmin, myYmin, x, y, myView, Standard_True);

    emit selectionChanged();

}

void OccView::inputEvent( const int x, const int y )
{
    Q_UNUSED(x);
    Q_UNUSED(y);

    myContext->Select(Standard_True);

    emit selectionChanged();
}

void OccView::multiInputEvent( const int x, const int y )
{
    Q_UNUSED(x);
    Q_UNUSED(y);

    myContext->ShiftSelect(Standard_True);

    emit selectionChanged();
}

void OccView::moveEvent( const int x, const int y )
{
    myContext->MoveTo(x, y, myView, Standard_True);
}

void OccView::multiMoveEvent( const int x, const int y )
{
    myContext->MoveTo(x, y, myView, Standard_True);
}

void OccView::drawRubberBand( const int minX, const int minY, const int maxX, const int maxY )
{
    QRect aRect;

    // Set the rectangle correctly.
    (minX < maxX) ? (aRect.setX(minX)) : (aRect.setX(maxX));
    (minY < maxY) ? (aRect.setY(minY)) : (aRect.setY(maxY));

    aRect.setWidth(abs(maxX - minX));
    aRect.setHeight(abs(maxY - minY));

    if (!myRectBand)
    {
        myRectBand = new QRubberBand(QRubberBand::Rectangle, this);

        // setStyle is important, set to windows style will just draw
        // rectangle frame, otherwise will draw a solid rectangle.
        myRectBand->setStyle(QStyleFactory::create("windows"));
    }

    myRectBand->setGeometry(aRect);
    myRectBand->show();
}

void OccView::panByMiddleButton( const QPoint& thePoint )
{
    Standard_Integer aCenterX = 0;
    Standard_Integer aCenterY = 0;

    QSize aSize = size();

    aCenterX = aSize.width() / 2;
    aCenterY = aSize.height() / 2;

    myView->Pan(aCenterX - thePoint.x(), thePoint.y() - aCenterY);
}

void OccView::mySlot()
{
    qDebug() << "The value is:";
}