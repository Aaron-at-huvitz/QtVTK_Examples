#pragma once

#include <math.h>
#include <float.h>
#include <vector>

#include "vtk_header_files.h"

struct HVector3 {
	double x, y, z;
};

struct HTriangle {
	HVector3 v0, v1, v2;
};

class HAABB
{
public:
	HAABB(const HVector3& minPoint = { DBL_MAX, DBL_MAX, DBL_MAX }, const HVector3& maxPoint = { -DBL_MAX, -DBL_MAX, -DBL_MAX });

	inline const HVector3& GetMinPoint() const { return xyz; }
	inline const HVector3& GetMaxPoint() const { return XYZ; }
	inline const HVector3& GetCenter() const { return center; }
	inline const HVector3& GetExtents() const { return extents; }

	inline void SetMixMax(const HVector3& minPoint, const HVector3& maxPoint)
	{
		xyz = minPoint;
		XYZ = maxPoint;
		update();
	}

	inline void Expand(double x, double y, double z)
	{
		if (x < xyz.x) { xyz.x = x; }
		if (y < xyz.y) { xyz.y = y; }
		if (z < xyz.z) { xyz.z = z; }

		if (x > XYZ.x) { XYZ.x = x; }
		if (y > XYZ.y) { XYZ.y = y; }
		if (z > XYZ.z) { XYZ.z = z; }

		update();
	}

	inline void Expand(const HVector3& p)
	{
		if (p.x < xyz.x) { xyz.x = p.x; }
		if (p.y < xyz.y) { xyz.y = p.y; }
		if (p.z < xyz.z) { xyz.z = p.z; }
									    
		if (p.x > XYZ.x) { XYZ.x = p.x; }
		if (p.y > XYZ.y) { XYZ.y = p.y; }
		if (p.z > XYZ.z) { XYZ.z = p.z; }

		update();
	}

	inline double GetXLength() const { return XYZ.x - xyz.x; }
	inline double GetYLength() const { return XYZ.y - xyz.y; }
	inline double GetZLength() const { return XYZ.z - xyz.z; }

	inline bool Contains(const HVector3& p) const { return (xyz.x <= p.x && p.x <= XYZ.x) && (xyz.y <= p.y && p.y <= XYZ.y) && (xyz.z <= p.z && p.z <= XYZ.z); }

	inline bool Intersects(const HAABB& other) const
	{
		if ((xyz.x > other.XYZ.x) && (xyz.y > other.XYZ.y) && (xyz.z > other.XYZ.z)) { return false; }
		if ((XYZ.x < other.xyz.x) && (XYZ.y < other.xyz.y) && (XYZ.z < other.xyz.z)) { return false; }

		if (Contains(other.xyz)) return true;
		if (Contains(other.xyZ)) return true;
		if (Contains(other.xYz)) return true;
		if (Contains(other.xYZ)) return true;
		if (Contains(other.Xyz)) return true;
		if (Contains(other.XyZ)) return true;
		if (Contains(other.XYz)) return true;
		if (Contains(other.XYZ)) return true;

		if (other.Contains(xyz)) return true;
		if (other.Contains(xyZ)) return true;
		if (other.Contains(xYz)) return true;
		if (other.Contains(xYZ)) return true;
		if (other.Contains(Xyz)) return true;
		if (other.Contains(XyZ)) return true;
		if (other.Contains(XYz)) return true;
		if (other.Contains(XYZ)) return true;
	}

	void InsertToPolyData(vtkPolyData* polyData);

protected:
	HVector3 center = { 0.0,  0.0,  0.0 };
	HVector3 extents = { 0.0,  0.0,  0.0 };
	HVector3 xyz = {  DBL_MAX,  DBL_MAX,  DBL_MAX };
	HVector3 xyZ = {  DBL_MAX,  DBL_MAX, -DBL_MAX };
	HVector3 xYz = {  DBL_MAX, -DBL_MAX,  DBL_MAX };
	HVector3 xYZ = {  DBL_MAX, -DBL_MAX, -DBL_MAX };
	HVector3 Xyz = { -DBL_MAX,  DBL_MAX,  DBL_MAX };
	HVector3 XyZ = { -DBL_MAX,  DBL_MAX, -DBL_MAX };
	HVector3 XYz = { -DBL_MAX, -DBL_MAX,  DBL_MAX };
	HVector3 XYZ = { -DBL_MAX, -DBL_MAX, -DBL_MAX };

