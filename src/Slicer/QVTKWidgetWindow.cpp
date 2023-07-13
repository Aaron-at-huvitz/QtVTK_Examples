#include "QVTKWidgetWindow.h"
#include "QVTKWidget.h"
#include "PrintingModel.h"

#include "HVolume.h"
#include "StopWatch.h"

QVTKWidgetWindow::QVTKWidgetWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    this->setWindowTitle("STL Viewer");
    this->showMaximized();

    InitializeMenuBar();
    InitializeSliders();

    InitializeVTK();
}

QVTKWidgetWindow::~QVTKWidgetWindow()
{
}

void QVTKWidgetWindow::InitializeVTK()
{
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    vtkNew<vtkRenderer> renderer;
    renderer->SetBackground(0.3, 0.5, 0.7);

    ui.vtkWidget->GetVTKOpenGLNativeWidget()->setRenderWindow(renderWindow);
    ui.vtkWidget->GetVTKOpenGLNativeWidget()->renderWindow()->AddRenderer(renderer);




    vtkNew<vtkVectorText> textSource;
    textSource->SetText("Slicer");

    vtkNew<vtkLinearExtrusionFilter> extrudeFilter;
    extrudeFilter->SetInputConnection(textSource->GetOutputPort());
    extrudeFilter->SetExtrusionTypeToVectorExtrusion();
    extrudeFilter->SetVector(0, 0, 1);
    extrudeFilter->SetScaleFactor(0.5);
    extrudeFilter->CappingOn();

    //vtkNew<vtkContourFilter> contourFilter;
    //contourFilter->SetInputData(extrudeFilter->GetOutput());
    //contourFilter->GenerateValues(1, 0.0, 0.0);

    vtkNew<vtkPolyDataMapper> textMapper;
    textMapper->SetInputConnection(textSource->GetOutputPort());

    vtkNew<vtkPolyDataMapper> extrudeMapper;
    extrudeMapper->SetInputConnection(extrudeFilter->GetOutputPort());

    //vtkNew<vtkPolyDataMapper> contourMapper;
    //contourMapper->SetInputConnection(contourFilter->GetOutputPort());

    vtkNew<vtkActor> textActor;
    textActor->SetMapper(textMapper);

    vtkNew<vtkActor> extrudeActor;
    extrudeActor->SetMapper(extrudeMapper);

    //vtkNew<vtkActor> contourActor;
    //contourActor->SetMapper(contourMapper);
    //contourActor->GetProperty()->SetColor(1.0, 0.0, 0.0);

    renderer->AddActor(textActor);
    renderer->AddActor(extrudeActor);
    //renderer->AddActor(contourActor);

    renderer->ResetCamera();
}

void QVTKWidgetWindow::InitializeMenuBar()
{
    auto fileMenu = ui.menuBar->addMenu("File (&F)");

    fileMenu->addAction("Open (&O)", this, SLOT(OnMenuActionOpen()));
}

void QVTKWidgetWindow::InitializeSliders()
{
    connect(ui.verticalSlider_1, SIGNAL(valueChanged(int)), this, SLOT(OnSlider1ValueChaned(int)));
    connect(ui.verticalSlider_2, SIGNAL(valueChanged(int)), this, SLOT(OnSlider1ValueChaned(int)));
    connect(ui.verticalSlider_3, SIGNAL(valueChanged(int)), this, SLOT(OnSlider1ValueChaned(int)));
}

void QVTKWidgetWindow::OnMenuActionOpen()
{
    auto fileName = QFileDialog::getOpenFileName(this, "Open File", "C:/Resources/3D/STL", "Supported Mesh Files (*.stl *.obj *.ply);;STL Files (*.stl);;Obj Files (*.obj);;PLY Files (*.ply)");
    if (false == fileName.isEmpty())
    {
        LoadModel(fileName);
    }
}

void QVTKWidgetWindow::OnSlider1ValueChaned(int value)
{
    cout << "slider1 : " << value << endl;
}

void QVTKWidgetWindow::OnSlider2ValueChaned(int value)
{
    cout << "slider2 : " << value << endl;
}

void QVTKWidgetWindow::OnSlider3ValueChaned(int value)
{
    cout << "slider3 : " << value << endl;
}


void QVTKWidgetWindow::LoadModel(const QString& fileName)
{
    if (nullptr != printingModel)
    {
        delete printingModel;
        printingModel = nullptr;
    }

    StopWatch::Start("Model Loading");
    
    auto renderers = ui.vtkWidget->GetVTKOpenGLNativeWidget()->renderWindow()->GetRenderers();
    auto renderer = renderers->GetFirstRenderer();
    while (nullptr != renderer) {
        renderer->RemoveAllViewProps();
        renderer = renderers->GetNextItem();
    }
    renderer = renderers->GetFirstRenderer();

    printingModel = new PrintingModel(renderer);
    printingModel->LoadModel(fileName);

    //auto longestEdgeLength = printingModel->GetLongestEdgeLength();
    //cout << "longestEdgeLength : " << longestEdgeLength << endl;
    //printingModel->Remesh(0.25);

    renderer->AddActor(printingModel->GetRemeshedModelActor());
    renderer->ResetCamera();
    ui.vtkWidget->GetVTKOpenGLNativeWidget()->renderWindow()->Render();

    HVolume volume(0.25, printingModel->GetRawModelData());
    auto minPoint = volume.GetMinPoint();
    auto maxPoint = volume.GetMaxPoint();
    cout << minPoint.x << ", " << minPoint.y << ", " << minPoint.z << ", "
        << maxPoint.x << ", " << maxPoint.y << ", " << maxPoint.z << endl;

    cout << "Resolution x: " << volume.GetResolutionX() << ", y: " << volume.GetResolutionY() << ", z: " << volume.GetResolutionZ() << endl;

    vtkNew<vtkPolyDataMapper> volumePolyDataMapper;
    volumePolyDataMapper->SetInputData(volume.GetPolyData());

    vtkNew<vtkActor> volumeActor;
    volumeActor->SetMapper(volumePolyDataMapper);
    volumeActor->GetProperty()->SetPointSize(10);

    renderer->AddActor(volumeActor);
    renderer->ResetCamera();
    ui.vtkWidget->GetVTKOpenGLNativeWidget()->renderWindow()->Render();

    StopWatch::Stop("Model Loading");
}
