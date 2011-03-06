#ifndef SimpleViewUI_H
#define SimpleViewUI_H

#include <QMainWindow>

#include <vtkXMLImageDataReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSphereSource.h>
#include <vtkImagePlaneWidget.h>
#include <vtkImageData.h>
#include <vtkCellPicker.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include "vtkSmartPointer.h"


// Forward Qt class declarations
class Ui_SimpleView;

class SimpleView : public QMainWindow
{
  Q_OBJECT
public:

  // Constructor/Destructor
  SimpleView();
  ~SimpleView() {};

public slots:

  virtual void slotExit();

  virtual void slotLoadImage1();
  virtual void slotLoadImage2();

protected:

  std::string GetFileName();

protected slots:

private:

  // Designer form
  Ui_SimpleView *ui;

  vtkSmartPointer<vtkRenderer> renderer;

  vtkSmartPointer< vtkActor >            transform1to2;

  vtkSmartPointer< vtkActor >             transform1;
  vtkSmartPointer< vtkImagePlaneWidget>   planeWidget1;
  vtkSmartPointer< vtkCellPicker >        picker1;
  vtkSmartPointer< vtkProperty >          property1;

  vtkSmartPointer< vtkActor >             transform2;
  vtkSmartPointer< vtkImagePlaneWidget>   planeWidget2;
  vtkSmartPointer< vtkCellPicker >        picker2;
  vtkSmartPointer< vtkProperty >          property2;
};

#endif // SimpleViewUI_H


