//-------------------------------------------------------------------------------------------------------------------
/*!	\brief 	Implementation of class ApplicationData
*	\file	ApplicationData.cpp
*///-----------------------------------------------------------------------------------------------------------------

#include "ApplicationData.h"

/*---- ITK Includes ----*/
#include <itkGDCMImageIO.h>
#include <itkImageFileReader.h>
#include <itkImageRegionConstIterator.h>
#include <itkBinaryThresholdImageFilter.h>

/*---- QT Includes ----*/
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QStringList>
#include <QMessageBox>

/*---- VTK Includes ----*/
#include <vtkImageFlip.h>
#include <vtkPointData.h>
#include <vtkShortArray.h>
#include <vtkMarchingCubes.h>
#include <vtkUnsignedCharArray.h>

//Initialize static variable
ApplicationData * ApplicationData::m_instance = 0;

//-----------------------------------------------------------------------------------------------------------------
ApplicationData::ApplicationData() :
		m_scapula(vtkPolyData::New()), m_humerus(vtkPolyData::New()), m_dimensionZ(0), m_spacingZ(0)
//-----------------------------------------------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------------------------------------------
ApplicationData::~ApplicationData()
//-----------------------------------------------------------------------------------------------------------------
{
	m_scapula->Delete();
	m_humerus->Delete();
}

//-----------------------------------------------------------------------------------------------------------------
void ApplicationData::loadDirectory(const QString & p_dirPath)
//-----------------------------------------------------------------------------------------------------------------
{
	m_dirPath = p_dirPath;

	//List files in a directory

	//Create the list of string to store files path
	QStringList filesList;

	// Create an instance of QDir from directory path so the directory contents can be manipulated through Qt library
	QDir directory(p_dirPath);

	// List all files in selected directory with filter files so only files will be listed with entryList()
	filesList = directory.entryList(QDir::Files);


	m_rawImages.clear();

	//Read Dicom Image
	double positionZ = 0.;
	for(int i = 0 ; i < filesList.size() ; ++i)
	{
		//try to read the ITK file and save the image
		try
		{
			qDebug() << "Reading image " << i << "with path:" << filesList[i];
			
			//Try to read the ITK file and save the image - Ex4 et Ex5 ITK - itk::ImageFileReader and itk
			// Initialize an instance of Dicom Image IO
			itk::GDCMImageIO::Pointer gdcmImageIo = itk::GDCMImageIO::New();

			// Initialize an image reader from ITK library
			itk::ImageFileReader<ShortImageType>::Pointer reader = itk::ImageFileReader<ShortImageType>::New();

			reader->SetFileName((m_dirPath + QDir::separator() + filesList[i]).toStdString());
			reader->SetImageIO(gdcmImageIo);
			reader->Update();

			//Save read image in the list: m_rawImages
			m_rawImages.append(reader->GetOutput());
			//For the first two images, we get the image position (0020, 0032).
			//From the difference, we compute the spacingZ
			if(i <= 1)
			{
				std::string currentPosition;
				//Read currentPosition (0020, 0032) using GDCMImageIO - Refer to Ex5 - ITK
				// Read tag value from dicom header at (0020, 0032) --> tag of patient position
				gdcmImageIo->GetValueFromTag(std::string("0020|0032"), currentPosition);
					
				const double currentPositionZ = QString(currentPosition.c_str()).trimmed().split("\\").last().toDouble();
				qDebug() << QString(currentPosition.c_str()) << positionZ;
				if(i == 1)
				{
					m_spacingZ = currentPositionZ - positionZ;
				}
				else positionZ = currentPositionZ;
			}

		}
		//In case of non-image file
		catch(itk::ExceptionObject & ex)
		{
			qDebug() << "Error with file" << filesList[i] << ":" << ex.what();
		}

	}
	
	// Check if there is some images to be processed
	if (m_rawImages.isEmpty())
	{
		QMessageBox::information(NULL, "Loading error", "There is no images that have been loaded.");
		return;
	}

	//Get dimensionX/Y - Refer to Ex1 - ITK
	m_dimensionX = m_rawImages[0]->GetLargestPossibleRegion().GetSize()[0];
	m_dimensionY = m_rawImages[0]->GetLargestPossibleRegion().GetSize()[1];

	//Get spacing X/Y
	m_spacingX = m_rawImages[0]->GetSpacing()[0];
	m_spacingY = m_rawImages[0]->GetSpacing()[1];

	//Get dimensionZ
	m_dimensionZ = m_rawImages.size();
	
	//Build the vtkImageData containing the raw volume - Ex12 VTK
	//VTKImageData should have dimension X, Y and Z as well as SpacingX, Y and Z of the volume
	//Ex12 was a volume of 100x100x100
	m_rawVTKData = vtkSmartPointer<vtkImageData>::New();
	
	m_rawVTKData->SetDimensions(m_dimensionX, m_dimensionY, m_dimensionZ);
	m_rawVTKData->SetSpacing(m_spacingX, m_spacingY, m_spacingZ);
	m_rawVTKData->SetExtent(0, m_dimensionX-1, 0, m_dimensionY-1, 0, m_dimensionZ - 1);

	//Create scalars to fill the vtkImageData
	vtkSmartPointer<vtkShortArray> scalars = vtkSmartPointer<vtkShortArray>::New();
	//Set number of values = number of voxel in 3D volume
	
	scalars->SetNumberOfValues(m_dimensionX * m_dimensionY * m_dimensionZ);

	int offset  = 0;
	//Iterate over all images and fill the scalars
	for(int z = 0 ; z < m_rawImages.size() ; ++z)
	{
		//Create const ITK iterator on each image
		//In the while, set the value of each pixel to the scalars using variable offset
		//Ex3 - ITK
		itk::ImageRegionConstIterator<ShortImageType> localIterator(m_rawImages[z], m_rawImages[z]->GetLargestPossibleRegion());
		localIterator.GoToBegin();

		while (!localIterator.IsAtEnd()) {
			scalars->SetValue(offset, localIterator.Get());
			
			++offset;
;			++localIterator;
		}
	}

	m_rawVTKData->GetPointData()->SetScalars(scalars);

	vtkSmartPointer<vtkImageFlip> flipper = vtkSmartPointer<vtkImageFlip>::New();
	flipper->SetFilteredAxis(1);
	flipper->SetInputData(m_rawVTKData);
	flipper->Update();

	m_rawVTKData = flipper->GetOutput();
}

