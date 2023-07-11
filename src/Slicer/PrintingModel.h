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

	inline vtkPolyData* GetModelData() const { return modelData; }
	inline vtkPolyDataMapper* GetModelMapper() const { return modelMapper; }
	inline vtkActor* GetModelActor() const { return modelActor; }

protected:
	vtkRenderer* renderer = nullptr;
	vtkPolyData* modelData = nullptr;
	vtkPolyDataMapper* modelMapper = nullptr;
	vtkActor* modelActor = nullptr;
};
