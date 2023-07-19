#include "QVTKWidgetWindow.h"
#include "QVTKWidget.h"

QVTKWidgetWindow::QVTKWidgetWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    this->setWindowTitle("STL Viewer");
    this->showMaximized();

    InitializeMenuBar();

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






    vtkNew<vtkNamedColors> colors;

    // Create some spheres
    vtkNew<vtkSphereSource> sphereSource1;
    sphereSource1->Update();

    vtkNew<vtkSphereSource> sphereSource2;
    sphereSource2->SetCenter(5, 0, 0);
    sphereSource2->Update();

    vtkNew<vtkSphereSource> sphereSource3;
    sphereSource3->SetCenter(10, 0, 0);
    sphereSource3->Update();

    vtkNew<vtkAppendPolyData> appendFilter;
    appendFilter->AddInputConnection(sphereSource1->GetOutputPort());
    appendFilter->AddInputConnection(sphereSource2->GetOutputPort());
    appendFilter->AddInputConnection(sphereSource3->GetOutputPort());
    appendFilter->Update();

    vtkNew<vtkPolyDataConnectivityFilter> connectivityFilter;
    connectivityFilter->SetInputConnection(appendFilter->GetOutputPort());
    connectivityFilter->SetExtractionModeToAllRegions();
    connectivityFilter->ColorRegionsOn();
    connectivityFilter->Update();

    // Visualize
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(connectivityFilter->GetOutputPort());
    mapper->SetScalarRange(connectivityFilter->GetOutput()
        ->GetPointData()
        ->GetArray("RegionId")
        ->GetRange());
    mapper->Update();

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    renderer->AddActor(actor);













    //vtkNew<vtkVectorText> textSource;
    //textSource->SetText("Slicer");

    //vtkNew<vtkLinearExtrusionFilter> extrudeFilter;
    //extrudeFilter->SetInputConnection(textSource->GetOutputPort());
    //extrudeFilter->SetExtrusionTypeToVectorExtrusion();
    //extrudeFilter->SetVector(0, 0, 1);
    //extrudeFilter->SetScaleFactor(0.5);
    //extrudeFilter->CappingOn();

    ////vtkNew<vtkContourFilter> contourFilter;
    ////contourFilter->SetInputData(extrudeFilter->GetOutput());
    ////contourFilter->GenerateValues(1, 0.0, 0.0);

    //vtkNew<vtkPolyDataMapper> textMapper;
    //textMapper->SetInputConnection(textSource->GetOutputPort());

    //vtkNew<vtkPolyDataMapper> extrudeMapper;
    //extrudeMapper->SetInputConnection(extrudeFilter->GetOutputPort());

    ////vtkNew<vtkPolyDataMapper> contourMapper;
    ////contourMapper->SetInputConnection(contourFilter->GetOutputPort());

    //vtkNew<vtkActor> textActor;
    //textActor->SetMapper(textMapper);

    //vtkNew<vtkActor> extrudeActor;
    //extrudeActor->SetMapper(extrudeMapper);

    ////vtkNew<vtkActor> contourActor;
    ////contourActor->SetMapper(contourMapper);
    ////contourActor->GetProperty()->SetColor(1.0, 0.0, 0.0);

    //renderer->AddActor(textActor);
    //renderer->AddActor(extrudeActor);
    ////renderer->AddActor(contourActor);

    //renderer->ResetCamera();
}

void QVTKWidgetWindow::InitializeMenuBar()
{
    auto fileMenu = ui.menuBar->addMenu("File (&F)");

    fileMenu->addAction("Open (&O)", this, SLOT(OnMenuActionOpen()));
}

void QVTKWidgetWindow::OnMenuActionOpen()
{
    auto fileName = QFileDialog::getOpenFileName(this, "Open File", "C:/Resources/3D/STL", "Supported Mesh Files (*.stl *.obj *.ply);;STL Files (*.stl);;Obj Files (*.obj);;PLY Files (*.ply)");
    if (false == fileName.isEmpty())
    {
        LoadModel(fileName);
    }
}

void QVTKWidgetWindow::LoadModel(const QString& fileName)
{
    auto extension = fileName.last(3).toLower();

    vtkAbstractPolyDataReader* reader = nullptr;
    if ("stl" == extension)
    {
        reader = vtkSTLReader::New();
    }
    else if ("obj" == extension)
    {
        reader = vtkOBJReader::New();
    }
    else if ("ply" == extension)
    {
        reader = vtkPLYReader::New();
    }

    reader->SetFileName(fileName.toStdString().c_str());
    reader->Update();

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(reader->GetOutputPort());

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    auto renderers = ui.vtkWidget->GetVTKOpenGLNativeWidget()->renderWindow()->GetRenderers();
    auto renderer = renderers->GetFirstRenderer();
    while (nullptr != renderer) {
        renderer->RemoveAllViewProps();
        renderer = renderers->GetNextItem();
    }
    renderer = renderers->GetFirstRenderer();

    renderer->AddActor(actor);
    renderer->ResetCamera();
    ui.vtkWidget->GetVTKOpenGLNativeWidget()->renderWindow()->Render();

    reader->Delete();
}
