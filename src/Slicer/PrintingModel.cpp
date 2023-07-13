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

struct Vector3 {
    float x, y, z;
};

struct Triangle {
    Vector3 vertexA, vertexB, vertexC;
};

struct AABB {
    Vector3 minPoint, maxPoint;
};

Vector3 CalculateNormal(const Vector3& vertexA, const Vector3& vertexB, const Vector3& vertexC) {
    Vector3 edgeAB{ vertexB.x - vertexA.x, vertexB.y - vertexA.y, vertexB.z - vertexA.z };
    Vector3 edgeAC{ vertexC.x - vertexA.x, vertexC.y - vertexA.y, vertexC.z - vertexA.z };

    Vector3 normal;
    normal.x = (edgeAB.y * edgeAC.z) - (edgeAB.z * edgeAC.y);
    normal.y = (edgeAB.z * edgeAC.x) - (edgeAB.x * edgeAC.z);
    normal.z = (edgeAB.x * edgeAC.y) - (edgeAB.y * edgeAC.x);

    // Normalize the normal vector
    float length = std::sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
    normal.x /= length;
    normal.y /= length;
    normal.z /= length;

    return normal;
}

bool OverlapOnAxis(const Vector3& axis, const Vector3& minVertex, const Vector3& maxVertex, const AABB& box) {
    float minIntervalA = (minVertex.x * axis.x + minVertex.y * axis.y + minVertex.z * axis.z);
    float maxIntervalA = (maxVertex.x * axis.x + maxVertex.y * axis.y + maxVertex.z * axis.z);

    float minIntervalB, maxIntervalB;

    // Project the AABB onto the axis
    if (axis.x > 0.0f) {
        minIntervalB = box.minPoint.x;
        maxIntervalB = box.maxPoint.x;
    }
    else {
        minIntervalB = box.maxPoint.x;
        maxIntervalB = box.minPoint.x;
    }

    if (maxIntervalA < minIntervalB || minIntervalA > maxIntervalB) {
        // The projections do not overlap, separating axis found
        return false;
    }

    return true;
}

bool TriangleAABBIntersection(const Triangle& triangle, const AABB& box) {
    // Convert the AABB into convex planes
    Vector3 faceNormals[6] = {
        {1.0f, 0.0f, 0.0f},  // Right
        {-1.0f, 0.0f, 0.0f}, // Left
        {0.0f, 1.0f, 0.0f},  // Top
        {0.0f, -1.0f, 0.0f}, // Bottom
        {0.0f, 0.0f, 1.0f},  // Front
        {0.0f, 0.0f, -1.0f}  // Back
    };

    // Check for overlap on each potential separating axis
    for (int i = 0; i < 6; i++) {
        if (!OverlapOnAxis(faceNormals[i], triangle.vertexA, triangle.vertexC, box)) {
            return false;
        }
    }

    // Calculate the triangle's normal
    Vector3 triangleNormal = CalculateNormal(triangle.vertexA, triangle.vertexB, triangle.vertexC);

    // Check for overlap with triangle's normal
    if (!OverlapOnAxis(triangleNormal, triangle.vertexA, triangle.vertexC, box)) {
        return false;
    }

    return true;
}

void Voxelize()
{
    Triangle triangle;
    triangle.vertexA = { 1.0f, 2.0f, 3.0f };
    triangle.vertexB = { 4.0f, 5.0f, 6.0f };
    triangle.vertexC = { 7.0f, 8.0f, 9.0f };

    AABB box;
    box.minPoint = { 0.0f, 0.0f, 0.0f };
    box.maxPoint = { 5.0f, 5.0f, 5.0f };

    if (TriangleAABBIntersection(triangle, box)) {
        std::cout << "Triangle and AABB intersect." << std::endl;
    }
    else {
        std::cout << "Triangle and AABB do not intersect." << std::endl;
    }
}