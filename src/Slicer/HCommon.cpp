#include "HCommon.h"

HAABB::HAABB(const HVector3& minPoint, const HVector3& maxPoint)
    : xyz(minPoint), XYZ(maxPoint)
{
    update();
}

void HAABB::InsertToPolyData(vtkSmartPointer<vtkPolyData> polyData)
{
    //cout << "InsertToPolyData min: " << xyz.x << ", " << xyz.y << ", " << xyz.z << endl << "\tmax: " << XYZ.x << ", " << XYZ.y << ", " << XYZ.z << endl;

    auto points = polyData->GetPoints();

    auto ixyz = points->InsertNextPoint((double*)&xyz);
    auto ixyZ = points->InsertNextPoint((double*)&xyZ);
    auto ixYz = points->InsertNextPoint((double*)&xYz);
    auto ixYZ = points->InsertNextPoint((double*)&xYZ);
    auto iXyz = points->InsertNextPoint((double*)&Xyz);
    auto iXyZ = points->InsertNextPoint((double*)&XyZ);
    auto iXYz = points->InsertNextPoint((double*)&XYz);
    auto iXYZ = points->InsertNextPoint((double*)&XYZ);

    auto polys = polyData->GetPolys();

    {
        vtkIdType ids[4] = { ixyz, ixyZ, iXyZ, iXyz };
        polys->InsertNextCell(4, ids);
    }

    {
        vtkIdType ids[4] = { ixYz, iXYz, iXYZ, ixYZ };
        polys->InsertNextCell(4, ids);
    }

    {
        vtkIdType ids[4] = { ixYz, ixYZ, ixyZ, ixyz };
        polys->InsertNextCell(4, ids);
    }

    {
        vtkIdType ids[4] = { iXyz, iXyZ, iXYZ, iXYz };
        polys->InsertNextCell(4, ids);
    }

    {
        vtkIdType ids[4] = { ixyZ, ixYZ, iXYZ, iXyZ };
        polys->InsertNextCell(4, ids);
    }

    {
        vtkIdType ids[4] = { ixyz, iXyz, iXYz, ixYz };
        polys->InsertNextCell(4, ids);
    }
}

