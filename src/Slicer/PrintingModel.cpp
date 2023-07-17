#include "PrintingModel.h"

PrintingModel::PrintingModel(vtkRenderer* renderer)
    : renderer(renderer)
{
}

PrintingModel::~PrintingModel()
{
    this->ClearAll();
}

void PrintingModel::LoadModel(const QString& fileName)
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

    rawModelData = vtkPolyData::New();
    rawModelData->DeepCopy(reader->GetOutput());

    rawModelMapper = vtkPolyDataMapper::New();
    rawModelMapper->SetInputConnection(reader->GetOutputPort());

    rawModelActor = vtkActor::New();
    rawModelActor->SetMapper(rawModelMapper);

    double bounds[6];
    rawModelData->GetBounds(bounds);
    cout << "Model Bounds" << endl;
    cout << "xmin: " << bounds[0] << " xmax: " << bounds[1] <<
            " ymin: " << bounds[2] << " ymax: " << bounds[3] <<
            " zmin: " << bounds[4] << " zmax: " << bounds[5] << endl;
    cout << "xlength : " << bounds[1] - bounds[0] << ", ylength : " << bounds[3] - bounds[2] << ", zlength : " << bounds[5] - bounds[4] << endl;

    double center[3];
    rawModelData->GetCenter(center);
    cout << "Model Center : " << center[0] << ", " << center[1] << ", " << center[2] << endl;

    reader->Delete();
}

void PrintingModel::ClearAll()
{
    if (nullptr != volume)
    {
        delete volume;
        volume = nullptr;
    }

    ClearRemeshedModel();

    ClearOverhangModel();

    ClearVolumeModel();

    ClearRawModel();
}

void PrintingModel::ClearRemeshedModel()
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

void PrintingModel::ClearOverhangModel()
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

void PrintingModel::ClearVolumeModel()
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

void PrintingModel::ClearRawModel()
{
    if (nullptr != rawModelData)
    {
        rawModelData->Delete();
        rawModelData = nullptr;
    }

    if (nullptr != rawModelMapper)
    {
        rawModelMapper->Delete();
        rawModelMapper = nullptr;
    }

    if (nullptr != rawModelActor)
    {
        renderer->RemoveActor(rawModelActor);
        rawModelActor->Delete();
        rawModelActor = nullptr;
    }
}

void PrintingModel::Voxelize(double voxelSize)
{
    ClearVolumeModel();

    volumeModelData = vtkPolyData::New();
    vtkNew<vtkPoints> voxelsPoints;
    volumeModelData->SetPoints(voxelsPoints);
    vtkNew<vtkCellArray> voxelsQuads;
    volumeModelData->SetPolys(voxelsQuads);

    volume = new HVolume(voxelSize, rawModelData, volumeModelData);

    volumeModelMapper = vtkPolyDataMapper::New();
    volumeModelMapper->SetInputData(volumeModelData);

    volumeModelActor = vtkActor::New();
    volumeModelActor->SetMapper(volumeModelMapper);
    volumeModelActor->GetProperty()->SetRepresentationToWireframe();
    volumeModelActor->GetProperty()->SetColor(0.0, 0.5, 0.0);

    renderer->AddActor(volumeModelActor);
}

void PrintingModel::AnalyzeOverhang(bool faceNormal)
{
    ClearOverhangModel();

    overhangModelData = vtkPolyData::New();
    overhangModelData->DeepCopy(rawModelData);

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
    rawModelActor->VisibilityOff();
}

double PrintingModel::GetLongestEdgeLength()
{
    if (nullptr != rawModelData)
    {
        double edgeLength = 0.0;

        auto numberOfPolys = rawModelData->GetNumberOfCells();
        for (size_t i = 0; i < numberOfPolys; i++)
        {
            auto cell = rawModelData->GetCell(i);
            auto pi0 = cell->GetPointId(0);
            auto pi1 = cell->GetPointId(1);
            auto pi2 = cell->GetPointId(2);

            double p0[3], p1[3], p2[3];
            rawModelData->GetPoint(pi0, p0);
            rawModelData->GetPoint(pi1, p1);
            rawModelData->GetPoint(pi2, p2);

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

void PrintingModel::Remesh(double edgeLength)
{
    vtkNew<vtkAdaptiveSubdivisionFilter> subdivisionFilter;
    subdivisionFilter->SetInputData(rawModelData);
    subdivisionFilter->SetMaximumEdgeLength(edgeLength);
    subdivisionFilter->Update();

    if (nullptr != remeshedModelMapper)
    {
        remeshedModelMapper->Delete();
        remeshedModelMapper = nullptr;
    }

    if (nullptr != rawModelActor)
    {
        renderer->RemoveActor(rawModelActor);
        rawModelActor->Delete();
        rawModelActor = nullptr;
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
