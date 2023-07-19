#include "HPrintingModel.h"

#include "HVisualDebugging.h"

#include <map>
#include <set>

HPrintingModel::HPrintingModel(vtkRenderer* renderer)
    : renderer(renderer)
{
}

HPrintingModel::~HPrintingModel()
{
    this->ClearAll();
}

void HPrintingModel::LoadModel(const QString& fileName)
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

    initialModelData = vtkPolyData::New();
    initialModelData->DeepCopy(reader->GetOutput());

    initialModelMapper = vtkPolyDataMapper::New();
    initialModelMapper->SetInputConnection(reader->GetOutputPort());

    initialModelActor = vtkActor::New();
    initialModelActor->SetMapper(initialModelMapper);

    double bounds[6];
    initialModelData->GetBounds(bounds);
    cout << "Model Bounds" << endl;
    cout << "xmin: " << bounds[0] << " xmax: " << bounds[1] <<
            " ymin: " << bounds[2] << " ymax: " << bounds[3] <<
            " zmin: " << bounds[4] << " zmax: " << bounds[5] << endl;
    cout << "xlength : " << bounds[1] - bounds[0] << ", ylength : " << bounds[3] - bounds[2] << ", zlength : " << bounds[5] - bounds[4] << endl;

    double center[3];
    initialModelData->GetCenter(center);
    cout << "Model Center : " << center[0] << ", " << center[1] << ", " << center[2] << endl;

    reader->Delete();
}

void HPrintingModel::ClearAll()
{
    if (nullptr != volume)
    {
        delete volume;
        volume = nullptr;
    }

    ClearRemeshedModel();

    ClearOverhangModel();

    ClearVolumeModel();

    ClearInitialModel();
}

void HPrintingModel::ClearRemeshedModel()
{
    if (nullptr != remeshedModelData)
    {
        remeshedModelData->Delete();
        remeshedModelData = nullptr;
    }

    if (nullptr != remeshedModelMapper)
    {
        remeshedModelMapper->Delete();
        remeshedModelMapper = nullptr;
    }

    if (nullptr != remeshedModelActor)
    {
        renderer->RemoveActor(remeshedModelActor);
        remeshedModelActor->Delete();
        remeshedModelActor = nullptr;
    }
}

void HPrintingModel::ClearOverhangModel()
{
    if (nullptr != overhangModelData)
    {
        overhangModelData->Delete();
        overhangModelData = nullptr;
    }

    if (nullptr != overhangModelMapper)
    {
        overhangModelMapper->Delete();
        overhangModelMapper = nullptr;
    }

    if (nullptr != overhangModelActor)
    {
        renderer->RemoveActor(overhangModelActor);
        overhangModelActor->Delete();
        overhangModelActor = nullptr;
    }
}

void HPrintingModel::ClearVolumeModel()
{
    if (nullptr != volumeModelData)
    {
        volumeModelData->Delete();
        volumeModelData = nullptr;
    }

    if (nullptr != volumeModelMapper)
    {
        volumeModelMapper->Delete();
        volumeModelMapper = nullptr;
    }

    if (nullptr != volumeModelActor)
    {
        renderer->RemoveActor(volumeModelActor);
        volumeModelActor->Delete();
        volumeModelActor = nullptr;
    }
}

void HPrintingModel::ClearInitialModel()
{
    if (nullptr != initialModelData)
    {
        initialModelData->Delete();
        initialModelData = nullptr;
    }

    if (nullptr != initialModelMapper)
    {
        initialModelMapper->Delete();
        initialModelMapper = nullptr;
    }

    if (nullptr != initialModelActor)
    {
        renderer->RemoveActor(initialModelActor);
        initialModelActor->Delete();
        initialModelActor = nullptr;
    }
}

void HPrintingModel::Voxelize(double voxelSize)
{
    ClearVolumeModel();

    volumeModelData = vtkPolyData::New();
    vtkNew<vtkPoints> voxelsPoints;
    volumeModelData->SetPoints(voxelsPoints);
    vtkNew<vtkCellArray> voxelsQuads;
    volumeModelData->SetPolys(voxelsQuads);

    volume = new HVolume(voxelSize, initialModelData, volumeModelData);

    volumeModelMapper = vtkPolyDataMapper::New();
    volumeModelMapper->SetInputData(volumeModelData);

    volumeModelActor = vtkActor::New();
    volumeModelActor->SetMapper(volumeModelMapper);
    volumeModelActor->GetProperty()->SetRepresentationToWireframe();
    volumeModelActor->GetProperty()->SetColor(0.0, 0.5, 0.0);

    renderer->AddActor(volumeModelActor);
}

