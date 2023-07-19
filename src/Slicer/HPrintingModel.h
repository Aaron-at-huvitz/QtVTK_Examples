#pragma once

#include "vtk_header_files.h"

#include "HVolume.h"

class HPrintingModel
{
public:
	HPrintingModel(vtkRenderer* renderer);
	virtual ~HPrintingModel();

	void LoadModel(const QString& fileName);
	void ClearAll();

	void ClearRemeshedModel();
	void ClearOverhangModel();
	void ClearVolumeModel();
	void ClearInitialModel();

	void Voxelize(double voxelSize);

	void AnalyzeOverhang(bool faceNormal = true);
	void AnalyzeIsland();

	double GetLongestEdgeLength();
	void Remesh(double edgeLength);

	void Pick(double x, double y);

	void ShowInitialModel(bool bShow);
	void ToggleInitialModel();
	void ShowVolumeModel(bool bShow);
	void ToggleVolumeModel();
	void ShowOverhangModel(bool bShow);
	void ToggleOverhangModel();

	inline vtkPolyData* GetInitialModelData() const { return initialModelData; }
	inline vtkPolyDataMapper* GetInitialModelMapper() const { return initialModelMapper; }
	inline vtkActor* GetInitialModelActor() const { return initialModelActor; }

	inline vtkPolyData* GetRemeshedModelData() const { return remeshedModelData; }
	inline vtkPolyDataMapper* GetRemeshedModelMapper() const { return remeshedModelMapper; }
	inline vtkActor* GetRemeshedModelActor() const { return remeshedModelActor; }

	inline vtkPolyData* GetVolumeModelData() const { return volumeModelData; }
	inline vtkPolyDataMapper* GetVolumeModelMapper() const { return volumeModelMapper; }
	inline vtkActor* GetVolumeModelActor() const { return volumeModelActor; }

	inline vtkPolyData* GetOverhangModelData() const { return overhangModelData; }
	inline vtkPolyDataMapper* GetOverhangModelMapper() const { return overhangModelMapper; }
	inline vtkActor* GetOverhangModelActor() const { return overhangModelActor; }

protected:
	vtkRenderer* renderer = nullptr;
	
	vtkPolyData* initialModelData = nullptr;
	vtkPolyDataMapper* initialModelMapper = nullptr;
	vtkActor* initialModelActor = nullptr;

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
