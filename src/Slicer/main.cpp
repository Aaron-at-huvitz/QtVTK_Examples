#include "QVTKWidgetWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QVTKWidgetWindow w;
    w.show();
    return a.exec();
}





#pragma region AppendPolyData Example
//#include <vtkSmartPointer.h>
//#include <vtkSTLReader.h>
//#include <vtkPolyDataMapper.h>
//#include <vtkActor.h>
//#include <vtkRenderer.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkTriangle.h>
//#include <vtkPoints.h>
//#include <vtkCellArray.h>
//#include <vtkAppendPolyData.h>
//#include <vtkInteractorStyleTrackballCamera.h>
//#include <vtkInteractorStyleSwitch.h>
//#include <vtkCallbackCommand.h>
//
//#include <random>

//vtkSmartPointer<vtkPolyData> loadMesh(const std::string& filePath) {
//    vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
//    reader->SetFileName(filePath.c_str());
//    reader->Update();
//    return reader->GetOutput();
//}
//
//vtkSmartPointer<vtkPolyData> createRandomTriangle() {
//    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
//    for (int i = 0; i < 3; ++i) {
//        double x = static_cast<double>(rand()) / RAND_MAX; // Random x coordinate between 0 and 1
//        double y = static_cast<double>(rand()) / RAND_MAX; // Random y coordinate between 0 and 1
//        double z = static_cast<double>(rand()) / RAND_MAX; // Random z coordinate between 0 and 1
//        points->InsertNextPoint(x, y, z);
//    }
//
//    vtkSmartPointer<vtkTriangle> triangle = vtkSmartPointer<vtkTriangle>::New();
//    triangle->GetPointIds()->SetId(0, 0);
//    triangle->GetPointIds()->SetId(1, 1);
//    triangle->GetPointIds()->SetId(2, 2);
//
//    vtkSmartPointer<vtkCellArray> triangles = vtkSmartPointer<vtkCellArray>::New();
//    triangles->InsertNextCell(triangle);
//
//    vtkSmartPointer<vtkPolyData> trianglePolyData = vtkSmartPointer<vtkPolyData>::New();
//    trianglePolyData->SetPoints(points);
//    trianglePolyData->SetPolys(triangles);
//
//    return trianglePolyData;
//}
//
//void onSpaceBarCallback(vtkObject* caller, long unsigned int eventId, void* clientData, void* callData) {
//    vtkRenderWindowInteractor* interactor = static_cast<vtkRenderWindowInteractor*>(caller);
//    vtkSmartPointer<vtkPolyData> meshPolyData = static_cast<vtkPolyData*>(clientData);
//
//    if (interactor->GetKeyCode() == ' ') {
//        vtkSmartPointer<vtkPolyData> randomTriangle = createRandomTriangle();
//
//        vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
//        appendFilter->AddInputData(meshPolyData);
//        appendFilter->AddInputData(randomTriangle);
//        appendFilter->Update();
//
//        meshPolyData->ShallowCopy(appendFilter->GetOutput());
//
//        interactor->GetRenderWindow()->Render();
//    }
//}
//
//int main(int argc, char* argv[]) {
//    //if (argc != 2) {
//    //    std::cerr << "Usage: " << argv[0] << " <mesh.stl>" << std::endl;
//    //    return 1;
//    //}
//
//    //std::string filePath = argv[1];
//    std::string filePath = "c:/Resources/3D/STL/UnitCube.stl";
//    vtkSmartPointer<vtkPolyData> meshPolyData = loadMesh(filePath);
//
//    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//    mapper->SetInputData(meshPolyData);
//
//    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
//    actor->SetMapper(mapper);
//
//    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
//    renderer->AddActor(actor);
//
//    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
//    renderWindow->AddRenderer(renderer);
//
//    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//    interactor->SetRenderWindow(renderWindow);
//
//    vtkSmartPointer<vtkInteractorStyleSwitch> styleSwitch = vtkSmartPointer<vtkInteractorStyleSwitch>::New();
//    interactor->SetInteractorStyle(styleSwitch);
//
//    vtkSmartPointer<vtkCallbackCommand> spaceBarCallback = vtkSmartPointer<vtkCallbackCommand>::New();
//    spaceBarCallback->SetCallback(onSpaceBarCallback);
//    spaceBarCallback->SetClientData(meshPolyData);
//    interactor->AddObserver(vtkCommand::KeyPressEvent, spaceBarCallback);
//
//    interactor->Initialize();
//    renderWindow->Render();
//    interactor->Start();
//
//    return 0;
//}
#pragma endregion

#pragma region Glyph3D Example
//#include <vtkActor.h>
//#include <vtkCellArray.h>
//#include <vtkCubeSource.h>
//#include <vtkGlyph3D.h>
//#include <vtkNamedColors.h>
//#include <vtkNew.h>
//#include <vtkPoints.h>
//#include <vtkPolyData.h>
//#include <vtkPolyDataMapper.h>
//#include <vtkProperty.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkRenderer.h>
//
//int main(int, char* [])
//{
//    vtkNew<vtkNamedColors> colors;
//
//    vtkNew<vtkPoints> points;
//    points->InsertNextPoint(0, 0, 0);
//    points->InsertNextPoint(1, 1, 0);
//    points->InsertNextPoint(2, 2, 0);
//    points->InsertNextPoint(3, 1, 0);
//    points->InsertNextPoint(4, 0, 0);
//
//    vtkNew<vtkPolyData> polydata;
//    polydata->SetPoints(points);
//
//    // Create anything you want here, we will use a cube for the demo.
//    vtkNew<vtkCubeSource> cubeSource;
//
//    vtkNew<vtkGlyph3D> glyph3D;
//    glyph3D->SetSourceConnection(cubeSource->GetOutputPort());
//    glyph3D->SetInputData(polydata);
//    glyph3D->Update();
//
//    // Visualize
//    vtkNew<vtkPolyDataMapper> mapper;
//    mapper->SetInputConnection(glyph3D->GetOutputPort());
//
//    vtkNew<vtkActor> actor;
//    actor->SetMapper(mapper);
//    actor->GetProperty()->SetColor(colors->GetColor3d("Salmon").GetData());
//
//    vtkNew<vtkRenderer> renderer;
//    vtkNew<vtkRenderWindow> renderWindow;
//    renderWindow->AddRenderer(renderer);
//    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
//    renderWindowInteractor->SetRenderWindow(renderWindow);
//
//    renderer->AddActor(actor);
//    renderer->SetBackground(colors->GetColor3d("SlateGray").GetData());
//
//    renderWindow->SetWindowName("Glyph3D");
//    renderWindow->Render();
//    renderWindowInteractor->Start();
//
//    return EXIT_SUCCESS;
//}
#pragma endregion
