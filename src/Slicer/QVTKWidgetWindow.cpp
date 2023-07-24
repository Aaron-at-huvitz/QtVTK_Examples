#include "QVTKWidgetWindow.h"
#include "QVTKWidget.h"
#include "QVoxelizationOptionDialog.h"
#include "HPrintingModel.h"
#include "HVisualDebugging.h"

#include "HVolume.h"
#include "StopWatch.h"

QVTKWidgetWindow::QVTKWidgetWindow(QWidget* parent)
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
    HVisualDebugging::Terminate();

    if (nullptr != printingModel)
    {
        delete printingModel;
        printingModel = nullptr;
    }

    if (nullptr != voxelizationOptionDialog)
    {
        delete voxelizationOptionDialog;
        voxelizationOptionDialog = nullptr;
    }
}

void QVTKWidgetWindow::InitializeVTK()
{
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    vtkNew<vtkRenderer> renderer;
    renderer->SetBackground(0.3, 0.5, 0.7);

    ui.vtkWidget->GetVTKOpenGLNativeWidget()->setRenderWindow(renderWindow);
    ui.vtkWidget->GetVTKOpenGLNativeWidget()->renderWindow()->AddRenderer(renderer);

    HVisualDebugging::Initialize(renderer);



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

    auto operationMenu = ui.menuBar->addMenu("Operation (&O)");
    operationMenu->addAction("Find Overhang (&O)", this, SLOT(OnMenuActionFindOverhang()));
    operationMenu->addAction("Find Island (&I)", this, SLOT(OnMenuActionFindIsland()));
    operationMenu->addAction("Voxelize (&V)", this, SLOT(OnMenuActionVoxelize()));
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
        StopWatch::Start("Model Loading");
        
        LoadModel(fileName);
        ui.vtkWidget->GetVTKOpenGLNativeWidget()->renderWindow()->Render();
        
        StopWatch::Stop("Model Loading");
    }
}

void QVTKWidgetWindow::OnMenuActionFindOverhang()
{
    if (nullptr != printingModel)
    {
        StopWatch::Start("Analyze Overhang Face Normal");

        printingModel->AnalyzeOverhang();
        ui.vtkWidget->GetVTKOpenGLNativeWidget()->renderWindow()->Render();

        StopWatch::Stop("Analyze Overhang Face Normal");
    }
}

void QVTKWidgetWindow::OnMenuActionVoxelize()
{
    if (nullptr != printingModel)
    {
        if (nullptr != voxelizationOptionDialog)
        {
            delete voxelizationOptionDialog;
            voxelizationOptionDialog = nullptr;
        }
        
        voxelizationOptionDialog = new QVoxelizationOptionDialog(this);
        voxelizationOptionDialog->setModal(true);
        voxelizationOptionDialog->exec();

        bool voxelize = voxelizationOptionDialog->Voxelize();
        double voxelSize = voxelizationOptionDialog->VoxelSize();

        delete voxelizationOptionDialog;
        voxelizationOptionDialog = nullptr;

        if (voxelize)
        {
            StopWatch::Start("Voxelize");
            printingModel->Voxelize(printingModel->GetInitialModelData(), voxelSize);
            ui.vtkWidget->GetVTKOpenGLNativeWidget()->renderWindow()->Render();
            StopWatch::Stop("Voxelize");
        }
    }
}