bool HAABB::IntersectsTriangle(const HVector3& tp0, const HVector3& tp1, const HVector3& tp2)
{
    HVector3 v0 = { tp0.x - center.x, tp0.y - center.y, tp0.z - center.z };
    HVector3 v1 = { tp1.x - center.x, tp1.y - center.y, tp1.z - center.z };
    HVector3 v2 = { tp2.x - center.x, tp2.y - center.y, tp2.z - center.z };

    // Compute edge vectors for triangle
    HVector3 f0 = { tp1.x - tp0.x, tp1.y - tp0.y, tp1.z - tp0.z };
    HVector3 f1 = { tp2.x - tp1.x, tp2.y - tp1.y, tp2.z - tp1.z };
    HVector3 f2 = { tp0.x - tp2.x, tp0.y - tp2.y, tp0.z - tp2.z };

    //// region Test axes a00..a22 (category 3)

    // Test axis a00
    HVector3 a00 = { 0, -f0.z, f0.y };
    double p0 = v0.x * a00.x + v0.y * a00.y + v0.z * a00.z;
    double p1 = v1.x * a00.x + v1.y * a00.y + v1.z * a00.z;
    double p2 = v2.x * a00.x + v2.y * a00.y + v2.z * a00.z;
    double r = extents.y * std::fabs(f0.z) + extents.z * std::fabs(f0.y);
    if (std::max(-Trimax(p0, p1, p2), Trimin(p0, p1, p2)) > r + EPSILON)
        return false;

    // Test axis a01
    HVector3 a01 = { 0, -f1.z, f1.y };
    p0 = v0.x * a01.x + v0.y * a01.y + v0.z * a01.z;
    p1 = v1.x * a01.x + v1.y * a01.y + v1.z * a01.z;
    p2 = v2.x * a01.x + v2.y * a01.y + v2.z * a01.z;
    r = extents.y * std::fabs(f1.z) + extents.z * std::fabs(f1.y);
    if (std::max(-Trimax(p0, p1, p2), Trimin(p0, p1, p2)) > r + EPSILON)
        return false;

    // Test axis a02
    HVector3 a02 = { 0, -f2.z, f2.y };
    p0 = v0.x * a02.x + v0.y * a02.y + v0.z * a02.z;
    p1 = v1.x * a02.x + v1.y * a02.y + v1.z * a02.z;
    p2 = v2.x * a02.x + v2.y * a02.y + v2.z * a02.z;
    r = extents.y * std::fabs(f2.z) + extents.z * std::fabs(f2.y);
    if (std::max(-Trimax(p0, p1, p2), Trimin(p0, p1, p2)) > r + EPSILON)
        return false;

    // Test axis a10
    HVector3 a10 = { f0.z, 0, -f0.x };
    p0 = v0.x * a10.x + v0.y * a10.y + v0.z * a10.z;
    p1 = v1.x * a10.x + v1.y * a10.y + v1.z * a10.z;
    p2 = v2.x * a10.x + v2.y * a10.y + v2.z * a10.z;
    r = extents.x * std::fabs(f0.z) + extents.z * std::fabs(f0.x);
    if (std::max(-Trimax(p0, p1, p2), Trimin(p0, p1, p2)) > r + EPSILON)
        return false;

    // Test axis a11
    HVector3 a11 = { f1.z, 0, -f1.x };
    p0 = v0.x * a11.x + v0.y * a11.y + v0.z * a11.z;
    p1 = v1.x * a11.x + v1.y * a11.y + v1.z * a11.z;
    p2 = v2.x * a11.x + v2.y * a11.y + v2.z * a11.z;
    r = extents.x * std::fabs(f1.z) + extents.z * std::fabs(f1.x);
    if (std::max(-Trimax(p0, p1, p2), Trimin(p0, p1, p2)) > r + EPSILON)
        return false;

    // Test axis a12
    HVector3 a12 = { f2.z, 0, -f2.x };
    p0 = v0.x * a12.x + v0.y * a12.y + v0.z * a12.z;
    p1 = v1.x * a12.x + v1.y * a12.y + v1.z * a12.z;
    p2 = v2.x * a12.x + v2.y * a12.y + v2.z * a12.z;
    r = extents.x * std::fabs(f2.z) + extents.z * std::fabs(f2.x);
    if (std::max(-Trimax(p0, p1, p2), Trimin(p0, p1, p2)) > r + EPSILON)
        return false;

    // Test axis a20
    HVector3 a20 = { -f0.y, f0.x, 0 };
    p0 = v0.x * a20.x + v0.y * a20.y + v0.z * a20.z;
    p1 = v1.x * a20.x + v1.y * a20.y + v1.z * a20.z;
    p2 = v2.x * a20.x + v2.y * a20.y + v2.z * a20.z;
    r = extents.x * std::fabs(f0.y) + extents.y * std::fabs(f0.x);
    if (std::max(-Trimax(p0, p1, p2), Trimin(p0, p1, p2)) > r + EPSILON)
        return false;

    // Test axis a21
    HVector3 a21 = { -f1.y, f1.x, 0 };
    p0 = v0.x * a21.x + v0.y * a21.y + v0.z * a21.z;
    p1 = v1.x * a21.x + v1.y * a21.y + v1.z * a21.z;
    p2 = v2.x * a21.x + v2.y * a21.y + v2.z * a21.z;
    r = extents.x * std::fabs(f1.y) + extents.y * std::fabs(f1.x);
    if (std::max(-Trimax(p0, p1, p2), Trimin(p0, p1, p2)) > r + EPSILON)
        return false;

    // Test axis a22
    HVector3 a22 = { -f2.y, f2.x, 0 };
    p0 = v0.x * a22.x + v0.y * a22.y + v0.z * a22.z;
    p1 = v1.x * a22.x + v1.y * a22.y + v1.z * a22.z;
    p2 = v2.x * a22.x + v2.y * a22.y + v2.z * a22.z;
    r = extents.x * std::fabs(f2.y) + extents.y * std::fabs(f2.x);
    if (std::max(-Trimax(p0, p1, p2), Trimin(p0, p1, p2)) > r + EPSILON)
        return false;

    //// endregion

    //// region Test the three axes corresponding to the face normals of AABB b (category 1)

    // Exit if...
    // ... [-extents.X, extents.X] and [Min(v0.X,v1.X,v2.X), Max(v0.X,v1.X,v2.X)] do not overlap
    if (Trimax(v0.x, v1.x, v2.x) < -extents.x || Trimin(v0.x, v1.x, v2.x) > extents.x) {
        if (Trimax(v0.x, v1.x, v2.x) - (-extents.x) > EPSILON ||
            Trimin(v0.x, v1.x, v2.x) - (extents.x) > EPSILON) {
            return false;
        }
    }

    // ... [-extents.Y, extents.Y] and [Min(v0.Y,v1.Y,v2.Y), Max(v0.Y,v1.Y,v2.Y)] do not overlap
    if (Trimax(v0.y, v1.y, v2.y) < -extents.y || Trimin(v0.y, v1.y, v2.y) > extents.y) {
        if (Trimax(v0.y, v1.y, v2.y) - (-extents.y) > EPSILON ||
            Trimin(v0.y, v1.y, v2.y) - (extents.y) > EPSILON) {
            return false;
        }
    }

    // ... [-extents.Z, extents.Z] and [Min(v0.Z,v1.Z,v2.Z), Max(v0.Z,v1.Z,v2.Z)] do not overlap
    if (Trimax(v0.z, v1.z, v2.z) < -extents.z || Trimin(v0.z, v1.z, v2.z) > extents.z) {
        if (Trimax(v0.z, v1.z, v2.z) - (-extents.z) > EPSILON ||
            Trimin(v0.z, v1.z, v2.z) - (extents.z) > EPSILON) {
            return false;
        }
    }

    //// endregion

    //// region Test separating axis corresponding to triangle face normal (category 2)

    HVector3 plane_normal = { f0.y * f1.z - f0.z * f1.y, f0.z * f1.x - f0.x * f1.z, f0.x * f1.y - f0.y * f1.x };
    double plane_distance = std::fabs(plane_normal.x * v0.x + plane_normal.y * v0.y + plane_normal.z * v0.z);

    // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
    r = extents.x * std::fabs(plane_normal.x) + extents.y * std::fabs(plane_normal.y) + extents.z * std::fabs(plane_normal.z);

    // Intersection occurs when plane distance falls within [-r,+r] interval
    if (plane_distance > r + EPSILON)
        return false;

    //// endregion

    return true;
}

