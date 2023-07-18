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
            auto position = initialModelData->GetPoint(i);
            auto zmin = position[2];
            auto zminIndex = i;
            int sameZCount = 0;
            for (auto& pi : lps)
            {
                auto p = initialModelData->GetPoint(pi);
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
        auto p = initialModelData->GetPoint(i);
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

    picker->Pick(x, y, 0, renderer);
    auto pickedActor = picker->GetActor();
    if (nullptr != pickedActor)
    {
        auto pickPosition = picker->GetPickPosition();
        auto cellId = picker->GetCellId();
        cout << "cellId:" << cellId << ", pickPosition x: " << pickPosition[0] << ", " << pickPosition[1] << ", " << pickPosition[2] << endl;
        HVisualDebugging::AddSphere(pickPosition, 1, 255, 0, 0);
    }
}