void QVTKWidgetWindow::OnMenuActionFindIsland()
{
    if (nullptr != printingModel)
    {
        StopWatch::Start("Find Island");

        printingModel->AnalyzeIsland();
        ui.vtkWidget->GetVTKOpenGLNativeWidget()->renderWindow()->Render();

        StopWatch::Stop("Find Island");
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

void QVTKWidgetWindow::mousePressEvent(QMouseEvent* event)
{
    //cout << "mouse press event" << endl;
    if (event->button() == Qt::LeftButton) {
        lastMouseLButtonPosition = event->pos();
    }

    if (event->button() == Qt::RightButton) {
        lastMouseRButtonPosition = event->pos();
    }

    if (event->button() == Qt::MiddleButton) {
        lastMouseMButtonPosition = event->pos();
    }
}

void QVTKWidgetWindow::mouseReleaseEvent(QMouseEvent* event)
{
    auto pos = event->pos();

    if (event->button() == Qt::LeftButton) {
        if (pos == lastMouseLButtonPosition)
        {
            cout << "mouse release event" << endl;

            if (nullptr != printingModel)
            {
                auto vtkNativeWidget = ui.vtkWidget->GetVTKOpenGLNativeWidget();

                pos = ui.vtkWidget->mapFromParent(pos);
                pos = vtkNativeWidget->mapFromParent(pos);
                pos = ui.centralWidget->mapFromParent(pos);

                int qtX = pos.x();
                int qtY = pos.y();

                int vtkWidgetWidth = vtkNativeWidget->width();
                int vtkWidgetHeight = vtkNativeWidget->height();

                auto renderers = vtkNativeWidget->renderWindow()->GetRenderers();
                auto renderer = renderers->GetFirstRenderer();
                int rendererWidth = renderer->GetSize()[0];
                int rendererHeight = renderer->GetSize()[1];

                double scaleX = (double)rendererWidth / vtkWidgetWidth;
                double scaleY = (double)rendererHeight / vtkWidgetHeight;

                double vtkX = qtX * scaleX;
                double vtkY = (vtkWidgetHeight - qtY) * scaleY;

                printingModel->Pick(vtkX, vtkY);

                vtkNativeWidget->renderWindow()->Render();
            }
        }
    }
}

void QVTKWidgetWindow::keyPressEvent(QKeyEvent* event)
{

}

void QVTKWidgetWindow::keyReleaseEvent(QKeyEvent* event)
{
    //if (event->key() == Qt::Key_QuoteLeft)
    if (event->key() == Qt::Key_Escape)
    {
        HVisualDebugging::ToggleLines();
        HVisualDebugging::ToggleTriangles();
        HVisualDebugging::ToggleSpheres();
    }
    else if (event->key() == Qt::Key_F1)
    {
        if (nullptr != printingModel)
        {
            if (event->modifiers() == Qt::KeyboardModifier::ShiftModifier)
            {
                printingModel->ToggleInitialModelRepresentation();
            }
            else
            {
                printingModel->ToggleInitialModelVisibility();
            }
        }
    }
    else if (event->key() == Qt::Key_F2)
    {
        if (nullptr != printingModel)
        {
            if (event->modifiers() == Qt::KeyboardModifier::ShiftModifier)
            {
                printingModel->ToggleVolumeModelRepresentation();
            }
            else
            {
                printingModel->ToggleVolumeModelVisibility();
            }
        }
    }
    else if (event->key() == Qt::Key_F3)
    {
        if (nullptr != printingModel)
        {
            if (event->modifiers() == Qt::KeyboardModifier::ShiftModifier)
            {
                printingModel->ToggleOverhangModelRepresentation();
            }
            else
            {
                printingModel->ToggleOverhangModelVisibility();
            }
        }
    }
    else if (event->key() == Qt::Key_Space)
    {
        HVector3 p0, p1, p2;
        p0.x = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX));
        p0.y = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX));
        p0.z = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX));

        p1.x = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX));
        p1.y = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX));
        p1.z = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX));

        p2.x = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX));
        p2.y = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX));
        p2.z = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX));

        unsigned char r = unsigned char(static_cast<double>(rand()) / (static_cast<double>(RAND_MAX)) * 255);
        unsigned char g = unsigned char(static_cast<double>(rand()) / (static_cast<double>(RAND_MAX)) * 255);
        unsigned char b = unsigned char(static_cast<double>(rand()) / (static_cast<double>(RAND_MAX)) * 255);

        HVisualDebugging::AddTriangle(p0, p1, p2, r, g, b);
        ui.vtkWidget->GetVTKOpenGLNativeWidget()->renderWindow()->Render();
    }
}