	inline void update()
	{
		xYz.x = xyz.x; xYz.y = XYZ.y; xYz.z = xyz.z;
		xyZ.x = xyz.x; xyZ.y = xyz.y; xyZ.z = XYZ.z;
		xYZ.x = xyz.x; xYZ.y = XYZ.y; xYZ.z = XYZ.z;
		Xyz.x = XYZ.x; Xyz.y = xyz.y; Xyz.z = xyz.z;
		XYz.x = XYZ.x; XYz.y = XYZ.y; XYz.z = xyz.z;
		XyZ.x = XYZ.x; XyZ.y = xyz.y; XyZ.z = XYZ.z;

		center.x = (xyz.x + XYZ.x) * 0.5;
		center.y = (xyz.y + XYZ.y) * 0.5;
		center.z = (xyz.z + XYZ.z) * 0.5;
		extents.x = XYZ.x - center.x;
		extents.y = XYZ.y - center.y;
		extents.z = XYZ.z - center.z;
	}
};

HVector3 CalculateNormal(const HVector3& vertexA, const HVector3& vertexB, const HVector3& vertexC);
bool OverlapOnAxis(const HVector3& axis, const HVector3& minVertex, const HVector3& maxVertex, const HAABB& box);
bool TriangleAABBIntersection(const HTriangle& triangle, const HAABB& box);
bool BoxIntersectsTriangle(const HAABB& bounds, const HVector3& triangle0, const HVector3& triangle1, const HVector3& triangle2);

class HVoxel : public HAABB
{
public:
	HVoxel(const HVector3& minPoint = { DBL_MAX, DBL_MAX, DBL_MAX }, const HVector3& maxPoint = { -DBL_MAX, -DBL_MAX, -DBL_MAX })
		: HAABB(minPoint, maxPoint)
	{
	}

	inline bool IsOccupied() const { return occupied; }
	inline void SetOccupied(bool isOccupied) { occupied = isOccupied; }

protected:
	bool occupied = false;
};

struct HVolumeIndex {
	int x, y, z;
};

class HVolume : public HAABB
{
public:
	//HVolume(double voxelSize, const HVector3& minPoint, const HVector3& maxPoint)
	//	: HAABB(minPoint, maxPoint), voxelSize(voxelSize)
	//{
	//	InitializeVTK();
	//}

	HVolume(double voxelSize, vtkPolyData* polyData)
		: voxelSize(voxelSize)
	{
		double bounds[6];
		polyData->GetBounds(bounds);

		Expand(bounds[0], bounds[2], bounds[4]);
		Expand(bounds[1], bounds[3], bounds[5]);

		InitializeVTK(polyData);
	}

	void InitializeVTK(vtkPolyData* polyData)
	{
		auto cells = polyData->GetPolys();
		auto noc = cells->GetNumberOfCells();
		auto points = polyData->GetPoints();
		auto nop = points->GetNumberOfPoints();

		resolutionX = ceil(GetXLength() / voxelSize);
		resolutionY = ceil(GetYLength() / voxelSize);
		resolutionZ = ceil(GetZLength() / voxelSize);

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

		for (size_t i = 0; i < noc; i++)
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
						if (BoxIntersectsTriangle(vaabb, p0, p1, p2)) {
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

	HVolumeIndex GetIndex(const HVector3& position)
	{
		int x = floor((position.x - xyz.x) / voxelSize);
		int y = floor((position.y - xyz.y) / voxelSize);
		int z = floor((position.z - xyz.z) / voxelSize);
		return HVolumeIndex{ x, y, z };
	}

	HVoxel& GetVoxel(int x, int y, int z)
	{
		return voxels[resolutionX * resolutionY * z + resolutionX * y + x];
	}

	HVoxel& GetVoxel(const HVolumeIndex& index)
	{
		return voxels[resolutionX * resolutionY * index.z + resolutionX * index.y + index.x];
	}

	HVoxel& GetVoxel(const HVector3& position)
	{
		int x = floor((position.x - xyz.x) / voxelSize);
		int y = floor((position.y - xyz.y) / voxelSize);
		int z = floor((position.z - xyz.z) / voxelSize);
		return voxels[resolutionX * resolutionY * z + resolutionX * y + x];
	}

	inline int GetResolutionX() const { return resolutionX; }
	inline int GetResolutionY() const { return resolutionY; }
	inline int GetResolutionZ() const { return resolutionZ; }

	inline vtkPolyData* GetPolyData() { return voxelsPolyData; }

protected:
	double voxelSize = 0.0;
	int resolutionX = 0;
	int resolutionY = 0;
	int resolutionZ = 0;

	std::vector<HVoxel> voxels;
	vtkPolyData* voxelsPolyData = nullptr;
};
