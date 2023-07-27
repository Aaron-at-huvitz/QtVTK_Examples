#include "HVisualDebugging.h"

HVisualDebugging* HVisualDebugging::s_instance = nullptr;

vtkSmartPointer<vtkRenderer> HVisualDebugging::s_renderer = nullptr;
vtkSmartPointer<vtkRenderWindow> HVisualDebugging::s_renderWindow = nullptr;

vtkSmartPointer<vtkActor> HVisualDebugging::s_lineActor = nullptr;
vtkSmartPointer<vtkPolyDataMapper> HVisualDebugging::s_linePolyDataMapper = nullptr;
vtkSmartPointer<vtkPolyData> HVisualDebugging::s_linePolyData = nullptr;

vtkSmartPointer<vtkActor> HVisualDebugging::s_triangleActor = nullptr;
vtkSmartPointer<vtkPolyDataMapper> HVisualDebugging::s_trianglePolyDataMapper = nullptr;
vtkSmartPointer<vtkPolyData> HVisualDebugging::s_trianglePolyData = nullptr;

vtkSmartPointer<vtkActor> HVisualDebugging::s_sphereActor = nullptr;
vtkSmartPointer<vtkPolyDataMapper> HVisualDebugging::s_spherePolyDataMapper = nullptr;
vtkSmartPointer<vtkGlyph3D> HVisualDebugging::s_sphereGlyph3D = nullptr;
vtkSmartPointer<vtkPolyData> HVisualDebugging::s_spherePolyData = nullptr;

vtkSmartPointer<vtkActor> HVisualDebugging::s_cubeActor = nullptr;
vtkSmartPointer<vtkPolyDataMapper> HVisualDebugging::s_cubePolyDataMapper = nullptr;
vtkSmartPointer<vtkGlyph3D> HVisualDebugging::s_cubeGlyph3D = nullptr;
vtkSmartPointer<vtkPolyData> HVisualDebugging::s_cubePolyData = nullptr;

vtkSmartPointer<vtkActor> HVisualDebugging::s_arrowActor = nullptr;
vtkSmartPointer<vtkPolyDataMapper> HVisualDebugging::s_arrowPolyDataMapper = nullptr;
vtkSmartPointer<vtkPolyData> HVisualDebugging::s_arrowPolyData = nullptr;

std::vector<std::tuple<HVector3, HVector3, unsigned char, unsigned char, unsigned char>> HVisualDebugging::s_lineInfosToDraw;
std::vector<std::tuple<HVector3, HVector3, HVector3, unsigned char, unsigned char, unsigned char>> HVisualDebugging::s_triangleInfosToDraw;
std::vector<std::tuple<HVector3, double, unsigned char, unsigned char, unsigned char>> HVisualDebugging::s_sphereInfosToDraw;
std::vector<std::tuple<HVector3, double, unsigned char, unsigned char, unsigned char>> HVisualDebugging::s_cubeInfosToDraw;
std::vector<std::tuple<HVector3, unsigned char, unsigned char, unsigned char>> HVisualDebugging::s_arrowInfosToDraw;

HVisualDebugging::HVisualDebugging()
{

}

HVisualDebugging::~HVisualDebugging()
{

}