//-----------------------------------------------------------------------------------------------------------------
void ApplicationData::segmentData()
//-----------------------------------------------------------------------------------------------------------------
{
	//Segment from 2 to 2 for Scapula using segmentData(2, 2)
	m_scapula->Delete();
	m_scapula = segmentData(2, 2);

	//Segment from 0 to 1 for Humerus using segmentData(0, 1)
	m_humerus->Delete();
	m_humerus = segmentData(1, 1);
}

//-----------------------------------------------------------------------------------------------------------------
vtkPolyData * ApplicationData::segmentData(const int p_lowerThreshold, const int p_upperThreshold) const
//-----------------------------------------------------------------------------------------------------------------
{
	QList<UCharImageType::Pointer> binaryImages;

	//Apply threshold on each raw images
	//Same as ITKReader with itk::BinaryThresholdImageFilter
	//See Exo6 ITK
	
	QList<ShortImageType::Pointer> processedImages;

	for(int z = 0 ; z < m_rawImages.size() ; ++z)
	{
		//Create thresholder and save outpout in binaryImages
		itk::BinaryThresholdImageFilter<ShortImageType, ShortImageType>::Pointer thresholder = itk::BinaryThresholdImageFilter<ShortImageType, ShortImageType>::New();
		thresholder->SetInput(m_rawImages[z]);
		// Set threshold values outside of loop (avoid repetitive call for same parameters)
		thresholder->SetLowerThreshold(p_lowerThreshold);
		thresholder->SetUpperThreshold(p_upperThreshold);
		thresholder->Update();

		processedImages.append(thresholder->GetOutput());
	}

	//Build Image Data for input of marching cubes - Refer to Ex12 VTK and similar to line 113
	vtkSmartPointer<vtkImageData> binaryVTKData = vtkSmartPointer<vtkImageData>::New();
	
	binaryVTKData->SetDimensions(m_dimensionX, m_dimensionY, m_dimensionZ);
	binaryVTKData->SetSpacing(m_spacingX, m_spacingY, m_spacingZ);
	binaryVTKData->SetExtent(0, m_dimensionX - 1, 0, m_dimensionY - 1, 0, m_dimensionZ - 1);

	//Create scalars to fill the vtkImageData
	vtkSmartPointer<vtkUnsignedCharArray> scalars = vtkSmartPointer<vtkUnsignedCharArray>::New();
	scalars->SetNumberOfValues(m_dimensionX * m_dimensionY * m_dimensionZ);

	int offset  = 0;
	//Iterate over all images and fill the scalars
	//For loop on binary images with ITK const iterator (refer to line 125)
	for (int z = 0; z < m_rawImages.size(); ++z)
	{
		//Create const ITK iterator on each image
		//In the while, set the value of each pixel to the scalars using variable offset
		//Ex3 - 
		itk::ImageRegionConstIterator<ShortImageType> localIterator(processedImages[z], processedImages[z]->GetLargestPossibleRegion());
		localIterator.GoToBegin();

		while (!localIterator.IsAtEnd()) {
			scalars->SetValue(offset, localIterator.Get());

			++offset;
			++localIterator;
		}
	}

	binaryVTKData->GetPointData()->SetScalars(scalars);

	//Apply the marching cubes - Ex12 VTK
	vtkSmartPointer<vtkMarchingCubes> marchingcubes = vtkSmartPointer<vtkMarchingCubes>::New();
	marchingcubes->SetInputData(binaryVTKData);
	marchingcubes->SetValue(0, 1);
	marchingcubes->Update();

	//Return resulting mesh
	vtkPolyData * mesh = vtkPolyData::New();
	
	// Deep copy of marching cubes to mesh data (because local data will be erased as soon as the method returns)
	mesh->DeepCopy(marchingcubes->GetOutput());

	return mesh;
}