void HPrintingModel::AnalyzeOverhang(bool faceNormal)
{
    ClearOverhangModel();

    overhangModelData = vtkPolyData::New();
    overhangModelData->DeepCopy(initialModelData);

    vtkNew<vtkPolyDataNormals> normals;
    normals->SetInputData(overhangModelData);
    if (faceNormal)
    {
        normals->ComputeCellNormalsOn();
    }
    else
    {
        normals->ComputePointNormalsOn();
    }

    normals->Update();

    auto polyDataWithNormal = normals->GetOutput();

    vtkNew<vtkFloatArray> overhangIntensity;
    overhangIntensity->SetNumberOfComponents(1);
    overhangIntensity->SetName("OverhangIntensity");

    if (faceNormal)
    {
        auto cellDatas = polyDataWithNormal->GetCellData();
        auto cellNormals = cellDatas->GetNormals();
        for (size_t i = 0; i < polyDataWithNormal->GetNumberOfCells(); i++)
        {
            auto normal = cellNormals->GetTuple(i);

            double gravity[3] = { 0.0, 0.0, -1.0 };
            auto angle = vtkMath::DegreesFromRadians(vtkMath::AngleBetweenVectors(normal, gravity));
            //if (angle < 45) angle = 45;

            overhangIntensity->InsertNextValue(angle);
        }

        overhangModelData->GetCellData()->SetScalars(overhangIntensity);
    }
    else
    {
        auto pointDatas = polyDataWithNormal->GetPointData();
        auto pointNormals = pointDatas->GetNormals();
        for (size_t i = 0; i < polyDataWithNormal->GetNumberOfPoints(); i++)
        {
            auto normal = pointNormals->GetTuple(i);
            double gravity[3] = { 0.0, 0.0, -1.0 };
            auto angle = vtkMath::DegreesFromRadians(vtkMath::AngleBetweenVectors(normal, gravity));
            //if (angle < 45) angle = 45;

            overhangIntensity->InsertNextValue(angle);
        }

        overhangModelData->GetPointData()->SetScalars(overhangIntensity);
    }

    overhangModelMapper = vtkPolyDataMapper::New();
    overhangModelMapper->SetInputData(overhangModelData);
    overhangModelMapper->ScalarVisibilityOn();
    overhangModelMapper->SetColorModeToMapScalars();
    if (faceNormal)
    {
        overhangModelMapper->SetScalarModeToUseCellData();
    }
    else
    {
        overhangModelMapper->SetScalarModeToUsePointData();
    }

    vtkNew<vtkColorTransferFunction> colorTransferFunction;
    colorTransferFunction->AddRGBPoint(0, 1.0, 0.0, 0.0);
    colorTransferFunction->AddRGBPoint(45, 1.0, 1.0, 1.0);
    colorTransferFunction->AddRGBPoint(90, 1.0, 1.0, 1.0);
    overhangModelMapper->SetLookupTable(colorTransferFunction);

    overhangModelActor = vtkActor::New();
    overhangModelActor->SetMapper(overhangModelMapper);

    renderer->AddActor(overhangModelActor);
    initialModelActor->VisibilityOff();
}

void HPrintingModel::AnalyzeIsland()
{
    std::map<vtkIdType, std::set<vtkIdType>> linkedPoints;
    std::map<vtkIdType, vtkIdType> lowestVertex;

    auto noc = initialModelData->GetNumberOfCells();
    for (size_t i = 0; i < noc; i++)
    {
        auto cell = initialModelData->GetCell(i);
        auto points = cell->GetPointIds();

        auto pi0 = points->GetId(0);
        auto pi1 = points->GetId(1);
        auto pi2 = points->GetId(2);

        if (linkedPoints.count(pi0) == 0) {
            linkedPoints[pi0] = std::set<vtkIdType>();
        }
        linkedPoints[pi0].insert(pi1);
        linkedPoints[pi0].insert(pi2);

        if (linkedPoints.count(pi1) == 0) {
            linkedPoints[pi1] = std::set<vtkIdType>();
        }
        linkedPoints[pi1].insert(pi0);
        linkedPoints[pi1].insert(pi2);

        if (linkedPoints.count(pi2) == 0) {
            linkedPoints[pi2] = std::set<vtkIdType>();
        }
        linkedPoints[pi2].insert(pi0);
        linkedPoints[pi2].insert(pi1);
    }

    auto nop = initialModelData->GetNumberOfPoints();
    for (size_t i = 0; i < nop; i++)
    {
        if (linkedPoints.count(i) == 0)
        {
            cout << "??? i: " << i << endl;
            continue;
        }
        else
        {
            auto& lps = linkedPoints[i];
            double position[3];
            initialModelData->GetPoint(i, position);
            auto zmin = position[2];
            auto zminIndex = i;
            int sameZCount = 0;
            for (auto& pi : lps)
            {
                double p[3];
                initialModelData->GetPoint(pi, p);
                if (p[2] < zmin) {
                    zminIndex = pi;
                    zmin = p[2];
                }
            }
            lowestVertex[i] = zminIndex;
        }
    }

    std::set<vtkIdType> islandPoints;
    for (auto& kvp : linkedPoints)
    {
        auto currentId = kvp.first;
        auto nextId = lowestVertex[currentId];
        bool store = false;
        while (currentId != nextId)
        {
            currentId = nextId;
            nextId = lowestVertex[currentId];
        }
        islandPoints.insert(currentId);
    }

    vtkNew<vtkSphereSource> sphereSource;
    sphereSource->SetRadius(0.5);
    vtkNew<vtkPolyDataMapper> sphereMapper;
    sphereMapper->SetInputConnection(sphereSource->GetOutputPort());

    for (auto& i : islandPoints)
    {
        double p[3];
        initialModelData->GetPoint(i, p);
        vtkNew<vtkActor> sphereActor;
        sphereActor->SetMapper(sphereMapper);
        sphereActor->SetPosition(p);
        sphereActor->GetProperty()->SetColor(0, 0, 255);
        renderer->AddActor(sphereActor);
    }

    cout << "Total Island Points : " << islandPoints.size() << endl;
}