void HVisualDebugging::Initialize(vtkSmartPointer<vtkRenderer> renderer)
{
	if (nullptr == s_instance)
	{
		s_instance = new HVisualDebugging();
	}

	s_renderer = renderer;
	s_renderWindow = s_renderer->GetRenderWindow();

#pragma region Line
	{
		s_linePolyData = vtkSmartPointer<vtkPolyData>::New();
		s_linePolyDataMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		s_linePolyDataMapper->SetInputData(s_linePolyData);
		s_linePolyDataMapper->SetScalarModeToUsePointData();
		s_lineActor = vtkSmartPointer<vtkActor>::New();
		s_lineActor->SetMapper(s_linePolyDataMapper);

		vtkNew<vtkPoints> points;
		s_linePolyData->SetPoints(points);

		vtkNew<vtkCellArray> lines;
		s_linePolyData->SetLines(lines);

		vtkNew<vtkUnsignedCharArray> colors;
		colors->SetNumberOfComponents(3);
		s_linePolyData->GetCellData()->SetScalars(colors);
	
		s_renderer->AddActor(s_lineActor);
	}
#pragma endregion

#pragma region Triangle
	{
		s_trianglePolyData = vtkSmartPointer<vtkPolyData>::New();
		s_trianglePolyDataMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		s_trianglePolyDataMapper->SetInputData(s_trianglePolyData);
		s_trianglePolyDataMapper->SetScalarModeToUsePointData();
		s_triangleActor = vtkSmartPointer<vtkActor>::New();
		s_triangleActor->SetMapper(s_trianglePolyDataMapper);

		vtkNew<vtkPoints> points;
		s_trianglePolyData->SetPoints(points);

		vtkNew<vtkCellArray> triangles;
		s_trianglePolyData->SetPolys(triangles);

		vtkNew<vtkUnsignedCharArray> colors;
		colors->SetNumberOfComponents(3);
		s_trianglePolyData->GetCellData()->SetScalars(colors);
	
		s_renderer->AddActor(s_triangleActor);
	}
#pragma endregion

#pragma region Sphere
	{
		s_spherePolyData = vtkSmartPointer<vtkPolyData>::New();

		vtkNew<vtkPoints> points;
		s_spherePolyData->SetPoints(points);

		vtkNew<vtkDoubleArray> scales;
		scales->SetNumberOfComponents(1);
		scales->SetName("Scales");
		s_spherePolyData->GetPointData()->AddArray(scales);

		vtkNew<vtkUnsignedCharArray> colors;
		colors->SetName("Colors");
		colors->SetNumberOfComponents(3);
		s_spherePolyData->GetPointData()->AddArray(colors);

		vtkNew<vtkSphereSource> sphereSource;
		sphereSource->Update();

		s_sphereGlyph3D = vtkSmartPointer<vtkGlyph3D>::New();
		s_sphereGlyph3D->SetSourceConnection(sphereSource->GetOutputPort());
		s_sphereGlyph3D->SetInputData(s_spherePolyData);
		s_sphereGlyph3D->SetScaleModeToScaleByScalar();
		s_sphereGlyph3D->SetColorModeToColorByScalar();

		s_sphereGlyph3D->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "Scales");
		s_sphereGlyph3D->SetInputArrayToProcess(3, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "Colors");
		s_sphereGlyph3D->Update();

		s_spherePolyDataMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		s_spherePolyDataMapper->SetInputConnection(s_sphereGlyph3D->GetOutputPort());

		s_sphereActor = vtkSmartPointer<vtkActor>::New();
		s_sphereActor->SetMapper(s_spherePolyDataMapper);
		//s_sphereActor->GetProperty()->SetAmbient(1.0);
		//s_sphereActor->GetProperty()->SetDiffuse(0.0);

		s_renderer->AddActor(s_sphereActor);
	}
#pragma endregion

#pragma region Cube
	s_cubePolyData = vtkSmartPointer<vtkPolyData>::New();

	vtkNew<vtkPoints> points;
	s_cubePolyData->SetPoints(points);

	vtkNew<vtkDoubleArray> scales;
	scales->SetNumberOfComponents(1);
	scales->SetName("Scales");
	s_cubePolyData->GetPointData()->AddArray(scales);

	vtkNew<vtkUnsignedCharArray> colors;
	colors->SetName("Colors");
	colors->SetNumberOfComponents(3);
	s_cubePolyData->GetPointData()->AddArray(colors);

	vtkNew<vtkCubeSource> cubeSource;
	cubeSource->Update();

	s_cubeGlyph3D = vtkSmartPointer<vtkGlyph3D>::New();
	s_cubeGlyph3D->SetSourceConnection(cubeSource->GetOutputPort());
	s_cubeGlyph3D->SetInputData(s_cubePolyData);
	s_cubeGlyph3D->SetScaleModeToScaleByScalar();
	s_cubeGlyph3D->SetColorModeToColorByScalar();

	s_cubeGlyph3D->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "Scales");
	s_cubeGlyph3D->SetInputArrayToProcess(3, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "Colors");
	s_cubeGlyph3D->Update();

	s_cubePolyDataMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	s_cubePolyDataMapper->SetInputConnection(s_cubeGlyph3D->GetOutputPort());

	s_cubeActor = vtkSmartPointer<vtkActor>::New();
	s_cubeActor->SetMapper(s_cubePolyDataMapper);
	//s_cubeActor->GetProperty()->SetAmbient(1.0);
	//s_cubeActor->GetProperty()->SetDiffuse(0.0);

	s_renderer->AddActor(s_cubeActor);
