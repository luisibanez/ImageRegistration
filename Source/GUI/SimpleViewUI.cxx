/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include "ui_SimpleViewUI.h"
#include "SimpleViewUI.h"

#include <QFileDialog>
#include <vtkCamera.h>
#include <vtkMatrix4x4.h>
#include <vtkImageChangeInformation.h>

#include "vtkKWImage.h"
#include "vtkKWImageIO.h"

// Constructor
SimpleView::SimpleView()
{
  this->ui = new Ui_SimpleView;
  this->ui->setupUi(this);

  renderer = vtkSmartPointer<vtkRenderer>::New();

  this->ui->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);

  vtkRenderWindowInteractor * renderInteractor = this->ui->qvtkWidget->GetInteractor();

  planeWidget1 = vtkSmartPointer<vtkImagePlaneWidget>::New();
  planeWidget2 = vtkSmartPointer<vtkImagePlaneWidget>::New();

  planeWidget1->SetInteractor( renderInteractor );
  planeWidget2->SetInteractor( renderInteractor );

  actor1 = vtkSmartPointer< vtkActor >::New();
  actor2 = vtkSmartPointer< vtkActor >::New();

  double origin[3] = {0,1,0};

  planeWidget1->SetOrigin(origin);
  planeWidget1->UpdatePlacement();

  planeWidget2->SetOrigin(origin);
  planeWidget2->UpdatePlacement();

  picker1 = vtkSmartPointer< vtkCellPicker >::New();
  picker2 = vtkSmartPointer< vtkCellPicker >::New();

  picker1->SetTolerance(0.005);
  picker2->SetTolerance(0.005);

  property1 = vtkSmartPointer< vtkProperty >::New();
  property2 = vtkSmartPointer< vtkProperty >::New();

  renderer->SetBackground(0.329,0.349,0.427);
  renderer->SetBackground2(0.658,0.698,0.855);
  renderer->GradientBackgroundOn();

  vtkCamera * camera = renderer->GetActiveCamera();

  camera->SetPosition ( 0.5, 0.5, -1 );
  camera->SetViewUp( 0, -1, 0 );

  renderer->ResetCamera();

  // Set up action signals and slots
  connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
  connect(this->ui->actionQuit, SIGNAL(triggered()), this, SLOT(slotExit()));
  connect(this->ui->actionImage1, SIGNAL(triggered()), this, SLOT(slotLoadImage1()));
  connect(this->ui->actionImage2, SIGNAL(triggered()), this, SLOT(slotLoadImage2()));
};

void SimpleView::slotExit()
{
  qApp->exit();
}

std::string SimpleView::GetFileName()
{
  QFileDialog fileDialog(this);

  fileDialog.setViewMode( QFileDialog::Detail );

  QStringList filters;
  filters << "Image files (*.*)";

  fileDialog.setFilters( filters );
  fileDialog.setLabelText( QFileDialog::LookIn,"Select Input");

  fileDialog.exec();

  QStringList listOfFiles = fileDialog.selectedFiles();

  std::string inputFilename = listOfFiles[0].toStdString();

  return inputFilename;
}

