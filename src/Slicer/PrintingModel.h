#pragma once

#include "vtk_header_files.h"

class PrintingModel
{
public:
	PrintingModel(vtkRenderer* renderer);
	virtual ~PrintingModel();

	void LoadModel(const QString& fileName);
	void Clear();

	double GetLongestEdgeLength();
	void Remesh(double edgeLength);

	inline vtkPolyData* GetRawModelData() const { return rawModelData; }
	inline vtkPolyData* GetRemeshedModelData() const { return remeshedModelData; }
	inline vtkPolyDataMapper* GetRawModelMapper() const { return rawModelMapper; }
	inline vtkPolyDataMapper* GetRemeshedModelMapper() const { return remeshedModelMapper; }
	inline vtkActor* GetRawModelActor() const { return rawModelActor; }
	inline vtkActor* GetRemeshedModelActor() const { return remeshedModelActor; }

protected:
	vtkRenderer* renderer = nullptr;
	vtkPolyData* rawModelData = nullptr;
	vtkPolyData* remeshedModelData = nullptr;
	vtkPolyDataMapper* rawModelMapper = nullptr;
	vtkPolyDataMapper* remeshedModelMapper = nullptr;
	vtkActor* rawModelActor = nullptr;
	vtkActor* remeshedModelActor = nullptr;
};
