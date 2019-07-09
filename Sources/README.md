This is a test for a C++ application using Qt/VTK/ITK.

ITK is used for Image Processing.
VTK is used for Mesh Processing.
QT is used for UI and files management.

- Install environment following instructions in Install.pdf. You need VS2017. ExternalLibs are compiled in Release mode only and should be installed on the system.
ExternalLibs, Testing data and Install.pdf are available here: https://drive.google.com/drive/folders/1TyoN5u6bnOPJMv_dYDp8rSOkomU4Ui2h?usp=sharing

- Regarding Testing data, it is composed of several patients. Each patient folder contains a pre-segmented CT scan of the shoulder. Humerus is defined as value 2 and Scapula is defined as value 1.

- The goal of this application is to load a pre-segmented CT scan of the shoulder. The data are provided separately in Zip file.

1) Select and read folder to find images
![screenshot1](https://user-images.githubusercontent.com/50709142/57864383-11773600-77fc-11e9-96a3-9e1870b2c7e0.PNG)

2) Read all Images using ITK - itkImageFileReader

3) Build a 3D VTK Volume to display the 2D images in Qt view using VTK - vtkImageData
![screenshot2](https://user-images.githubusercontent.com/50709142/57864425-29e75080-77fc-11e9-94c6-67185eb6bcda.PNG)

4) Segment data to build the 3D meshes - itkBinaryImageThresholdFilter - vtkMarchingCubes

5) Display the 3D meshes in 3D view - vtkMapper - vtkActor
![screenshot3](https://user-images.githubusercontent.com/50709142/57864448-3075c800-77fc-11e9-8421-33fb6f388add.PNG)


Please use branch exos for assistance. Several are refered in the source code.