bool HAABB::IntersectsTriangle(const HVector3& tp0, const HVector3& tp1, const HVector3& tp2, const HVector3& tn)
{
    auto center = (tp0 + tp1 + tp2) / 3.0;
    auto dir0 = (xyz - center);
    auto dir1 = (xyZ - center);
    auto dir2 = (xYz - center);
    auto dir3 = (xYZ - center);
    auto dir4 = (Xyz - center);
    auto dir5 = (XyZ - center);
    auto dir6 = (XYz - center);
    auto dir7 = (XYZ - center);
    auto d0 = dir0 * tn;
    auto d1 = dir1 * tn;
    auto d2 = dir2 * tn;
    auto d3 = dir3 * tn;
    auto d4 = dir4 * tn;
    auto d5 = dir5 * tn;
    auto d6 = dir6 * tn;
    auto d7 = dir7 * tn;

    if ((d0 > EPSILON && d1 > EPSILON && d2 > EPSILON && d3 > EPSILON && d4 > EPSILON && d5 > EPSILON && d6 > EPSILON && d7 > EPSILON) ||
        (d0 < -EPSILON && d1 < -EPSILON && d2 < -EPSILON && d3 < -EPSILON && d4 < -EPSILON && d5 < -EPSILON && d6 < -EPSILON && d7 < -EPSILON))
    {
        return false;
    }
    else
    {
        return IntersectsTriangle(tp0, tp1, tp2);
    }
}

void GetCellPoints(vtkSmartPointer<vtkPolyData> polyData, vtkIdType cellId, HVector3& p0, HVector3& p1, HVector3& p2)
{
    auto cell = polyData->GetCell(cellId);
    auto points = cell->GetPoints();
    points->GetPoint(0, (double*)&p0);
    points->GetPoint(1, (double*)&p1);
    points->GetPoint(2, (double*)&p2);
}

HVector3 GetCellCenter(vtkSmartPointer<vtkPolyData> polyData, vtkIdType cellId)
{
    HVector3 p0, p1, p2;
    auto cell = polyData->GetCell(cellId);
    auto points = cell->GetPoints();
    points->GetPoint(0, (double*)&p0);
    points->GetPoint(1, (double*)&p1);
    points->GetPoint(2, (double*)&p2);
    return TriangleCentroid(p0, p1, p2);
}

void GetNeighborCellIds(vtkSmartPointer<vtkPolyData> polyData, vtkIdType cellId, std::list<vtkIdType>& neighborCellIds)
{
    vtkNew<vtkIdList> cellPointIds;
    polyData->GetCellPoints(cellId, cellPointIds);

    for (vtkIdType i = 0; i < cellPointIds->GetNumberOfIds(); i++)
    {
        vtkNew<vtkIdList> idList;
        idList->InsertNextId(cellPointIds->GetId(i));

        // get the neighbors of the cell
        vtkNew<vtkIdList> ncIds;

        polyData->GetCellNeighbors(cellId, idList, ncIds);

        for (vtkIdType j = 0; j < ncIds->GetNumberOfIds(); j++)
        {
            neighborCellIds.push_back(ncIds->GetId(j));
        }
    }
}

