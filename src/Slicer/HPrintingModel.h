#pragma once

#include "vtk_header_files.h"

#include "HVolume.h"

class HPrintingModel
{
public:
	HPrintingModel(vtkSmartPointer<vtkRenderer> renderer);
	virtual ~HPrintingModel();

	void LoadModel(const QString& fileName);
	void ClearAll();

	void ClearRemeshedModel();
	void ClearOverhangModel();
	void ClearVolumeModel();
	void ClearInitialModel();

	void Voxelize(vtkSmartPointer<vtkPolyData> modelData, double voxelSize);

	void AnalyzeOverhang(double angleThreshhold, double reliefDistance);
	void AnalyzeIsland();

	double GetLongestEdgeLength();
	void Remesh(double edgeLength);

	void Pick(double x, double y);

	inline void ShowInitialModel(bool bShow) { ShowActor(renderer, initialModelActor, bShow); }
	inline void ToggleInitialModelVisibility() { ToggleActorVisibility(renderer, initialModelActor); }
	inline void ToggleInitialModelRepresentation() { ToggleActorRepresentation(renderer, initialModelActor); }
	
	inline void ShowVolumeModel(bool bShow) { ShowActor(renderer, volumeModelActor, bShow); }
	inline void ToggleVolumeModelVisibility() { ToggleActorVisibility(renderer, volumeModelActor); }
	inline void ToggleVolumeModelRepresentation() { ToggleActorRepresentation(renderer, volumeModelActor); }
	
	inline void ShowOverhangModel(bool bShow) { ShowActor(renderer, overhangModelActor, bShow); }
	inline void ToggleOverhangModelVisibility() { ToggleActorVisibility(renderer, overhangModelActor); }
	inline void ToggleOverhangModelRepresentation() { ToggleActorRepresentation(renderer, overhangModelActor); }

	inline vtkSmartPointer<vtkPolyData> GetInitialModelData() const { return initialModelData; }
	inline vtkSmartPointer<vtkPolyDataMapper> GetInitialModelMapper() const { return initialModelMapper; }
	inline vtkSmartPointer<vtkActor> GetInitialModelActor() const { return initialModelActor; }

	inline vtkSmartPointer<vtkPolyData> GetRemeshedModelData() const { return remeshedModelData; }
	inline vtkSmartPointer<vtkPolyDataMapper> GetRemeshedModelMapper() const { return remeshedModelMapper; }
	inline vtkSmartPointer<vtkActor> GetRemeshedModelActor() const { return remeshedModelActor; }

	inline vtkSmartPointer<vtkGlyph3D> GetVolumeModelData() const { return volumeModelData; }
	inline vtkSmartPointer<vtkPolyDataMapper> GetVolumeModelMapper() const { return volumeModelMapper; }
	inline vtkSmartPointer<vtkActor> GetVolumeModelActor() const { return volumeModelActor; }

	inline vtkSmartPointer<vtkPolyData> GetOverhangModelData() const { return overhangModelData; }
	inline vtkSmartPointer<vtkPolyDataMapper> GetOverhangModelMapper() const { return overhangModelMapper; }
	inline vtkSmartPointer<vtkActor> GetOverhangModelActor() const { return overhangModelActor; }

protected:
	vtkSmartPointer<vtkRenderer> renderer = nullptr;
	
	vtkSmartPointer<vtkPolyData> initialModelData = nullptr;
	vtkSmartPointer<vtkPolyDataMapper> initialModelMapper = nullptr;
	vtkSmartPointer<vtkActor> initialModelActor = nullptr;

	vtkSmartPointer<vtkPolyData> overhangModelData = nullptr;
	vtkSmartPointer<vtkPolyDataMapper> overhangModelMapper = nullptr;
	vtkSmartPointer<vtkActor> overhangModelActor = nullptr;
	vtkSmartPointer<vtkPolyDataConnectivityFilter> overhangModelDataConnectivityFilter = nullptr;

	vtkSmartPointer<vtkPolyData> remeshedModelData = nullptr;
	vtkSmartPointer<vtkPolyDataMapper> remeshedModelMapper = nullptr;
	vtkSmartPointer<vtkActor> remeshedModelActor = nullptr;

	vtkSmartPointer<vtkGlyph3D> volumeModelData = nullptr;
	vtkSmartPointer<vtkPolyDataMapper> volumeModelMapper = nullptr;
	vtkSmartPointer<vtkActor> volumeModelActor = nullptr;

	HVolume* volume = nullptr;
};
