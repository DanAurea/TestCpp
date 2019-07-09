#pragma once


/*---- ITK Includes ----*/
#include <itkImage.h>

/*---- QT Includes ----*/
#include <QList>
#include <QString>

/*---- VTK Includes ----*/
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>

//Define image type
typedef itk::Image<signed short, 2> ShortImageType;
typedef itk::Image<unsigned char, 2> UCharImageType;


//-------------------------------------------------------------------------------------------------------------------
/*!	\class	ApplicationData
*	\brief	Stored the application data using SingletonPattern
*
*	\note	Copy constructor and assignment operator are purposely not implemented.
*	
*	\file	ApplicationData.h
*///-----------------------------------------------------------------------------------------------------------------
class ApplicationData
{
	public:
		//-------------------------------------------------------------------------------------------------------------------
		/*!	\brief	Get instance: if NULL create it	
		*///-----------------------------------------------------------------------------------------------------------------
		static ApplicationData * getInstance()
		{
			//Create instance
			if (!m_instance)
				m_instance = new ApplicationData();
			return m_instance;
		}

		//-------------------------------------------------------------------------------------------------------------------
		/*!	\brief	Kill the instance if already created	
		*///-----------------------------------------------------------------------------------------------------------------
		static void kill()
		{
			//Delete instance
			if (m_instance)
			{
				delete m_instance;
				m_instance = 0;
			}
		}

		//-------------------------------------------------------------------------------------------------------------------
		/*!	\brief	Method to load a directory
		*///-----------------------------------------------------------------------------------------------------------------
		void loadDirectory(const QString & p_dirPath);

		//-------------------------------------------------------------------------------------------------------------------
		/*!	\brief	Method to segment the data
		*///-----------------------------------------------------------------------------------------------------------------
		void segmentData();

		//! Getters
		const QList<ShortImageType::Pointer> & getRawImages() const { return m_rawImages; }
		vtkSmartPointer<vtkImageData> getRawVTKData() { return m_rawVTKData; }
		const int & getDimensionZ() const { return m_dimensionZ; }
		const double & getSpacingZ() const { return m_spacingZ; }
		//vtkSmartPointer<vtkPolyData> getMesh() { return m_mesh; }
		vtkPolyData * getScapula() { return m_scapula; }
		vtkPolyData * getHumerus() { return m_humerus; }


		ApplicationData(const ApplicationData &) = delete;
		ApplicationData & operator=(const ApplicationData &) = delete;

	private:
		static ApplicationData *m_instance;
		
		QString m_dirPath;								//! The path to the current processed directory
		int m_dimensionX;
		int m_dimensionY;
		int m_dimensionZ;		//! The dimensions of the loaded 

		double m_spacingX;
		double m_spacingY;
		double m_spacingZ;		//! The spacing for the loaded scan
		
		QList<ShortImageType::Pointer> m_rawImages;		//! Store the raw images

		vtkSmartPointer<vtkImageData> m_rawVTKData;		//! The raw data in VTK format
		vtkPolyData * m_scapula, * m_humerus;

		//-------------------------------------------------------------------------------------------------------------------
		/*!	\brief	Method to segment the data depending on lower and higher threshold
		*///-----------------------------------------------------------------------------------------------------------------
		vtkPolyData * segmentData(const int p_lowerThreshold, const int p_upperThreshold) const;

		//-------------------------------------------------------------------------------------------------------------------
		/*!	\brief	Constructor
		*///-----------------------------------------------------------------------------------------------------------------
		ApplicationData();

		//-------------------------------------------------------------------------------------------------------------------
		/*!	\brief	Destructor
		*///-----------------------------------------------------------------------------------------------------------------
		~ApplicationData();
	
		
};
