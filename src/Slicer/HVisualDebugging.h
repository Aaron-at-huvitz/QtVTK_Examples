#pragma once

#include "vtk_header_files.h"

#include "HCommon.h"

class QVTKWidgetWindow;

class HVisualDebugging
{
public:
	static void AddLine(double* p0, double* p1, unsigned char r, unsigned char g, unsigned char b);
	static void AddLine(const HVector3& p0, const HVector3& p1, unsigned char r, unsigned char g, unsigned char b);

	static void AddTriangle(double* p0, double* p1, double* p2, unsigned char r, unsigned char g, unsigned char b);
	static void AddTriangle(const HVector3& p0, const HVector3& p1, const HVector3& p2, unsigned char r, unsigned char g, unsigned char b);

	static void AddSphere(double* center, double scale, unsigned char r, unsigned char g, unsigned char b);
	static void AddSphere(const HVector3& center, double scale, unsigned char r, unsigned char g, unsigned char b);

	static void AddCube(double* center, double scale, unsigned char r, unsigned char g, unsigned char b);
	static void AddCube(const HVector3& center, double scale, unsigned char r, unsigned char g, unsigned char b);

	static void AddArrow(double* position, unsigned char r, unsigned char g, unsigned char b);
	static void AddArrow(const HVector3& position, unsigned char r, unsigned char g, unsigned char b);

	static void Update();

private:
	HVisualDebugging();
	~HVisualDebugging();

	static void Initialize(vtkSmartPointer<vtkRenderer> renderer);

	static void Terminate();

	static HVisualDebugging* s_instance;

	static vtkSmartPointer<vtkRenderer> s_renderer;
	static vtkSmartPointer<vtkRenderWindow> s_renderWindow;

	static vtkSmartPointer<vtkActor> s_lineActor;
	static vtkSmartPointer<vtkPolyDataMapper> s_linePolyDataMapper;
	static vtkSmartPointer<vtkPolyData> s_linePolyData;

	static vtkSmartPointer<vtkActor> s_triangleActor;
	static vtkSmartPointer<vtkPolyDataMapper> s_trianglePolyDataMapper;
	static vtkSmartPointer<vtkPolyData> s_trianglePolyData;

	static vtkSmartPointer<vtkActor> s_sphereActor;
	static vtkSmartPointer<vtkPolyDataMapper> s_spherePolyDataMapper;
	static vtkSmartPointer<vtkGlyph3D> s_sphereGlyph3D;
	static vtkSmartPointer<vtkPolyData> s_spherePolyData;

	static vtkSmartPointer<vtkActor> s_cubeActor;
	static vtkSmartPointer<vtkPolyDataMapper> s_cubePolyDataMapper;
	static vtkSmartPointer<vtkGlyph3D> s_cubeGlyph3D;
	static vtkSmartPointer<vtkPolyData> s_cubePolyData;

	static vtkSmartPointer<vtkActor> s_arrowActor;
	static vtkSmartPointer<vtkPolyDataMapper> s_arrowPolyDataMapper;
	static vtkSmartPointer<vtkPolyData> s_arrowPolyData;

	static void DrawLines();
	static void DrawTriangle();
	static void DrawSpheres();
	static void DrawCubes();
	static void DrawArrows();

	static void ShowAll(bool show);
	static void ToggleAll();
	static void ToggleAllRepresentation();
	static void ShowLines(bool bShow);
	static void ToggleLines();
	static void ToggleLinesRepresentation();
	static void ShowTriangles(bool bShow);
	static void ToggleTriangles();
	static void ToggleTrianglesRepresentation();
	static void ShowSpheres(bool bShow);
	static void ToggleSpheres();
	static void ToggleSpheresRepresentation();
	static void ShowCubes(bool bShow);
	static void ToggleCubes();
	static void ToggleCubesRepresentation();
	static void ShowArrows(bool bShow);
	static void ToggleArrows();
	static void ToggleArrowsRepresentation();



	static std::vector<std::tuple<HVector3, HVector3, unsigned char, unsigned char, unsigned char>> s_lineInfosToDraw;
	static std::vector<std::tuple<HVector3, HVector3, HVector3, unsigned char, unsigned char, unsigned char>> s_triangleInfosToDraw;
	static std::vector<std::tuple<HVector3, double, unsigned char, unsigned char, unsigned char>> s_sphereInfosToDraw;
	static std::vector<std::tuple<HVector3, double, unsigned char, unsigned char, unsigned char>> s_cubeInfosToDraw;
	static std::vector<std::tuple<HVector3, unsigned char, unsigned char, unsigned char>> s_arrowInfosToDraw;

public:
	friend class QVTKWidgetWindow;
};
