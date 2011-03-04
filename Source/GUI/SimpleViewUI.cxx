#include "ui_SimpleViewUI.h"
#include "SimpleViewUI.h"
#include <QFileDialog>
#include <vtkCamera.h>

// Constructor
SimpleView::SimpleView()
{
  this->ui = new Ui_SimpleView;
  this->ui->setupUi(this);

  reader = vtkSmartPointer<vtkXMLImageDataReader>::New();

  renderer = vtkSmartPointer<vtkRenderer>::New();

  this->ui->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);

  planeWidget = vtkSmartPointer<vtkImagePlaneWidget>::New();

  vtkRenderWindowInteractor * iren = this->ui->qvtkWidget->GetInteractor();

  planeWidget->SetInteractor( iren );

  double origin[3] = {0, 1,0};
  planeWidget->SetOrigin(origin);
  planeWidget->UpdatePlacement();


  picker = vtkSmartPointer< vtkCellPicker >::New();
  picker->SetTolerance(0.005);

  property = vtkSmartPointer< vtkProperty >::New();

  // Set up action signals and slots
  connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
  connect(this->ui->pushButton, SIGNAL(pressed()), this, SLOT(slotLoadImage()));
};

void SimpleView::slotExit()
{
  qApp->exit();
}

void SimpleView::slotLoadImage()
{
  QFileDialog fileDialog(this);

  fileDialog.setViewMode( QFileDialog::Detail );

  QStringList filters;
  filters << "Image files (*.vti)";

  fileDialog.setFilters( filters );
  fileDialog.setLabelText( QFileDialog::LookIn,"Select Input");

  fileDialog.exec();

  QStringList listOfFiles = fileDialog.selectedFiles();

  std::string inputFilename = listOfFiles[0].toStdString();

  reader->SetFileName(inputFilename.c_str());
  reader->Update();

  vtkImageData * image = reader->GetOutput();

  planeWidget->SetInput( image );

  int x0, x1, y0, y1, z0, z1;

  image->GetExtent( x0, x1, y0, y1, z0, z1 );

  int middleSliceNumber = ( z1 + z0 ) / 2;

  planeWidget->SetPlaneOrientationToZAxes();
  planeWidget->SetSlicePosition( middleSliceNumber );
  planeWidget->SetPicker(picker);
  planeWidget->RestrictPlaneToVolumeOn();
  planeWidget->SetKeyPressActivationValue('z');
  planeWidget->SetTexturePlaneProperty(property);
  planeWidget->On();

  vtkCamera * camera = renderer->GetActiveCamera();

  camera->SetPosition ( 0.5, 0.5, -1 );
  camera->SetViewUp( 0, -1, 0 );

  renderer->ResetCamera();
}