#pragma endregion

#pragma region Arrow
	{
		s_arrowPolyData = vtkSmartPointer<vtkPolyData>::New();
		s_arrowPolyDataMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		s_arrowPolyDataMapper->SetInputData(s_arrowPolyData);
		s_arrowPolyDataMapper->SetScalarModeToUsePointData();
		s_arrowActor = vtkSmartPointer<vtkActor>::New();
		s_arrowActor->SetMapper(s_arrowPolyDataMapper);

		vtkNew<vtkPoints> points;
		s_arrowPolyData->SetPoints(points);

		vtkNew<vtkCellArray> triangles;
		s_arrowPolyData->SetPolys(triangles);

		vtkNew<vtkUnsignedCharArray> colors;
		colors->SetNumberOfComponents(3);
		s_arrowPolyData->GetCellData()->SetScalars(colors);

		s_renderer->AddActor(s_arrowActor);
	}
#pragma endregion
}

void HVisualDebugging::Terminate()
{
#pragma region Line
	if (nullptr != s_linePolyData)
	{
		s_linePolyData = nullptr;
	}

	if (nullptr != s_linePolyDataMapper)
	{
		s_linePolyDataMapper = nullptr;
	}

	if (nullptr != s_lineActor)
	{
		s_lineActor = nullptr;
	}
#pragma endregion

#pragma region Triangle
	if (nullptr != s_trianglePolyData)
	{
		s_trianglePolyData = nullptr;
	}

	if (nullptr != s_trianglePolyDataMapper)
	{
		s_trianglePolyDataMapper = nullptr;
	}

	if (nullptr != s_triangleActor)
	{
		s_triangleActor = nullptr;
	}
#pragma endregion

#pragma region Sphere
	if (nullptr != s_sphereGlyph3D)
	{
		s_sphereGlyph3D = nullptr;
	}

	if (nullptr != s_spherePolyData)
	{
		s_spherePolyData = nullptr;
	}

	if (nullptr != s_spherePolyDataMapper)
	{
		s_spherePolyDataMapper = nullptr;
	}

	if (nullptr != s_sphereActor)
	{
		s_sphereActor = nullptr;
	}
#pragma endregion

#pragma region Cube
	if (nullptr != s_cubeGlyph3D)
	{
		s_cubeGlyph3D = nullptr;
	}

	if (nullptr != s_cubePolyData)
	{
		s_cubePolyData = nullptr;
	}

	if (nullptr != s_cubePolyDataMapper)
	{
		s_cubePolyDataMapper = nullptr;
	}

	if (nullptr != s_cubeActor)
	{
		s_cubeActor = nullptr;
	}
#pragma endregion

#pragma region Arrow
	if (nullptr != s_arrowPolyData)
	{
		s_arrowPolyData = nullptr;
	}

	if (nullptr != s_arrowPolyDataMapper)
	{
		s_arrowPolyDataMapper = nullptr;
	}

	if (nullptr != s_arrowActor)
	{
		s_arrowActor = nullptr;
	}
#pragma endregion
}

void HVisualDebugging::Update()
{
	DrawLines();
	DrawTriangle();
	DrawSpheres();
	DrawCubes();
	DrawArrows();
}

