#include "PartSolution.h"
// occview include
#include<occView.h>
#include<QtCore/Qstring>
#include<QFileDialog>

using namespace std;

static Handle(Graphic3d_GraphicDriver)& GetGraphicDriver()
{
	static Handle(Graphic3d_GraphicDriver) aGraphicDriver;
	return aGraphicDriver;
}

PartSolution::PartSolution(QWidget *parent)
    : QMainWindow(parent)
{

	myOccView = new OccView(this);
	myOccView->reset();

	myOccView->show();

	setCentralWidget(myOccView);
}
void PartSolution::open_part(void)
{

	


}

void PartSolution::createActions(void)
{
	

}
PartSolution::~PartSolution()
{}