double HPrintingModel::GetLongestEdgeLength()
{
    if (nullptr != initialModelData)
    {
        double edgeLength = 0.0;

        auto numberOfPolys = initialModelData->GetNumberOfCells();
        for (size_t i = 0; i < numberOfPolys; i++)
        {
            auto cell = initialModelData->GetCell(i);
            auto pi0 = cell->GetPointId(0);
            auto pi1 = cell->GetPointId(1);
            auto pi2 = cell->GetPointId(2);

            double p0[3], p1[3], p2[3];
            initialModelData->GetPoint(pi0, p0);
            initialModelData->GetPoint(pi1, p1);
            initialModelData->GetPoint(pi2, p2);

            auto ll0 = vtkMath::Distance2BetweenPoints(p0, p1);
            auto ll1 = vtkMath::Distance2BetweenPoints(p1, p2);
            auto ll2 = vtkMath::Distance2BetweenPoints(p2, p0);

            if (edgeLength < ll0) edgeLength = ll0;
            if (edgeLength < ll1) edgeLength = ll1;
            if (edgeLength < ll2) edgeLength = ll2;
        }

        return sqrt(edgeLength);
    }

    return -1.0;
}

void HPrintingModel::Remesh(double edgeLength)
{
    vtkNew<vtkAdaptiveSubdivisionFilter> subdivisionFilter;
    subdivisionFilter->SetInputData(initialModelData);
    subdivisionFilter->SetMaximumEdgeLength(edgeLength);
    subdivisionFilter->Update();

    if (nullptr != remeshedModelMapper)
    {
        remeshedModelMapper->Delete();
        remeshedModelMapper = nullptr;
    }

    if (nullptr != initialModelActor)
    {
        renderer->RemoveActor(initialModelActor);
        initialModelActor->Delete();
        initialModelActor = nullptr;
    }

    if (nullptr != remeshedModelActor)
    {
        renderer->RemoveActor(remeshedModelActor);
        remeshedModelActor->Delete();
        remeshedModelActor = nullptr;
    }

    remeshedModelData = vtkPolyData::New();
    remeshedModelData->DeepCopy(subdivisionFilter->GetOutput());

    remeshedModelMapper = vtkPolyDataMapper::New();
    remeshedModelMapper->SetInputData(remeshedModelData);

    remeshedModelActor = vtkActor::New();
    remeshedModelActor->SetMapper(remeshedModelMapper);
}

