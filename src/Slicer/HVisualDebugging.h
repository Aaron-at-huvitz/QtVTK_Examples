#pragma once

#include "vtk_header_files.h"

class QVTKWidgetWindow;

class HVisualDebugging
{
public:
	static void AddLine(double* p0, double* p1, char r, char g, char b);
	static void AddSphere(double* center, double radius, char r, char g, char b);

private:
	HVisualDebugging();
	~HVisualDebugging();

	static void Initialize(vtkRenderer* renderer);

	static void Terminate();

	static HVisualDebugging* s_instance;

	static vtkRenderer* s_renderer;
	static vtkRenderWindow* s_renderWindow;
	static vtkRenderWindowInteractor* s_renderWindowInteractor;

	static vtkActor* s_lineActor;
	static vtkPolyDataMapper* s_linePolyDataMapper;
	static vtkPolyData* s_linePolyData;

public:
	friend class QVTKWidgetWindow;
};
