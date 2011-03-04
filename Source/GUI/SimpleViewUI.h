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
  virtual void slotLoadImage();

protected:

protected slots:

private:

  // Designer form
  Ui_SimpleView *ui;


  vtkSmartPointer<vtkXMLImageDataReader> reader;
  vtkSmartPointer<vtkSphereSource> sphereSource;
  vtkSmartPointer<vtkPolyDataMapper> sphereMapper;
  vtkSmartPointer<vtkActor> sphereActor;
  vtkSmartPointer<vtkRenderer> renderer;
  vtkSmartPointer<vtkImagePlaneWidget> planeWidget;
  vtkSmartPointer< vtkCellPicker > picker;
  vtkSmartPointer< vtkProperty > property;

};

#endif // SimpleViewUI_H