void SimpleView::slotLoadImage1()
{
  std::string inputFilename = this->GetFileName();

  vtkSmartPointer< vtkKWImageIO > kwreader = vtkSmartPointer< vtkKWImageIO >::New();

  kwreader->SetFileName( inputFilename );
  kwreader->ReadImage();

  vtkSmartPointer< vtkKWImage > kwimage = kwreader->HarvestReadImage();

  // We need to manage image origin in the Actor that holds the ImagePlane.
  // So, here, were remove the origin information from the VTK image, and let
  // the Actor grab that information later directly from the input ITK image.
  vtkSmartPointer< vtkImageChangeInformation > changeInformation =
    vtkSmartPointer< vtkImageChangeInformation >::New();

  changeInformation->SetInput( kwimage->GetVTKImage() );

  double nullorigin[3];
  nullorigin[0] = 0.0;
  nullorigin[1] = 0.0;
  nullorigin[2] = 0.0;

  changeInformation->SetOutputOrigin( nullorigin );

  changeInformation->Update();

  vtkImageData * vtkimage1 = changeInformation->GetOutput();

  const vtkKWImage::ImageBaseType * itkimage1 = kwimage->GetITKImageBase();

  planeWidget1->SetInput( vtkimage1 );

  int x0, x1, y0, y1, z0, z1;

  vtkimage1->GetExtent( x0, x1, y0, y1, z0, z1 );

  int middleSliceNumber = ( z1 + z0 ) / 2;

  vtkSmartPointer< vtkMatrix4x4 >  transformMatrix1 = vtkSmartPointer< vtkMatrix4x4 >::New();

  itk::ImageBase<3>::PointType       origin    = itkimage1->GetOrigin();
  itk::ImageBase<3>::DirectionType   direction = itkimage1->GetDirection();

  // VTK already takes image spacing into account, so here we only need to take
  // care of direction and origin.

  for(unsigned int i=0; i<3; i++ )
    {
    for(unsigned int j=0; j<3; j++ )
      {
      transformMatrix1->SetElement(i,j, direction(i,j) );
      }

    transformMatrix1->SetElement( i, 3, origin[i]);
    }

  transformMatrix1->Print( std::cout );

  actor1->SetUserMatrix( transformMatrix1 );
  planeWidget1->SetPlaneOrientationToZAxes();
  planeWidget1->SetSlicePosition( middleSliceNumber );
  planeWidget1->SetPicker(picker1);
  planeWidget1->RestrictPlaneToVolumeOn();
  planeWidget1->SetKeyPressActivationValue('z');
  planeWidget1->SetTexturePlaneProperty(property1);
  planeWidget1->SetProp3D( actor1 );
  planeWidget1->On();

  vtkCamera * camera = renderer->GetActiveCamera();

  camera->SetPosition ( 0.5, 0.5, -1 );
  camera->SetViewUp( 0, -1, 0 );

  renderer->ResetCamera();
}

void SimpleView::slotLoadImage2()
{
  std::string inputFilename = this->GetFileName();

  vtkSmartPointer< vtkKWImageIO > kwreader = vtkSmartPointer< vtkKWImageIO >::New();

  kwreader->SetFileName( inputFilename );
  kwreader->ReadImage();

  vtkSmartPointer< vtkKWImage > kwimage = kwreader->HarvestReadImage();

  // We need to manage image origin in the Actor that holds the ImagePlane.
  // So, here, were remove the origin information from the VTK image, and let
  // the Actor grab that information later directly from the input ITK image.
  vtkSmartPointer< vtkImageChangeInformation > changeInformation =
    vtkSmartPointer< vtkImageChangeInformation >::New();

  changeInformation->SetInput( kwimage->GetVTKImage() );

  double nullorigin[3];
  nullorigin[0] = 0.0;
  nullorigin[1] = 0.0;
  nullorigin[2] = 0.0;

  changeInformation->SetOutputOrigin( nullorigin );

  changeInformation->Update();

  vtkImageData * vtkimage2 = changeInformation->GetOutput();

  const vtkKWImage::ImageBaseType * itkimage2 = kwimage->GetITKImageBase();

  planeWidget2->SetInput( vtkimage2 );

  int x0, x1, y0, y1, z0, z1;

  vtkimage2->GetExtent( x0, x1, y0, y1, z0, z1 );

  int middleSliceNumber = ( z1 + z0 ) / 2;

  vtkSmartPointer< vtkMatrix4x4 >  transformMatrix2 = vtkSmartPointer< vtkMatrix4x4 >::New();

  itk::ImageBase<3>::PointType       origin    = itkimage2->GetOrigin();
  itk::ImageBase<3>::DirectionType   direction = itkimage2->GetDirection();

  // VTK already takes image spacing into account, so here we only need to take
  // care of direction and origin.

  for(unsigned int i=0; i<3; i++ )
    {
    for(unsigned int j=0; j<3; j++ )
      {
      transformMatrix2->SetElement(i,j, direction(i,j) );
      }

    transformMatrix2->SetElement( i, 3, origin[i]);
    }

  transformMatrix2->Print( std::cout );

  actor2->SetUserMatrix( transformMatrix2 );
  planeWidget2->SetPlaneOrientationToZAxes();
  planeWidget2->SetSlicePosition( middleSliceNumber );
  planeWidget2->SetPicker(picker2);
  planeWidget2->RestrictPlaneToVolumeOn();
  planeWidget2->SetKeyPressActivationValue('z');
  planeWidget2->SetTexturePlaneProperty(property2);
  planeWidget2->SetProp3D( actor2 );
  planeWidget2->On();

  vtkCamera * camera = renderer->GetActiveCamera();

  camera->SetPosition ( 0.5, 0.5, -1 );
  camera->SetViewUp( 0, -1, 0 );

  renderer->ResetCamera();
}
