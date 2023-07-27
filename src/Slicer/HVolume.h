#pragma once

#include <math.h>
#include <float.h>
#include <vector>
#include <omp.h>

#include <set>

#include "vtk_header_files.h"

#include "HCommon.h"

class HVoxel : public HAABB
{
public:
	HVoxel(const HVector3& minPoint = { DBL_MAX, DBL_MAX, DBL_MAX }, const HVector3& maxPoint = { -DBL_MAX, -DBL_MAX, -DBL_MAX })
		: HAABB(minPoint, maxPoint)
	{
	}

	inline bool IsOccupied() const { return occupied; }
	inline void SetOccupied(bool isOccupied) { occupied = isOccupied; }

	inline const std::set<vtkIdType>& GetCellIds() const { return cellIds; }
	inline void SetCellId(vtkIdType id) { cellIds.insert(id); }

	inline double GetMinimumDistance() const { return minimumDistance; }
	inline void SetMinimumDistance(double distance) { minimumDistance = distance; }

	inline int GetMinimumDistanceCellId() const { return minimumDistanceCellId; }
	inline void SetMinimumDistanceCellId(int cellId) { minimumDistanceCellId = cellId; }

protected:
	bool occupied = false;

	std::set<vtkIdType> cellIds;

	double minimumDistance = DBL_MAX;
	int minimumDistanceCellId = -1;
};

struct HVolumeIndex {
	int x, y, z;
};

class HVolume : public HAABB
{
public:
	//HVolume(double voxelSize, const HVector3& minPoint, const HVector3& maxPoint);

	HVolume(double voxelSize, vtkSmartPointer<vtkPolyData> modelData);
	//HVolume(double voxelSize, vtkSmartPointer<vtkPolyData> modelData, vtkSmartPointer<vtkCellArray> cells);

	void Initialize(vtkSmartPointer<vtkPolyData> modelData);

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
	vtkSmartPointer<vtkPolyData> modelData = nullptr;
};
