#include "PrintingModel.h"

PrintingModel::PrintingModel(vtkRenderer* renderer)
    : renderer(renderer)
{
}

PrintingModel::~PrintingModel()
{
    this->Clear();
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

    auto longestEdgeLength = GetLongestEdgeLength();
    cout << "longestEdgeLength : " << longestEdgeLength << endl;

    rawModelMapper = vtkPolyDataMapper::New();
    rawModelMapper->SetInputConnection(reader->GetOutputPort());

    rawModelActor = vtkActor::New();
    rawModelActor->SetMapper(rawModelMapper);

    reader->Delete();
}

void PrintingModel::Clear()
{
    if (nullptr != rawModelData)
    {
        rawModelData->Delete();
        rawModelData = nullptr;
    }

    if (nullptr != remeshedModelData)
    {
        remeshedModelData->Delete();
        remeshedModelData = nullptr;
    }

    if (nullptr != rawModelMapper)
    {
        rawModelMapper->Delete();
        rawModelMapper = nullptr;
    }

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

}