void HPrintingModel::Pick(double x, double y)
{
    vtkNew<vtkCellPicker> picker;

    picker->PickFromListOn();
    picker->AddPickList(initialModelActor);
    picker->SetTolerance(0.0005);

    picker->Pick(x, y, 0, renderer);
    auto pickedActor = picker->GetActor();
    if (nullptr != pickedActor)
    {
        auto pickPosition = picker->GetPickPosition();
        auto cellId = picker->GetCellId();
        auto subId = picker->GetSubId();
        cout << "cellId: " << cellId << " subId: " << subId << endl;
        cout << "pickPosition: " << pickPosition[0] << ", " << pickPosition[1] << ", " << pickPosition[2] << endl;
        HVisualDebugging::AddSphere(pickPosition, 0.125, 255, 0, 0);


        auto cell = initialModelData->GetCell(cellId);
        auto points = cell->GetPoints();

        double p0[3], p1[3], p2[3];
        points->GetPoint(0, p0);
        points->GetPoint(1, p1);
        points->GetPoint(2, p2);

        cout << "p0 x: " << p0[0] << ", y:" << p0[1] << ", z: " << p0[2] << endl;
        cout << "p1 x: " << p1[0] << ", y:" << p1[1] << ", z: " << p1[2] << endl;
        cout << "p2 x: " << p2[0] << ", y:" << p2[1] << ", z: " << p2[2] << endl;
        
#pragma region Selection Display
        //vtkNew<vtkNamedColors> colors;
//vtkSmartPointer<vtkDataSetMapper> selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
//vtkSmartPointer<vtkActor>selectedActor = vtkSmartPointer<vtkActor>::New();

//vtkNew<vtkIdTypeArray> ids;
//ids->SetNumberOfComponents(1);
//ids->InsertNextValue(cellId);

//vtkNew<vtkSelectionNode> selectionNode;
//selectionNode->SetFieldType(vtkSelectionNode::CELL);
//selectionNode->SetContentType(vtkSelectionNode::INDICES);
//selectionNode->SetSelectionList(ids);

//vtkNew<vtkSelection> selection;
//selection->AddNode(selectionNode);

//vtkNew<vtkExtractSelection> extractSelection;
//extractSelection->SetInputData(0, initialModelData);
//extractSelection->SetInputData(1, selection);
//extractSelection->Update();

//// In selection
//vtkNew<vtkUnstructuredGrid> selected;
//selected->ShallowCopy(extractSelection->GetOutput());

//std::cout << "Number of points in the selection: "
//    << selected->GetNumberOfPoints() << std::endl;
//std::cout << "Number of cells in the selection : "
//    << selected->GetNumberOfCells() << std::endl;
//selectedMapper->SetInputData(selected);
//selectedActor->SetMapper(selectedMapper);
//selectedActor->GetProperty()->EdgeVisibilityOn();
//selectedActor->GetProperty()->SetColor(
//    colors->GetColor3d("Tomato").GetData());

//selectedActor->GetProperty()->SetLineWidth(3);

//for (size_t i = 0; i < selected->GetNumberOfPoints(); i++)
//{
//    auto p = selected->GetPoints()->GetPoint(i);
//    cout << "p x: " << p[0] << ", y:" << p[1] << ", z: " << p[2] << endl;
//}

//auto p0 = selected->GetPoints()->GetPoint(0);
//auto p1 = selected->GetPoints()->GetPoint(1);
//auto p2 = selected->GetPoints()->GetPoint(2);

//cout << "p0 x: " << p0[0] << ", y:" << p0[1] << ", z: " << p0[2] << endl;
//cout << "p1 x: " << p1[0] << ", y:" << p1[1] << ", z: " << p1[2] << endl;
//cout << "p2 x: " << p2[0] << ", y:" << p2[1] << ", z: " << p2[2] << endl;

//renderer->AddActor(selectedActor);
#pragma endregion

        HAABB aabb;
        aabb.Expand(p0[0], p0[1], p0[2]);
        aabb.Expand(p1[0], p1[1], p1[2]);
        aabb.Expand(p2[0], p2[1], p2[2]);

        auto minPoint = aabb.GetMinPoint();
        auto maxPoint = aabb.GetMaxPoint();

        cout << "minPoint x: " << minPoint.x << ", y:" << minPoint.y << ", z: " << minPoint.z << endl;
        cout << "maxPoint x: " << maxPoint.x << ", y:" << maxPoint.y << ", z: " << maxPoint.z << endl;

        auto minIndex = volume->GetIndex(aabb.GetMinPoint());
        auto maxIndex = volume->GetIndex(aabb.GetMaxPoint());

        cout << "minIndex x: " << minIndex.x << ", y:" << minIndex.y << ", z: " << minIndex.z << endl;
        cout << "maxIndex x: " << maxIndex.x << ", y:" << maxIndex.y << ", z: " << maxIndex.z << endl;

        cout << "x length: " << maxIndex.x - minIndex.x + 1 << ", y length: " << maxIndex.y - minIndex.y + 1 << ", z length: " << maxIndex.z - minIndex.z + 1;

        bool intersected = false;
        for (size_t z = minIndex.z; z <= maxIndex.z; z++)
        {
            for (size_t y = minIndex.y; y <= maxIndex.y; y++)
            {
                for (size_t x = minIndex.x; x <= maxIndex.x; x++)
                {
                    auto& voxel = volume->GetVoxel(x, y, z);
                    if (voxel.IsOccupied()) {
                        intersected = true;
                        //continue;
                    }

                    HVector3 tp0{ p0[0], p0[1], p0[2] };
                    HVector3 tp1{ p1[0], p1[1], p1[2] };
                    HVector3 tp2{ p2[0], p2[1], p2[2] };

                    if (voxel.IntersectsTriangle(tp0, tp1, tp2)) {
                        cout << "Intersects : " << x << ", " << y << ", " << z << endl;
                        voxel.SetOccupied(true);
                        voxel.SetCellId(cellId);
                        intersected = true;
                    }
                }
            }
        }
    }
}
