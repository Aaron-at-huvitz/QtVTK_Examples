#include "HVolume.h"

#define CUSTOM_MIN(a, b) (a) < (b) ? (a) : (b)
#define CUSTOM_MAX(a, b) (a) > (b) ? (a) : (b)

float Max(float a, float b, float c) {
    return std::max(std::max(a, b), c);
}

float Min(float a, float b, float c) {
    return std::min(std::min(a, b), c);
}

HAABB::HAABB(const HVector3& minPoint, const HVector3& maxPoint)
	: xyz(minPoint), XYZ(maxPoint)
{
	update();
}

void HAABB::InsertToPolyData(vtkPolyData* polyData)
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
    float p0 = v0.x * a00.x + v0.y * a00.y + v0.z * a00.z;
    float p1 = v1.x * a00.x + v1.y * a00.y + v1.z * a00.z;
    float p2 = v2.x * a00.x + v2.y * a00.y + v2.z * a00.z;
    float r = extents.y * std::abs(f0.z) + extents.z * std::abs(f0.y);
    if (std::max(-Max(p0, p1, p2), Min(p0, p1, p2)) > r)
        return false;

    // Test axis a01
    HVector3 a01 = { 0, -f1.z, f1.y };
    p0 = v0.x * a01.x + v0.y * a01.y + v0.z * a01.z;
    p1 = v1.x * a01.x + v1.y * a01.y + v1.z * a01.z;
    p2 = v2.x * a01.x + v2.y * a01.y + v2.z * a01.z;
    r = extents.y * std::abs(f1.z) + extents.z * std::abs(f1.y);
    if (std::max(-Max(p0, p1, p2), Min(p0, p1, p2)) > r)
        return false;

    // Test axis a02
    HVector3 a02 = { 0, -f2.z, f2.y };
    p0 = v0.x * a02.x + v0.y * a02.y + v0.z * a02.z;
    p1 = v1.x * a02.x + v1.y * a02.y + v1.z * a02.z;
    p2 = v2.x * a02.x + v2.y * a02.y + v2.z * a02.z;
    r = extents.y * std::abs(f2.z) + extents.z * std::abs(f2.y);
    if (std::max(-Max(p0, p1, p2), Min(p0, p1, p2)) > r)
        return false;

    // Test axis a10
    HVector3 a10 = { f0.z, 0, -f0.x };
    p0 = v0.x * a10.x + v0.y * a10.y + v0.z * a10.z;
    p1 = v1.x * a10.x + v1.y * a10.y + v1.z * a10.z;
    p2 = v2.x * a10.x + v2.y * a10.y + v2.z * a10.z;
    r = extents.x * std::abs(f0.z) + extents.z * std::abs(f0.x);
    if (std::max(-Max(p0, p1, p2), Min(p0, p1, p2)) > r)
        return false;

    // Test axis a11
    HVector3 a11 = { f1.z, 0, -f1.x };
    p0 = v0.x * a11.x + v0.y * a11.y + v0.z * a11.z;
    p1 = v1.x * a11.x + v1.y * a11.y + v1.z * a11.z;
    p2 = v2.x * a11.x + v2.y * a11.y + v2.z * a11.z;
    r = extents.x * std::abs(f1.z) + extents.z * std::abs(f1.x);
    if (std::max(-Max(p0, p1, p2), Min(p0, p1, p2)) > r)
        return false;

    // Test axis a12
    HVector3 a12 = { f2.z, 0, -f2.x };
    p0 = v0.x * a12.x + v0.y * a12.y + v0.z * a12.z;
    p1 = v1.x * a12.x + v1.y * a12.y + v1.z * a12.z;
    p2 = v2.x * a12.x + v2.y * a12.y + v2.z * a12.z;
    r = extents.x * std::abs(f2.z) + extents.z * std::abs(f2.x);
    if (std::max(-Max(p0, p1, p2), Min(p0, p1, p2)) > r)
        return false;

    // Test axis a20
    HVector3 a20 = { -f0.y, f0.x, 0 };
    p0 = v0.x * a20.x + v0.y * a20.y + v0.z * a20.z;
    p1 = v1.x * a20.x + v1.y * a20.y + v1.z * a20.z;
    p2 = v2.x * a20.x + v2.y * a20.y + v2.z * a20.z;
    r = extents.x * std::abs(f0.y) + extents.y * std::abs(f0.x);
    if (std::max(-Max(p0, p1, p2), Min(p0, p1, p2)) > r)
        return false;

    // Test axis a21
    HVector3 a21 = { -f1.y, f1.x, 0 };
    p0 = v0.x * a21.x + v0.y * a21.y + v0.z * a21.z;
    p1 = v1.x * a21.x + v1.y * a21.y + v1.z * a21.z;
    p2 = v2.x * a21.x + v2.y * a21.y + v2.z * a21.z;
    r = extents.x * std::abs(f1.y) + extents.y * std::abs(f1.x);
    if (std::max(-Max(p0, p1, p2), Min(p0, p1, p2)) > r)
        return false;

    // Test axis a22
    HVector3 a22 = { -f2.y, f2.x, 0 };
    p0 = v0.x * a22.x + v0.y * a22.y + v0.z * a22.z;
    p1 = v1.x * a22.x + v1.y * a22.y + v1.z * a22.z;
    p2 = v2.x * a22.x + v2.y * a22.y + v2.z * a22.z;
    r = extents.x * std::abs(f2.y) + extents.y * std::abs(f2.x);
    if (std::max(-Max(p0, p1, p2), Min(p0, p1, p2)) > r)
        return false;

    //// endregion

    //// region Test the three axes corresponding to the face normals of AABB b (category 1)

    // Exit if...
    // ... [-extents.X, extents.X] and [Min(v0.X,v1.X,v2.X), Max(v0.X,v1.X,v2.X)] do not overlap
    if (Max(v0.x, v1.x, v2.x) < -extents.x || Min(v0.x, v1.x, v2.x) > extents.x)
        return false;

    // ... [-extents.Y, extents.Y] and [Min(v0.Y,v1.Y,v2.Y), Max(v0.Y,v1.Y,v2.Y)] do not overlap
    if (Max(v0.y, v1.y, v2.y) < -extents.y || Min(v0.y, v1.y, v2.y) > extents.y)
        return false;

    // ... [-extents.Z, extents.Z] and [Min(v0.Z,v1.Z,v2.Z), Max(v0.Z,v1.Z,v2.Z)] do not overlap
    if (Max(v0.z, v1.z, v2.z) < -extents.z || Min(v0.z, v1.z, v2.z) > extents.z)
        return false;

    //// endregion

    //// region Test separating axis corresponding to triangle face normal (category 2)

    HVector3 plane_normal = { f0.y * f1.z - f0.z * f1.y, f0.z * f1.x - f0.x * f1.z, f0.x * f1.y - f0.y * f1.x };
    float plane_distance = std::abs(plane_normal.x * v0.x + plane_normal.y * v0.y + plane_normal.z * v0.z);

    // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
    r = extents.x * std::abs(plane_normal.x) + extents.y * std::abs(plane_normal.y) + extents.z * std::abs(plane_normal.z);

    // Intersection occurs when plane distance falls within [-r,+r] interval
    if (plane_distance > r)
        return false;

    //// endregion

    return true;
}