void GetConnectedCellIdsFromCell(vtkSmartPointer<vtkPolyData> polyData, vtkIdType cellId, std::set<vtkIdType>& connectedCellIds)
{
    auto currentCellId = cellId;
    std::stack<vtkIdType> nextCellIds;
    nextCellIds.push(cellId);
    while (nextCellIds.size() != 0)
    {
        currentCellId = nextCellIds.top();
        nextCellIds.pop();

        connectedCellIds.insert(currentCellId);

        std::list<vtkIdType> neighbors;
        GetNeighborCellIds(polyData, currentCellId, neighbors);

        for (auto& n : neighbors)
        {
            if (connectedCellIds.count(n) == 0)
            {
                nextCellIds.push(n);
            }
        }
    }
}

void GetConnectedCellIdsFromCellWithInDistance(vtkSmartPointer<vtkPolyData> polyData, vtkIdType cellId, double distance, std::set<vtkIdType>& connectedCellIds)
{
    auto cellCenter = GetCellCenter(polyData, cellId);

    auto currentCellId = cellId;
    std::stack<vtkIdType> nextCellIds;
    nextCellIds.push(cellId);
    while (nextCellIds.size() != 0)
    {
        currentCellId = nextCellIds.top();
        nextCellIds.pop();

        connectedCellIds.insert(currentCellId);

        std::list<vtkIdType> neighbors;
        GetNeighborCellIds(polyData, currentCellId, neighbors);

        for (auto& n : neighbors)
        {
            if (connectedCellIds.count(n) == 0)
            {
                HVector3 nc = GetCellCenter(polyData, n);
                auto d = HVector3::Distance(cellCenter, nc);
                if (d < distance)
                {
                    nextCellIds.push(n);
                }
            }
        }
    }
}

void GetConnectedCellIds(vtkSmartPointer<vtkPolyData> polyData, std::vector<std::set<vtkIdType>>& connectedCellIds, std::vector<int>& cellGroupIds, std::vector<double>& cellGroupAreas)
{
    connectedCellIds.clear();
    cellGroupIds.clear();
    cellGroupAreas.clear();

    std::set<vtkIdType> visited;
    auto noc = polyData->GetNumberOfCells();
    cellGroupIds.resize(noc);
    for (vtkIdType id = 0; id < noc; id++)
    {
        if (visited.count(id) != 0)
            continue;

        int groupId = connectedCellIds.size();
        connectedCellIds.push_back(std::set<vtkIdType>());
        auto& cellIds = connectedCellIds.back();
        GetConnectedCellIdsFromCell(polyData, id, cellIds);

        double totalArea = 0;
        for (auto& cid : cellIds)
        {
            visited.insert(cid);
            cellGroupIds[cid] = groupId;

            HVector3 p0, p1, p2;
            GetCellPoints(polyData, cid, p0, p1, p2);
            totalArea += TriangleArea(p0, p1, p2);
        }

        //cout << "totalArea : " << totalArea << endl;

        cellGroupAreas.push_back(totalArea);
    }
}

vtkSmartPointer<vtkPolyData> GetOverhangPolyData(vtkSmartPointer<vtkPolyData> modelData, double angleThreshhold)
{
    auto output = vtkSmartPointer<vtkPolyData>::New();

    std::vector<vtkIdType> cellsToCreate;

    auto cellDatas = modelData->GetCellData();
    auto cellNormals = cellDatas->GetNormals();
    for (size_t i = 0; i < modelData->GetNumberOfCells(); i++)
    {
        auto normal = cellNormals->GetTuple(i);

        double gravity[3] = { 0.0, 0.0, -1.0 };
        auto angle = vtkMath::DegreesFromRadians(vtkMath::AngleBetweenVectors(normal, gravity));

        if (angle < angleThreshhold)
        {
            //auto cell = modelData->GetCell(i);
            //double p0[3], p1[3], p2[3];
            //cell->GetPoints()->GetPoint(0, p0);
            //cell->GetPoints()->GetPoint(1, p1);
            //cell->GetPoints()->GetPoint(2, p2);
            cellsToCreate.push_back(i);
        }
    }

    std::map<vtkIdType, vtkIdType> pointIdMapping;

    vtkNew<vtkPoints> points;
    output->SetPoints(points);
    vtkNew<vtkCellArray> polys;
    output->SetPolys(polys);
    for (size_t i = 0; i < cellsToCreate.size(); i++)
    {
        auto oldCellId = cellsToCreate[i];
        auto oldCell = modelData->GetCell(oldCellId);
        auto pi0 = oldCell->GetPointId(0);
        auto pi1 = oldCell->GetPointId(1);
        auto pi2 = oldCell->GetPointId(2);
        double p0[3], p1[3], p2[3];
        if (pointIdMapping.count(pi0) == 0) {
            modelData->GetPoint(pi0, p0);
            auto npi0 = points->InsertNextPoint(p0);
            pointIdMapping[pi0] = npi0;
        }
        if (pointIdMapping.count(pi1) == 0) {
            modelData->GetPoint(pi1, p1);
            auto npi1 = points->InsertNextPoint(p1);
            pointIdMapping[pi1] = npi1;
        }
        if (pointIdMapping.count(pi2) == 0) {
            modelData->GetPoint(pi2, p2);
            auto npi2 = points->InsertNextPoint(p2);
            pointIdMapping[pi2] = npi2;
        }

        vtkIdType pids[3]{ pointIdMapping[pi0], pointIdMapping[pi1], pointIdMapping[pi2] };
        polys->InsertNextCell(3, pids);
    }

    return output;
}

