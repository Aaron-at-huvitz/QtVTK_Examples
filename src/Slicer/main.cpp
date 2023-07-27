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

#pragma region PCADemo
//#include <vtkActor.h>
//#include <vtkBoxMuellerRandomSequence.h>
//#include <vtkDoubleArray.h>
//#include <vtkGlyph3DMapper.h>
//#include <vtkLine.h>
//#include <vtkLineSource.h>
//#include <vtkNamedColors.h>
//#include <vtkNew.h>
//#include <vtkPCAStatistics.h>
//#include <vtkPoints.h>
//#include <vtkPolyData.h>
//#include <vtkPolyDataMapper.h>
//#include <vtkProperty.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkRenderer.h>
//#include <vtkSmartPointer.h>
//#include <vtkSphereSource.h>
//#include <vtkTable.h>
//#include <vtkTransform.h>
//#include <vtkTransformPolyDataFilter.h>
//#include <vtkVertexGlyphFilter.h>
//
//namespace {
//    /**
//     * Convert points to glyphs.
//     *
//     * @param points - The points to glyph
//     * @param scale - The scale, used to determine the size of the glyph
//     * representing the point, expressed as a fraction of the largest side of the
//     * bounding box surrounding the points. e.g. 0.05
//     *
//     * @return The actor.
//     */
//    vtkSmartPointer<vtkActor> PointToGlyph(vtkPoints* points, double const& scale);
//
//} // namespace
//
//int main(int, char* [])
//{
//    vtkNew<vtkBoxMuellerRandomSequence> randomSequence;
//
//    vtkNew<vtkPoints> points;
//    for (unsigned int i = 0; i < 200; i++)
//    {
//        auto x = randomSequence->GetScaledValue(0, 2);
//        randomSequence->Next();
//        auto y = randomSequence->GetScaledValue(0, 5);
//        randomSequence->Next();
//        points->InsertNextPoint(x, y, 0);
//    }
//    vtkNew<vtkPolyData> polydata;
//    polydata->SetPoints(points);
//
//    vtkNew<vtkTransform> transform;
//    transform->RotateZ(30);
//
//    vtkNew<vtkTransformPolyDataFilter> transformFilter;
//    transformFilter->SetTransform(transform);
//    transformFilter->SetInputData(polydata);
//    transformFilter->Update();
//
//    // These would be all of your "x" values.
//    vtkNew<vtkDoubleArray> xArray;
//    xArray->SetNumberOfComponents(1);
//    xArray->SetName("x");
//
//    // These would be all of your "y" values.
//    vtkNew<vtkDoubleArray> yArray;
//    yArray->SetNumberOfComponents(1);
//    yArray->SetName("y");
//
//    for (vtkIdType i = 0; i < polydata->GetNumberOfPoints(); i++)
//    {
//        double p[3];
//        transformFilter->GetOutput()->GetPoint(i, p);
//        xArray->InsertNextValue(p[0]);
//        yArray->InsertNextValue(p[1]);
//    }
//
//    vtkNew<vtkTable> datasetTable;
//    datasetTable->AddColumn(xArray);
//    datasetTable->AddColumn(yArray);
//
//    vtkNew<vtkPCAStatistics> pcaStatistics;
//    pcaStatistics->SetInputData(vtkStatisticsAlgorithm::INPUT_DATA, datasetTable);
//
//    pcaStatistics->SetColumnStatus("x", 1);
//    pcaStatistics->SetColumnStatus("y", 1);
//
//    pcaStatistics->RequestSelectedColumns();
//    pcaStatistics->SetDeriveOption(true);
//    pcaStatistics->Update();
//
//    ///////// Eigenvalues ////////////
//    vtkNew<vtkDoubleArray> eigenvalues;
//    pcaStatistics->GetEigenvalues(eigenvalues);
//    for (vtkIdType i = 0; i < eigenvalues->GetNumberOfTuples(); i++)
//    {
//        std::cout << "Eigenvalue " << i << " = " << eigenvalues->GetValue(i)
//            << std::endl;
//    }
//
//    ///////// Eigenvectors ////////////
//    vtkNew<vtkDoubleArray> eigenvectors;
//
//    pcaStatistics->GetEigenvectors(eigenvectors);
//
//    vtkNew<vtkDoubleArray> evec1;
//    pcaStatistics->GetEigenvector(0, evec1);
//
//    vtkNew<vtkDoubleArray> evec2;
//    pcaStatistics->GetEigenvector(1, evec2);
//
//    double scale = 3.0;
//    vtkNew<vtkLineSource> vector1Source;
//    vector1Source->SetPoint1(0, 0, 0);
//    vector1Source->SetPoint1(scale * evec1->GetValue(0),
//        scale * evec1->GetValue(1), 0);
//
//    vtkNew<vtkPolyDataMapper> vec1Mapper;
//    vec1Mapper->SetInputConnection(vector1Source->GetOutputPort());
//
//    vtkNew<vtkNamedColors> colors;
//
//    vtkNew<vtkActor> vector1Actor;
//    vector1Actor->SetMapper(vec1Mapper);
//    vector1Actor->GetProperty()->SetColor(
//        colors->GetColor3d("LimeGreen").GetData());
//    vector1Actor->GetProperty()->SetLineWidth(3);
//
//    vtkNew<vtkLineSource> vector2Source;
//    vector2Source->SetPoint1(0, 0, 0);
//    vector2Source->SetPoint1(scale * evec2->GetValue(0),
//        scale * evec2->GetValue(1), 0);
//
//    vtkNew<vtkPolyDataMapper> vec2Mapper;
//    vec2Mapper->SetInputConnection(vector2Source->GetOutputPort());
//
//    vtkNew<vtkActor> vector2Actor;
//    vector2Actor->SetMapper(vec2Mapper);
//    vector2Actor->GetProperty()->SetColor(
//        colors->GetColor3d("Crimson").GetData());
//    vector2Actor->GetProperty()->SetLineWidth(3);
//
//    // Project all of the points onto the eigenvector with
//    // the largest eigenvalues
//    double p0[3];
//    p0[0] = -100 * evec1->GetValue(0);
//    p0[1] = -100 * evec1->GetValue(1);
//    p0[2] = 0;
//    double p1[3];
//    p1[0] = 100 * evec1->GetValue(0);
//    p1[1] = 100 * evec1->GetValue(1);
//    p1[2] = 0;
//
//    vtkNew<vtkPoints> projectedPoints;
//    for (vtkIdType i = 0; i < polydata->GetNumberOfPoints(); i++)
//    {
//        double p[3];
//        transformFilter->GetOutput()->GetPoint(i, p);
//        double t;
//        double closestPoint[3];
//        vtkLine::DistanceToLine(p, p0, p1, t, closestPoint);
//        double newP[3];
//        double v[3];
//        vtkMath::Subtract(p1, p0, v);
//        vtkMath::Normalize(v);
//        vtkMath::MultiplyScalar(v, t);
//        vtkMath::Add(p0, v, newP);
//        projectedPoints->InsertNextPoint(t, 0, 0);
//    }
//
//    vtkNew<vtkPolyData> projectedPolyData;
//    projectedPolyData->SetPoints(projectedPoints);
//
//    vtkNew<vtkVertexGlyphFilter> projectedGlyphFilter;
//    projectedGlyphFilter->SetInputData(projectedPolyData);
//    projectedGlyphFilter->Update();
//
//    vtkNew<vtkPolyDataMapper> projectedMapper;
//    projectedMapper->SetInputConnection(projectedGlyphFilter->GetOutputPort());
//    vtkNew<vtkActor> projectedActor;
//    projectedActor->SetMapper(projectedMapper);
//    projectedActor->GetProperty()->SetPointSize(2);
//    projectedActor->GetProperty()->SetColor(colors->GetColor3d("Gold").GetData());
//
//    // There will be one render window
//    vtkNew<vtkRenderWindow> renderWindow;
//    renderWindow->SetSize(600, 300);
//    renderWindow->SetWindowName("PCADemo");
//
//    // And one interactor
//    vtkNew<vtkRenderWindowInteractor> interactor;
//    interactor->SetRenderWindow(renderWindow);
//
//    // Define viewport ranges
//    // (xmin, ymin, xmax, ymax)
//    double leftViewport[4] = { 0.0, 0.0, 0.5, 1.0 };
//    double rightViewport[4] = { 0.5, 0.0, 1.0, 1.0 };
//
//    // Setup both renderers
//    vtkNew<vtkRenderer> leftRenderer;
//    renderWindow->AddRenderer(leftRenderer);
//    leftRenderer->SetViewport(leftViewport);
//    leftRenderer->SetBackground(colors->GetColor3d("Burlywood").GetData());
//
//    vtkNew<vtkRenderer> rightRenderer;
//    renderWindow->AddRenderer(rightRenderer);
//    rightRenderer->SetViewport(rightViewport);
//    rightRenderer->SetBackground(colors->GetColor3d("SlateGray").GetData());
//
//    vtkNew<vtkVertexGlyphFilter> glyphFilter;
//    glyphFilter->SetInputConnection(transformFilter->GetOutputPort());
//    glyphFilter->Update();
//
//    vtkNew<vtkPolyDataMapper> originalMapper;
//    originalMapper->SetInputConnection(glyphFilter->GetOutputPort());
//    vtkNew<vtkActor> originalActor;
//    originalActor->SetMapper(originalMapper);
//    originalActor->GetProperty()->SetPointSize(1);
//
//    // Map the points to spheres
//    auto sphereActor =
//        PointToGlyph(transformFilter->GetOutput()->GetPoints(), 0.007);
//    sphereActor->GetProperty()->SetColor(colors->GetColor3d("Blue").GetData());
//
//    // leftRenderer->AddActor(originalActor);
//    leftRenderer->AddActor(sphereActor);
//    leftRenderer->AddActor(vector1Actor);
//    leftRenderer->AddActor(vector2Actor);
//
//    rightRenderer->AddActor(projectedActor);
//
//    leftRenderer->ResetCamera();
//    rightRenderer->ResetCamera();
//
//    renderWindow->Render();
//    interactor->Start();
//
//    return EXIT_SUCCESS;
//}
//
//namespace {
//
//    vtkSmartPointer<vtkActor> PointToGlyph(vtkPoints* points, double const& scale)
//    {
//        auto bounds = points->GetBounds();
//        double maxLen = 0;
//        for (int i = 1; i < 3; ++i)
//        {
//            maxLen = std::max(bounds[i + 1] - bounds[i], maxLen);
//        }
//
//        vtkNew<vtkSphereSource> sphereSource;
//        sphereSource->SetRadius(scale * maxLen);
//
//        vtkNew<vtkPolyData> pd;
//        pd->SetPoints(points);
//
//        vtkNew<vtkGlyph3DMapper> mapper;
//        mapper->SetInputData(pd);
//        mapper->SetSourceConnection(sphereSource->GetOutputPort());
//        mapper->ScalarVisibilityOff();
//        mapper->ScalingOff();
//
//        vtkNew<vtkActor> actor;
//        actor->SetMapper(mapper);
//
//        return actor;
//    }
//
//} // namespace
#pragma endregion

