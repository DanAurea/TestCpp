//-------------------------------------------------------------------------------------------------------------------
/*!	\brief 	Implementation of class Widget
*	\file	Widget.cpp
*///-----------------------------------------------------------------------------------------------------------------

#include "Widget.h"

/*---- Internal Includes ----*/
#include "ApplicationData.h"
#include "View2D.h"
#include "View3D.h"

/*---- QT Includes ----*/
#include <QFileDialog>
#include <QHBoxLayout>
#include <qdebug.h>
//-----------------------------------------------------------------------------------------------------------------
Widget::Widget(QWidget * p_parent) : QWidget(p_parent)
//-----------------------------------------------------------------------------------------------------------------
{
	m_ui.setupUi(this);

	//Connect loadButton
	QObject::connect(m_ui.loadButton, SIGNAL(clicked()), this, SLOT(slotSelectDir()));
}

//-----------------------------------------------------------------------------------------------------------------
Widget::~Widget()
//-----------------------------------------------------------------------------------------------------------------
{
}


//-----------------------------------------------------------------------------------------------------------------
void Widget::slotSelectDir()
//-----------------------------------------------------------------------------------------------------------------
{
	QString dirPath = ""; //Replace to select directory with images

	// Open a filedialog box that allow selection of directory only and doesn't resolve symlinks (Unix only).
	dirPath = QFileDialog::getExistingDirectory(this, tr("Select a directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	
	if(dirPath != "")
	{
		ApplicationData::getInstance()->loadDirectory(dirPath);

		//Connect segmentButton
		QObject::connect(m_ui.segmentButton, SIGNAL(clicked()), this, SLOT(slotSegmentData()));

		//Initialize and show 2D view
		m_ui.view2D->initData();
	}
}

//-----------------------------------------------------------------------------------------------------------------
void Widget::slotSegmentData()
//-----------------------------------------------------------------------------------------------------------------
{
	//Disconnect segment button
	QObject::disconnect(m_ui.segmentButton, SIGNAL(clicked()), this, SLOT(slotSegmentData()));
	
	//Segment data
	ApplicationData::getInstance()->segmentData();

	//Init and show 3D view
	m_ui.view3D->initData();
}
