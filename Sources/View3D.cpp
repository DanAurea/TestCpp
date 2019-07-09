//-------------------------------------------------------------------------------------------------------------------
/*!	\brief 	Implementation of class View3D
*	\file	View3D.cpp
*///-----------------------------------------------------------------------------------------------------------------

#include "View3D.h"

/*---- Internal Includes ----*/
#include "ApplicationData.h"

/*---- VTK Includes ----*/
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>

#include <QDebug>

//-----------------------------------------------------------------------------------------------------------------
View3D::View3D(QWidget * p_parent)
: QVTKWidget(p_parent), m_renderer(vtkSmartPointer<vtkRenderer>::New())
//-----------------------------------------------------------------------------------------------------------------
{
	m_ui.setupUi(this);
}

//-----------------------------------------------------------------------------------------------------------------
View3D::~View3D()
//-----------------------------------------------------------------------------------------------------------------
{
	//Not necessary to delete children View3D, Qt will do it automatically
}

//-----------------------------------------------------------------------------------------------------------------
void View3D::initData()
//-----------------------------------------------------------------------------------------------------------------
{
	// Clear contents from m_renderer so any content added previously won't be displayed.
	m_renderer->RemoveAllViewProps();

	//Add renderer
	m_ui.view->GetRenderWindow()->AddRenderer(m_renderer);

	//Set background color
	m_renderer->SetBackground(1, 1, 1);

	//Render
	m_renderer->Render();
	update();

	//Create mapper and actor for Scapula Mesh - Ex9 - VTK
	//Use mapper->ScalarVisibilityOff() to take into account actor color
	vtkSmartPointer<vtkPolyDataMapper> mapperScapula = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapperScapula->ScalarVisibilityOff();
	mapperScapula->SetInputData(ApplicationData::getInstance()->getScapula());
	vtkSmartPointer<vtkActor> actorScapula = vtkSmartPointer<vtkActor>::New();
	actorScapula->SetMapper(mapperScapula);
	actorScapula->GetProperty()->SetColor(0.9, 0.5, 0.4);

	//Create mapper and actor for Humerus Mesh - Ex9 - VTK
	//Use mapper->ScalarVisibilityOff() to take into account actor color
	vtkSmartPointer<vtkPolyDataMapper> mapperHumerus = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapperHumerus->ScalarVisibilityOff();
	mapperHumerus->SetInputData(ApplicationData::getInstance()->getHumerus());
	vtkSmartPointer<vtkActor> actorHumerus = vtkSmartPointer<vtkActor>::New();
	actorHumerus->SetMapper(mapperHumerus);
	actorHumerus->GetProperty()->SetColor(0.55, 0.63, 0.8);

	//Add actor in renderer
	m_renderer->AddActor(actorScapula);
	m_renderer->AddActor(actorHumerus);

	m_renderer->ResetCamera();
}