void HVolume::InitializeVTK(vtkPolyData* polyData)
{
	auto cells = polyData->GetPolys();
	auto noc = cells->GetNumberOfCells();
	auto points = polyData->GetPoints();
	auto nop = points->GetNumberOfPoints();

	resolutionX = ceil(GetXLength() / voxelSize);
	resolutionY = ceil(GetYLength() / voxelSize);
	resolutionZ = ceil(GetZLength() / voxelSize);

	cout << xyz.x << ", " << xyz.y << ", " << xyz.z << ", "
		<< XYZ.x << ", " << XYZ.y << ", " << XYZ.z << endl;

	cout << "Resolution x: " << resolutionX << ", y: " << resolutionY << ", z: " << resolutionZ << endl;

	voxels.resize(resolutionX * resolutionY * resolutionZ);

	voxelsPolyData = vtkPolyData::New();
	vtkNew<vtkPoints> voxelsPoints;
	voxelsPolyData->SetPoints(voxelsPoints);
	vtkNew<vtkCellArray> voxelsQuads;
	voxelsPolyData->SetPolys(voxelsQuads);

	vtkNew<vtkCellArray> pointCells;
	voxelsPolyData->SetVerts(pointCells);

	for (int z = 0; z < resolutionZ; z++)
	{
		for (int y = 0; y < resolutionY; y++)
		{
			for (int x = 0; x < resolutionX; x++)
			{
				auto min = HVector3({ x * voxelSize + xyz.x, y * voxelSize + xyz.y, z * voxelSize + xyz.z });
				auto max = HVector3({ (x + 1) * voxelSize + xyz.x, (y + 1) * voxelSize + xyz.y, (z + 1) * voxelSize + xyz.z });

				auto& voxel = GetVoxel(x, y, z);
				voxel.SetMixMax(min, max);
				//voxel.InsertToPolyData(voxelsPolyData);
			}
		}
	}

	for (int i = 0; i < noc; i++)
	{
		auto cell = polyData->GetCell(i);
		auto pi0 = cell->GetPointId(0);
		auto pi1 = cell->GetPointId(1);
		auto pi2 = cell->GetPointId(2);
		HVector3 p0, p1, p2;
		points->GetPoint(pi0, (double*)&p0);
		points->GetPoint(pi1, (double*)&p1);
		points->GetPoint(pi2, (double*)&p2);

		GetVoxel(p0).SetOccupied(true);
		GetVoxel(p1).SetOccupied(true);
		GetVoxel(p2).SetOccupied(true);


		HAABB taabb;
		taabb.Expand(p0);
		taabb.Expand(p1);
		taabb.Expand(p2);

		auto tminIndex = GetIndex(taabb.GetMinPoint());
		auto tmaxIndex = GetIndex(taabb.GetMaxPoint());

		bool intersected = false;
		for (size_t z = tminIndex.z; z <= tmaxIndex.z; z++)
		{
			for (size_t y = tminIndex.y; y <= tmaxIndex.y; y++)
			{
				for (size_t x = tminIndex.x; x <= tmaxIndex.x; x++)
				{
					auto& voxel = GetVoxel(x, y, z);
					if (voxel.IsOccupied()) {
						intersected = true;
						continue;
					}

					HAABB vaabb(voxel.GetMinPoint(), voxel.GetMaxPoint());
					if (voxel.IntersectsTriangle(p0, p1, p2)) {
						//cout << "Intersects : " << resolutionZ * z + resolutionY * y + x << endl;
						voxel.SetOccupied(true);
						intersected = true;
					}
				}
			}
		}
		if (intersected == false)
		{
			cout << "No intersects" << endl;
			cout << "WTF?" << endl;
		}
	}

	for (int z = 0; z < resolutionZ; z++)
	{
		for (int y = 0; y < resolutionY; y++)
		{
			for (int x = 0; x < resolutionX; x++)
			{
				auto& voxel = GetVoxel(x, y, z);
				if (voxel.IsOccupied())
				{
					voxel.InsertToPolyData(voxelsPolyData);

					//cout << "x : " << x << ", y : " << y << ", z : " << z << endl;
				}
			}
		}
	}
}