#pragma region Threshhold
//#include <vtkCellArray.h>
//#include <vtkCellData.h>
//#include <vtkIntArray.h>
//#include <vtkNew.h>
//#include <vtkPointData.h>
//#include <vtkPoints.h>
//#include <vtkPolyData.h>
//#include <vtkThreshold.h>
//#include <vtkTriangle.h>
//#include <vtkUnstructuredGrid.h>
//
//int main(int, char* [])
//{
//    vtkNew<vtkPoints> points;
//    points->InsertNextPoint(0, 0, 0);
//    points->InsertNextPoint(1, 1, 1);
//    points->InsertNextPoint(2, 2, 2);
//    points->InsertNextPoint(3, 3, 3);
//    points->InsertNextPoint(4, 4, 4);
//
//    // Create three triangles
//    vtkNew<vtkTriangle> triangle0;
//    triangle0->GetPointIds()->SetId(0, 0);
//    triangle0->GetPointIds()->SetId(1, 1);
//    triangle0->GetPointIds()->SetId(2, 2);
//
//    vtkNew<vtkTriangle> triangle1;
//    triangle1->GetPointIds()->SetId(0, 1);
//    triangle1->GetPointIds()->SetId(1, 2);
//    triangle1->GetPointIds()->SetId(2, 3);
//
//    vtkNew<vtkTriangle> triangle2;
//    triangle2->GetPointIds()->SetId(0, 2);
//    triangle2->GetPointIds()->SetId(1, 3);
//    triangle2->GetPointIds()->SetId(2, 4);
//
//    // Add the triangles to a cell array
//    vtkNew<vtkCellArray> triangles;
//    triangles->InsertNextCell(triangle0);
//    triangles->InsertNextCell(triangle1);
//    triangles->InsertNextCell(triangle2);
//
//    // Setup index array
//    vtkNew<vtkIntArray> index;
//    index->SetNumberOfComponents(1);
//    index->SetName("index");
//    index->InsertNextValue(0);
//    index->InsertNextValue(1);
//    index->InsertNextValue(2);
//
//    // Add points, cells and index array to a polydata
//    vtkNew<vtkPolyData> polydata;
//    polydata->SetPoints(points);
//    polydata->SetPolys(triangles);
//    polydata->GetCellData()->AddArray(index);
//
//    std::cout << "There are " << polydata->GetNumberOfCells()
//        << " cells before thresholding." << std::endl;
//
//    vtkNew<vtkThreshold> threshold;
//    threshold->SetInputData(polydata);
//    threshold->SetLowerThreshold(1);
//    threshold->SetThresholdFunction(vtkThreshold::THRESHOLD_LOWER);
//    // doesn't work because the array is not added as SCALARS, i.e. via SetScalars
//    // threshold->SetInputArrayToProcess(0, 0, 0,
//    // vtkDataObject::FIELD_ASSOCIATION_CELLS, vtkDataSetAttributes::SCALARS); use
//    // like this:
//    threshold->SetInputArrayToProcess(
//        0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS, "index");
//    threshold->Update();
//
//    vtkUnstructuredGrid* thresholdedPolydata = threshold->GetOutput();
//    std::cout << "There are " << thresholdedPolydata->GetNumberOfCells()
//        << " cells after thresholding." << std::endl;
//
//    return EXIT_SUCCESS;
//}
#pragma endregion