void HVisualDebugging::DrawLines()
{
	if (s_lineInfosToDraw.empty()) return;

	vtkNew<vtkPoints> points;
	vtkNew<vtkCellArray> lines;
	vtkNew<vtkUnsignedCharArray> colors;
	colors->SetNumberOfComponents(3);
	
	for (auto& lineInfo : s_lineInfosToDraw)
	{
		auto p0 = std::get<0>(lineInfo);
		auto p1 = std::get<1>(lineInfo);
		auto r = std::get<2>(lineInfo);
		auto g = std::get<3>(lineInfo);
		auto b = std::get<4>(lineInfo);

		auto pi0 = points->InsertNextPoint((double*)&p0);
		auto pi1 = points->InsertNextPoint((double*)&p1);

		vtkIdType pids[] = {pi0, pi1};

		lines->InsertNextCell(2, pids);

		unsigned char uc[3]{ r, g, b };
		colors->InsertNextTypedTuple(uc);
		colors->InsertNextTypedTuple(uc);
	}

	vtkSmartPointer<vtkPolyData> linePolyData = vtkSmartPointer<vtkPolyData>::New();
	linePolyData->SetPoints(points);
	linePolyData->SetLines(lines);
	linePolyData->GetPointData()->SetScalars(colors);

	vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
	appendFilter->AddInputData(s_linePolyData);
	appendFilter->AddInputData(linePolyData);
	appendFilter->Update();

	s_linePolyData->ShallowCopy(appendFilter->GetOutput());

	s_lineInfosToDraw.clear();
}

void HVisualDebugging::DrawTriangle()
{
	if (s_triangleInfosToDraw.empty()) return;

	vtkNew<vtkPoints> points;
	vtkNew<vtkCellArray> triangles;
	vtkNew<vtkUnsignedCharArray> colors;
	colors->SetNumberOfComponents(3);

	for (auto& triangleInfo : s_triangleInfosToDraw)
	{
		auto p0 = std::get<0>(triangleInfo);
		auto p1 = std::get<1>(triangleInfo);
		auto p2 = std::get<2>(triangleInfo);
		auto r = std::get<3>(triangleInfo);
		auto g = std::get<4>(triangleInfo);
		auto b = std::get<5>(triangleInfo);

		auto pi0 = points->InsertNextPoint((double*)&p0);
		auto pi1 = points->InsertNextPoint((double*)&p1);
		auto pi2 = points->InsertNextPoint((double*)&p2);

		vtkIdType pids[] = { pi0, pi1, pi2 };

		triangles->InsertNextCell(3, pids);

		unsigned char uc[3]{ r, g, b };
		colors->InsertNextTypedTuple(uc);
		colors->InsertNextTypedTuple(uc);
		colors->InsertNextTypedTuple(uc);
	}

	vtkNew<vtkPolyData> trianglePolyData;
	trianglePolyData->SetPoints(points);
	trianglePolyData->SetPolys(triangles);
	trianglePolyData->GetPointData()->SetScalars(colors);

	vtkNew<vtkAppendPolyData> appendFilter;
	appendFilter->AddInputData(s_trianglePolyData);
	appendFilter->AddInputData(trianglePolyData);
	appendFilter->Update();

	s_trianglePolyData->ShallowCopy(appendFilter->GetOutput());

	s_triangleInfosToDraw.clear();
}

void HVisualDebugging::DrawSpheres()
{
	if (s_sphereInfosToDraw.empty()) return;

	auto points = s_spherePolyData->GetPoints();
	auto pointData = s_spherePolyData->GetPointData();
	vtkDoubleArray* scales = vtkDoubleArray::SafeDownCast(pointData->GetArray("Scales"));
	vtkUnsignedCharArray* colors = vtkUnsignedCharArray::SafeDownCast(pointData->GetArray("Colors"));

	for (auto& cubeInfo : s_sphereInfosToDraw)
	{
		auto center = std::get<0>(cubeInfo);
		auto scale = std::get<1>(cubeInfo);
		auto r = std::get<2>(cubeInfo);
		auto g = std::get<3>(cubeInfo);
		auto b = std::get<4>(cubeInfo);

		points->InsertNextPoint(center.xyz());
		scales->InsertNextValue(scale);
		unsigned char uc[3]{ r, g, b };
		colors->InsertNextTypedTuple(uc);
	}

	points->Modified();
	s_sphereGlyph3D->Update();

	s_renderWindow->Render();

	s_sphereInfosToDraw.clear();
}

