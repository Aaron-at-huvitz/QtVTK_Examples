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

    modelData = vtkPolyData::New();
    modelData->DeepCopy(reader->GetOutput());

    auto longestEdgeLength = GetLongestEdgeLength();
    cout << "longestEdgeLength : " << longestEdgeLength << endl;

    modelMapper = vtkPolyDataMapper::New();
    modelMapper->SetInputConnection(reader->GetOutputPort());

    modelActor = vtkActor::New();
    modelActor->SetMapper(modelMapper);

    reader->Delete();
}

void PrintingModel::Clear()
{
    if (nullptr != modelData)
    {
        modelData->Delete();
        modelData = nullptr;
    }

    if (nullptr != modelMapper)
    {
        modelMapper->Delete();
        modelMapper = nullptr;
    }

    if (nullptr != modelActor)
    {
        renderer->RemoveActor(modelActor);
        modelActor->Delete();
        modelActor = nullptr;
    }
}

double PrintingModel::GetLongestEdgeLength()
{
    if (nullptr != modelData)
    {
        double edgeLength = 0.0;

        auto numberOfPolys = modelData->GetNumberOfCells();
        for (size_t i = 0; i < numberOfPolys; i++)
        {
            auto cell = modelData->GetCell(i);
            auto pi0 = cell->GetPointId(0);
            auto pi1 = cell->GetPointId(1);
            auto pi2 = cell->GetPointId(2);

            double p0[3], p1[3], p2[3];
            modelData->GetPoint(pi0, p0);
            modelData->GetPoint(pi1, p1);
            modelData->GetPoint(pi2, p2);

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
