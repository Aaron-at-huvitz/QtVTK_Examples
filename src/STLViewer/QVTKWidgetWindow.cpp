#include "QVTKWidgetWindow.h"
#include "QVTKWidget.h"

#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkConeSource.h>
#include <vtkCubeSource.h>
#include <vtkDataObjectToTable.h>
#include <vtkElevationFilter.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkQtTableView.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkVersion.h>

QVTKWidgetWindow::QVTKWidgetWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    vtkNew<vtkNamedColors> colors;

    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;

    ui.vtkWidget->GetVTKOpenGLNativeWidget()->setRenderWindow(renderWindow);

    // Cone
    vtkNew<vtkConeSource> coneSource;
    coneSource->SetDirection(0.0, 1.0, 0.0);
    vtkNew<vtkPolyDataMapper> coneMapper;
    coneMapper->SetInputConnection(coneSource->GetOutputPort());
    vtkNew<vtkActor> coneActor;
    coneActor->SetMapper(coneMapper);
    coneActor->GetProperty()->SetColor(colors->GetColor4d("Tomato").GetData());

    // Cube
    vtkNew<vtkCubeSource> cubeSource;
    cubeSource->SetXLength(0.8);
    cubeSource->SetYLength(0.8);
    cubeSource->SetZLength(0.8);
    vtkNew<vtkPolyDataMapper> cubeMapper;
    cubeMapper->SetInputConnection(cubeSource->GetOutputPort());
    vtkNew<vtkActor> cubeActor;
    cubeActor->SetMapper(cubeMapper);
    cubeActor->GetProperty()->SetColor(
        colors->GetColor4d("MediumSeaGreen").GetData());

    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(coneActor);
    renderer->AddActor(cubeActor);
    renderer->SetBackground(colors->GetColor3d("LightSteelBlue").GetData());

    ui.vtkWidget->GetVTKOpenGLNativeWidget()->renderWindow()->AddRenderer(renderer);
    renderer->ResetCamera();
}

QVTKWidgetWindow::~QVTKWidgetWindow()
{}
