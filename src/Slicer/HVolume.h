#pragma once

//#define FLT_EPSILON = 1.192093e-07
//#define DBL_EPSILON = 2.220446e-16
//#define LDBL_EPSILON = 1.084202e-19

#include <math.h>
#include <float.h>
#include <vector>
#include <omp.h>

#include <set>

#include "vtk_header_files.h"

#include "HCommon.h"

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

	inline bool Contains(const HVector3& p) const
	{
		return (xyz.x <= p.x && p.x <= XYZ.x) &&
			(xyz.y <= p.y && p.y <= XYZ.y) &&
			(xyz.z <= p.z && p.z <= XYZ.z);
	}

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

	void InsertToPolyData(vtkSmartPointer<vtkPolyData> polyData);

	bool IntersectsTriangle(const HVector3& tp0, const HVector3& tp1, const HVector3& tp2);
	bool IntersectsTriangle(const HVector3& tp0, const HVector3& tp1, const HVector3& tp2, const HVector3& tn);

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

class HVoxel : public HAABB
{
public:
	HVoxel(const HVector3& minPoint = { DBL_MAX, DBL_MAX, DBL_MAX }, const HVector3& maxPoint = { -DBL_MAX, -DBL_MAX, -DBL_MAX })
		: HAABB(minPoint, maxPoint)
	{
	}

	inline bool IsOccupied() const { return occupied; }
	inline void SetOccupied(bool isOccupied) { occupied = isOccupied; }

	inline void SetCellId(int id) { cellIds.insert(id); }

	std::set<int> cellIds;

protected:
	bool occupied = false;
};

struct HVolumeIndex {
	int x, y, z;
};

class HVolume : public HAABB
{
public:
	//HVolume(double voxelSize, const HVector3& minPoint, const HVector3& maxPoint);

	HVolume(double voxelSize, vtkSmartPointer<vtkPolyData> initialModelData);

	void Initialize(vtkSmartPointer<vtkPolyData> initialModelData);

	inline HVolumeIndex GetIndex(const HVector3& position)
	{
		int x = floor((position.x - xyz.x) / voxelSize);
		int y = floor((position.y - xyz.y) / voxelSize);
		int z = floor((position.z - xyz.z) / voxelSize);
		return HVolumeIndex{ x, y, z };
	}

	inline HVoxel& GetVoxel(int x, int y, int z)
	{
		return voxels[resolutionX * resolutionY * z + resolutionX * y + x];
	}

	inline HVoxel& GetVoxel(const HVolumeIndex& index)
	{
		return voxels[resolutionX * resolutionY * index.z + resolutionX * index.y + index.x];
	}

	inline HVoxel& GetVoxel(const HVector3& position)
	{
		int x = floor((position.x - xyz.x) / voxelSize);
		int y = floor((position.y - xyz.y) / voxelSize);
		int z = floor((position.z - xyz.z) / voxelSize);
		return voxels[resolutionX * resolutionY * z + resolutionX * y + x];
	}

	inline double GetVoxelSize() { return voxelSize; }

	inline int GetResolutionX() const { return resolutionX; }
	inline int GetResolutionY() const { return resolutionY; }
	inline int GetResolutionZ() const { return resolutionZ; }

	inline const std::vector<HVoxel>& GetVoxels() const { return voxels; }

protected:
	double voxelSize = 0.5;
	int resolutionX = 0;
	int resolutionY = 0;
	int resolutionZ = 0;

	std::vector<HVoxel> voxels;
	vtkSmartPointer<vtkPolyData> initialModelData = nullptr;
};
