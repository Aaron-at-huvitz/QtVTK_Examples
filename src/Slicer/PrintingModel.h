#pragma once

#include "vtk_header_files.h"

#include "HVolume.h"

class PrintingModel
{
public:
	PrintingModel(vtkRenderer* renderer);
	virtual ~PrintingModel();

	void LoadModel(const QString& fileName);
	void ClearAll();

	void ClearRemeshedModel();
	void ClearOverhangModel();
	void ClearVolumeModel();
	void ClearRawModel();

	void Voxelize(double voxelSize);

	void AnalyzeOverhang(bool faceNormal = true);

	double GetLongestEdgeLength();
	void Remesh(double edgeLength);

	inline vtkPolyData* GetRawModelData() const { return rawModelData; }
	inline vtkPolyDataMapper* GetRawModelMapper() const { return rawModelMapper; }
	inline vtkActor* GetRawModelActor() const { return rawModelActor; }

	inline vtkPolyData* GetRemeshedModelData() const { return remeshedModelData; }
	inline vtkPolyDataMapper* GetRemeshedModelMapper() const { return remeshedModelMapper; }
	inline vtkActor* GetRemeshedModelActor() const { return remeshedModelActor; }

	inline vtkPolyData* GetVolumeModelData() const { return volumeModelData; }
	inline vtkPolyDataMapper* GetVolumeModelMapper() const { return volumeModelMapper; }
	inline vtkActor* GetVolumeModelActor() const { return volumeModelActor; }

protected:
	vtkRenderer* renderer = nullptr;
	
	vtkPolyData* rawModelData = nullptr;
	vtkPolyDataMapper* rawModelMapper = nullptr;
	vtkActor* rawModelActor = nullptr;

	vtkPolyData* overhangModelData = nullptr;
	vtkPolyDataMapper* overhangModelMapper = nullptr;
	vtkActor* overhangModelActor = nullptr;

	vtkPolyData* remeshedModelData = nullptr;
	vtkPolyDataMapper* remeshedModelMapper = nullptr;
	vtkActor* remeshedModelActor = nullptr;

	vtkPolyData* volumeModelData = nullptr;
	vtkPolyDataMapper* volumeModelMapper = nullptr;
	vtkActor* volumeModelActor = nullptr;

	HVolume* volume = nullptr;
};