void HVisualDebugging::DrawCubes()
{
	if (s_cubeInfosToDraw.empty()) return;

	auto points = s_cubePolyData->GetPoints();
	auto pointData = s_cubePolyData->GetPointData();
	vtkDoubleArray* scales = vtkDoubleArray::SafeDownCast(pointData->GetArray("Scales"));
	vtkUnsignedCharArray* colors = vtkUnsignedCharArray::SafeDownCast(pointData->GetArray("Colors"));

	for (auto& cubeInfo : s_cubeInfosToDraw)
	{
		auto center = std::get<0>(cubeInfo);
		auto scale = std::get<1>(cubeInfo);
		auto r = std::get<2>(cubeInfo);
		auto g = std::get<3>(cubeInfo);
		auto b = std::get<4>(cubeInfo);

		points->InsertNextPoint(center.xyz());
		scales->InsertNextValue(scale);
		unsigned char uc[3]{ r, g, b };
		colors->InsertNextTypedTuple(uc);
	}

	points->Modified();
	s_cubeGlyph3D->Update();

	s_renderWindow->Render();

	s_cubeInfosToDraw.clear();
}

void HVisualDebugging::DrawArrows()
{
	if (s_arrowInfosToDraw.empty()) return;

	vtkNew<vtkAppendPolyData> appendFilter;
	appendFilter->AddInputData(s_arrowPolyData);
	vtkNew<vtkUnsignedCharArray> colors;
	colors->SetNumberOfComponents(3);

	for (auto& arrowInfo : s_arrowInfosToDraw)
	{
		auto center = std::get<0>(arrowInfo);
		auto r = std::get<1>(arrowInfo);
		auto g = std::get<2>(arrowInfo);
		auto b = std::get<3>(arrowInfo);

		vtkNew<vtkArrowSource> arrowSource;
		//arrowSource->SetArrowOrigin((double*)&center);
		arrowSource->SetShaftRadius(0.25);
		arrowSource->SetTipRadius(0.5);
		arrowSource->Update();

		appendFilter->AddInputConnection(arrowSource->GetOutputPort());

		auto nop = arrowSource->GetOutput()->GetNumberOfPoints();
		for (size_t i = 0; i < nop; i++)
		{
			unsigned char uc[3]{ r, g, b };
			colors->InsertNextTypedTuple(uc);
			colors->InsertNextTypedTuple(uc);
			colors->InsertNextTypedTuple(uc);
		}
		arrowSource->GetOutput()->GetPointData()->SetScalars(colors);
	}
	appendFilter->Update();

	s_arrowPolyData->ShallowCopy(appendFilter->GetOutput());

	s_arrowInfosToDraw.clear();
}

void HVisualDebugging::AddLine(double* p0, double* p1, unsigned char r, unsigned char g, unsigned char b)
{
	HVector3 v0{ p0[0], p0[1], p0[2] };
	HVector3 v1{ p1[0], p1[1], p1[2] };
	HVisualDebugging::AddLine(v0, v1, r, g, b);
}

void HVisualDebugging::AddLine(const HVector3& p0, const HVector3& p1, unsigned char r, unsigned char g, unsigned char b)
{
	s_lineInfosToDraw.push_back(std::make_tuple(p0, p1, r, g, b));
}

void HVisualDebugging::AddTriangle(double* p0, double* p1, double* p2, unsigned char r, unsigned char g, unsigned char b)
{
	HVector3 v0{ p0[0], p0[1], p0[2] };
	HVector3 v1{ p1[0], p1[1], p1[2] };
	HVector3 v2{ p2[0], p2[1], p2[2] };
	HVisualDebugging::AddTriangle(v0, v1, v2, r, g, b);
}