void QVTKWidgetWindow::LoadModel(const QString& fileName)
{
    if (nullptr != printingModel)
    {
        delete printingModel;
        printingModel = nullptr;
    }

    if (nullptr != orientationMarkerWidget)
    {
        orientationMarkerWidget = nullptr;
    }

    auto renderers = ui.vtkWidget->GetVTKOpenGLNativeWidget()->renderWindow()->GetRenderers();
    auto renderer = renderers->GetFirstRenderer();
    while (nullptr != renderer) {
        renderer->RemoveAllViewProps();
        renderer = renderers->GetNextItem();
    }
    renderer = renderers->GetFirstRenderer();

    HVisualDebugging::Terminate();

    printingModel = new HPrintingModel(renderer);
    printingModel->LoadModel(fileName);

    //auto longestEdgeLength = printingModel->GetLongestEdgeLength();
    //cout << "longestEdgeLength : " << longestEdgeLength << endl;
    //printingModel->Remesh(0.2);
    //renderer->AddActor(printingModel->GetRemeshedModelActor());

    renderer->AddActor(printingModel->GetInitialModelActor());

    renderer->ResetCamera();

    auto bounds = printingModel->GetInitialModelActor()->GetBounds();

    HVisualDebugging::Initialize(renderer);
    
    HVector3 pO{ 0.0, 0.0, 0.0 };
    HVector3 pX{ (bounds[1] - bounds[0]) * 2, 0.0, 0.0};
    HVector3 pY{ 0.0, (bounds[3] - bounds[2]) * 2, 0.0 };
    HVector3 pZ{ 0.0, 0.0, (bounds[5] - bounds[4]) * 2 };
    HVisualDebugging::AddLine((double*)&pO, (double*)&pX, 255, 0, 0);
    HVisualDebugging::AddLine((double*)&pO, (double*)&pY, 0, 255, 0);
    HVisualDebugging::AddLine((double*)&pO, (double*)&pZ, 0, 0, 255);

    vtkNew<vtkAxesActor> axes;
    //axes->SetShaftTypeToLine();
    //axes->SetTotalLength(33.0, 33.0, 33.0);
    //axes->SetNormalizedShaftLength(1.0, 1.0, 1.0);
    //axes->SetNormalizedTipLength(0.05, 0.05, 0.05);

    //auto xAxisLabel = axes->GetXAxisCaptionActor2D();
    //xAxisLabel->GetCaptionTextProperty()->SetFontSize(60);

    //auto yAxisLabel = axes->GetYAxisCaptionActor2D();
    //yAxisLabel->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
    //yAxisLabel->GetPositionCoordinate()->SetValue(-0.20, 0.05);
    //yAxisLabel->GetCaptionTextProperty()->SetFontSize(14);

    //auto zAxisLabel = axes->GetZAxisCaptionActor2D();
    //zAxisLabel->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
    //zAxisLabel->GetPositionCoordinate()->SetValue(0.00, -0.13);
    //zAxisLabel->GetCaptionTextProperty()->SetFontSize(14);

    axes->SetShaftTypeToCylinder();

    axes->SetXAxisLabelText("X");
    axes->SetYAxisLabelText("Y");
    axes->SetZAxisLabelText("Z");
    axes->SetCylinderRadius(0.5 * axes->GetCylinderRadius());
    axes->SetConeRadius(1.025 * axes->GetConeRadius());
    axes->SetSphereRadius(1.5 * axes->GetSphereRadius());

    axes->GetXAxisCaptionActor2D()->GetPositionCoordinate()->SetValue(0, 10);
    axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(1, 0, 0);
    axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->ShadowOff();
    axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(30);
    axes->GetYAxisCaptionActor2D()->GetPositionCoordinate()->SetValue(0, 10);
    axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 1, 0);
    axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->ShadowOff();
    axes->GetZAxisCaptionActor2D()->GetPositionCoordinate()->SetValue(0, 10);
    axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 0, 1);
    axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->ShadowOff();
    //auto tprop = axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty();
    //tprop->ItalicOn();
    //tprop->ShadowOn();
    //tprop->SetFontFamilyToTimes();
    //// Use the same text properties on the other two axes.
    //axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->ShallowCopy(tprop);
    //axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->ShallowCopy(tprop);

    orientationMarkerWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    orientationMarkerWidget->SetOrientationMarker(axes);
    // Position upper left in the viewport.
    // orientationMarkerWidget->SetViewport(0.0, 0.8, 0.2, 1.0);
    // Position lower left in the viewport.
    orientationMarkerWidget->SetViewport(0, 0, 0.15, 0.15);
    orientationMarkerWidget->SetZoom(1.5);
    orientationMarkerWidget->SetInteractor(renderer->GetRenderWindow()->GetInteractor());
    orientationMarkerWidget->EnabledOn();
    orientationMarkerWidget->InteractiveOff();
}
