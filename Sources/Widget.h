#pragma once


/*---- QT Includes ----*/
#include <QGridLayout>
#include <QLCDNumber>
#include <QPushButton>
#include <QSlider>
#include <QWidget>

#include "ui_Widget.h"

//Forward declaration
class View2D;
class View3D;

//-------------------------------------------------------------------------------------------------------------------
/*!	\class	Widget
*	\brief	Widget used for the glenoid fitting validation
*
*	\note	Copy constructor and assignment operator are purposely not implemented.
*	
*	\file	Widget.h
*///-----------------------------------------------------------------------------------------------------------------
class Widget : public QWidget
{
	Q_OBJECT
	public:
		//-------------------------------------------------------------------------------------------------------------------
		/*!	\brief	Constructor	
		*///-----------------------------------------------------------------------------------------------------------------
		Widget(QWidget * p_parent = NULL);

		//-------------------------------------------------------------------------------------------------------------------
		/*!	\brief	Virtual destructor	
		*///-----------------------------------------------------------------------------------------------------------------
		~Widget();


		Widget( const Widget & ) = delete;					//! Purposely deleted
		Widget & operator=(const Widget & ) = delete;		//! Purposely deleted

	private slots:
		void slotSelectDir();
		void slotSegmentData();

	private:
		Ui::Widget m_ui;



};