void HVisualDebugging::AddTriangle(const HVector3& p0, const HVector3& p1, const HVector3& p2, unsigned char r, unsigned char g, unsigned char b)
{
	s_triangleInfosToDraw.push_back(std::make_tuple(p0, p1, p2, r, g, b));
}

void HVisualDebugging::AddSphere(double* center, double scale, unsigned char r, unsigned char g, unsigned char b)
{
	HVector3 c{ center[0], center[1], center[2] };
	HVisualDebugging::AddSphere(c, scale, r, g, b);
}

void HVisualDebugging::AddSphere(const HVector3& center, double scale, unsigned char r, unsigned char g, unsigned char b)
{
	s_sphereInfosToDraw.push_back(std::make_tuple(center, scale, r, g, b));
}

void HVisualDebugging::AddCube(double* center, double scale, unsigned char r, unsigned char g, unsigned char b)
{
	HVector3 c{ center[0], center[1], center[2] };
	HVisualDebugging::AddCube(c, scale, r, g, b);
}

void HVisualDebugging::AddCube(const HVector3& center, double scale, unsigned char r, unsigned char g, unsigned char b)
{
	s_cubeInfosToDraw.push_back(std::make_tuple(center, scale, r, g, b));
}

void HVisualDebugging::AddArrow(double* center, unsigned char r, unsigned char g, unsigned char b)
{
	HVector3 c{ center[0], center[1], center[2] };
	HVisualDebugging::AddArrow(c, r, g, b);
}

void HVisualDebugging::AddArrow(const HVector3& center, unsigned char r, unsigned char g, unsigned char b)
{
	s_arrowInfosToDraw.push_back(std::make_tuple(center, r, g, b));
}

void HVisualDebugging::ShowLines(bool bShow)
{
	if (nullptr != s_lineActor)
	{
		s_lineActor->SetVisibility(bShow);
		s_renderer->GetRenderWindow()->Render();
	}
}

void HVisualDebugging::ToggleLines()
{
	if (nullptr != s_lineActor)
	{
		s_lineActor->SetVisibility(!s_lineActor->GetVisibility());
		s_renderer->GetRenderWindow()->Render();
	}
}

void HVisualDebugging::ShowTriangles(bool bShow)
{
	if (nullptr != s_triangleActor)
	{
		s_triangleActor->SetVisibility(bShow);
		s_renderer->GetRenderWindow()->Render();
	}
}

void HVisualDebugging::ToggleTriangles()
{
	if (nullptr != s_triangleActor)
	{
		s_triangleActor->SetVisibility(!s_triangleActor->GetVisibility());
		s_renderer->GetRenderWindow()->Render();
	}
}

void HVisualDebugging::ShowSpheres(bool bShow)
{
	if (nullptr != s_sphereActor)
	{
		s_sphereActor->SetVisibility(bShow);
		s_renderer->GetRenderWindow()->Render();
	}
}

void HVisualDebugging::ToggleSpheres()
{
	if (nullptr != s_sphereActor)
	{
		s_sphereActor->SetVisibility(!s_sphereActor->GetVisibility());
		s_renderer->GetRenderWindow()->Render();
	}
}

void HVisualDebugging::ShowCubes(bool bShow)
{
	if (nullptr != s_cubeActor)
	{
		s_cubeActor->SetVisibility(bShow);
		s_renderer->GetRenderWindow()->Render();
	}
}

void HVisualDebugging::ToggleCubes()
{
	if (nullptr != s_cubeActor)
	{
		s_cubeActor->SetVisibility(!s_cubeActor->GetVisibility());
		s_renderer->GetRenderWindow()->Render();
	}
}

void HVisualDebugging::ShowArrows(bool bShow)
{
	if (nullptr != s_arrowActor)
	{
		s_arrowActor->SetVisibility(bShow);
		s_renderer->GetRenderWindow()->Render();
	}
}

void HVisualDebugging::ToggleArrows()
{
	if (nullptr != s_arrowActor)
	{
		s_arrowActor->SetVisibility(!s_arrowActor->GetVisibility());
		s_renderer->GetRenderWindow()->Render();
	}
}