void SplitPolyData(vtkSmartPointer<vtkPolyData> modelData, std::vector<vtkSmartPointer<vtkPolyData>>& result, std::vector<double>& groupAreas)
{
    result.clear();
    groupAreas.clear();

    std::vector<std::set<vtkIdType>> connectedCellIds;
    std::vector<int> cellGroupIds;
    GetConnectedCellIds(modelData, connectedCellIds, cellGroupIds, groupAreas);

    for (auto& group : connectedCellIds)
    {
        vtkNew<vtkPolyData> polyData;

        std::map<vtkIdType, vtkIdType> pointIdMapping;

        vtkNew<vtkPoints> points;
        polyData->SetPoints(points);
        vtkNew<vtkCellArray> polys;
        polyData->SetPolys(polys);

        for (auto& oldCellId : group)
        {
            auto oldCell = modelData->GetCell(oldCellId);
            auto pi0 = oldCell->GetPointId(0);
            auto pi1 = oldCell->GetPointId(1);
            auto pi2 = oldCell->GetPointId(2);
            double p0[3], p1[3], p2[3];
            if (pointIdMapping.count(pi0) == 0) {
                modelData->GetPoint(pi0, p0);
                auto npi0 = points->InsertNextPoint(p0);
                pointIdMapping[pi0] = npi0;
            }
            if (pointIdMapping.count(pi1) == 0) {
                modelData->GetPoint(pi1, p1);
                auto npi1 = points->InsertNextPoint(p1);
                pointIdMapping[pi1] = npi1;
            }
            if (pointIdMapping.count(pi2) == 0) {
                modelData->GetPoint(pi2, p2);
                auto npi2 = points->InsertNextPoint(p2);
                pointIdMapping[pi2] = npi2;
            }

            vtkIdType pids[3]{ pointIdMapping[pi0], pointIdMapping[pi1], pointIdMapping[pi2] };
            polys->InsertNextCell(3, pids);
        }

        vtkNew<vtkPolyDataNormals> polyDataNormals;
        polyDataNormals->SetInputData(polyData);
        polyDataNormals->ComputeCellNormalsOn();
        polyDataNormals->Update();

        result.push_back(polyDataNormals->GetOutput());
    }
}

void SplitPolyDataWithRemesh(vtkSmartPointer<vtkPolyData> modelData, double remeshEdgeLength, std::vector<vtkSmartPointer<vtkPolyData>>& result, std::vector<double>& groupAreas)
{
    vtkNew<vtkAdaptiveSubdivisionFilter> subdivisionFilter;
    subdivisionFilter->SetInputData(modelData);
    subdivisionFilter->SetMaximumEdgeLength(remeshEdgeLength);
    subdivisionFilter->Update();

    SplitPolyData(subdivisionFilter->GetOutput(), result, groupAreas);
}

std::vector<std::tuple<double, vtkIdType>> GetOverhangCellIds(vtkSmartPointer<vtkPolyData> polyData)
{
    std::vector<std::tuple<double, vtkIdType>> result;

    auto noc = polyData->GetNumberOfCells();
    auto cellData = polyData->GetCellData();
    auto cellNormals = cellData->GetNormals();
    for (vtkIdType id = 0; id < noc; id++)
    {
        auto cell = polyData->GetCell(id);
        auto normal = cellNormals->GetTuple(id);
        double gravity[3] = { 0.0, 0.0, -1.0 };
        auto angle = vtkMath::DegreesFromRadians(vtkMath::AngleBetweenVectors(normal, gravity));

        result.push_back(std::make_tuple(angle, id));
    }

    std::sort(result.begin(), result.end());

    return result;
}
