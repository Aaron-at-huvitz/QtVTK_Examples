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
vtkSmartPointer<vtkPolyData> HVisualDebugging::s_spherePolyData = nullptr;

std::vector<std::tuple<HVector3, HVector3, unsigned char, unsigned char, unsigned char>> HVisualDebugging::s_lineInfosToDraw;
std::vector<std::tuple<HVector3, HVector3, HVector3, unsigned char, unsigned char, unsigned char>> HVisualDebugging::s_triangleInfosToDraw;
std::vector<std::tuple<HVector3, double, unsigned char, unsigned char, unsigned char>> HVisualDebugging::s_sphereInfosToDraw;

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
		s_spherePolyDataMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		s_spherePolyDataMapper->SetInputData(s_spherePolyData);
		s_spherePolyDataMapper->SetScalarModeToUsePointData();
		s_sphereActor = vtkSmartPointer<vtkActor>::New();
		s_sphereActor->SetMapper(s_spherePolyDataMapper);

		vtkNew<vtkPoints> points;
		s_spherePolyData->SetPoints(points);

		vtkNew<vtkCellArray> triangles;
		s_spherePolyData->SetPolys(triangles);

		vtkNew<vtkUnsignedCharArray> colors;
		colors->SetNumberOfComponents(3);
		s_spherePolyData->GetCellData()->SetScalars(colors);

		s_renderer->AddActor(s_sphereActor);
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
}

void HVisualDebugging::Update()
{
	DrawLines();
	DrawTriangle();
	DrawSpheres();
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
	
	vtkNew<vtkAppendPolyData> appendFilter;
	appendFilter->AddInputData(s_spherePolyData);
	vtkNew<vtkUnsignedCharArray> colors;
	colors->SetNumberOfComponents(3);

	for (auto& sphereInfo : s_sphereInfosToDraw)
	{
		auto center = std::get<0>(sphereInfo);
		auto radius = std::get<1>(sphereInfo);
		auto r = std::get<2>(sphereInfo);
		auto g = std::get<3>(sphereInfo);
		auto b = std::get<4>(sphereInfo);

		vtkNew<vtkSphereSource> sphereSource;
		sphereSource->SetCenter((double*)&center);
		sphereSource->SetRadius(radius);
		sphereSource->Update();
	
		appendFilter->AddInputConnection(sphereSource->GetOutputPort());

		auto nop = sphereSource->GetOutput()->GetNumberOfPoints();
		for (size_t i = 0; i < nop; i++)
		{
			unsigned char uc[3]{ r, g, b };
			colors->InsertNextTypedTuple(uc);
			colors->InsertNextTypedTuple(uc);
			colors->InsertNextTypedTuple(uc);
		}
		sphereSource->GetOutput()->GetPointData()->SetScalars(colors);
	}
	appendFilter->Update();

	s_spherePolyData->ShallowCopy(appendFilter->GetOutput());

	s_sphereInfosToDraw.clear();
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

void HVisualDebugging::AddSphere(double* center, double radius, unsigned char r, unsigned char g, unsigned char b)
{
	HVector3 c{ center[0], center[1], center[2] };
	HVisualDebugging::AddSphere(c, radius, r, g, b);
}

void HVisualDebugging::AddSphere(const HVector3& center, double radius, unsigned char r, unsigned char g, unsigned char b)
{
	s_sphereInfosToDraw.push_back(std::make_tuple(center, radius, r, g, b));
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
