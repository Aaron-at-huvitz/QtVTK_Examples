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

}

void HVisualDebugging::AddLine(double* p0, double* p1, unsigned char r, unsigned char g, unsigned char b)
{
	HVector3 v0{ p0[0], p0[1], p0[2] };
	HVector3 v1{ p1[0], p1[1], p1[2] };
	HVisualDebugging::AddLine(v0, v1, r, g, b);
}

void HVisualDebugging::AddLine(const HVector3& p0, const HVector3& p1, unsigned char r, unsigned char g, unsigned char b)
{
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	auto pi0 = points->InsertNextPoint((double*)&p0);
	auto pi1 = points->InsertNextPoint((double*)&p1);

	vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
	line->GetPointIds()->SetId(0, pi0);
	line->GetPointIds()->SetId(1, pi1);

	vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
	lines->InsertNextCell(line);

	vtkSmartPointer<vtkPolyData> linePolyData = vtkSmartPointer<vtkPolyData>::New();
	linePolyData->SetPoints(points);
	linePolyData->SetLines(lines);

	vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
	appendFilter->AddInputData(s_linePolyData);
	appendFilter->AddInputData(linePolyData);
	appendFilter->Update();

	s_linePolyData->ShallowCopy(appendFilter->GetOutput());
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
	vtkNew<vtkPoints> points;
	auto pi0 = points->InsertNextPoint((double*)&p0);
	auto pi1 = points->InsertNextPoint((double*)&p1);
	auto pi2 = points->InsertNextPoint((double*)&p2);

	vtkNew<vtkTriangle> triangle;
	triangle->GetPointIds()->SetId(0, pi0);
	triangle->GetPointIds()->SetId(1, pi1);
	triangle->GetPointIds()->SetId(2, pi2);

	vtkNew<vtkCellArray> triangles;
	triangles->InsertNextCell(triangle);

	vtkNew<vtkPolyData> trianglePolyData;
	trianglePolyData->SetPoints(points);
	trianglePolyData->SetPolys(triangles);

	vtkNew<vtkAppendPolyData> appendFilter;
	appendFilter->AddInputData(s_trianglePolyData);
	appendFilter->AddInputData(trianglePolyData);
	appendFilter->Update();

	s_trianglePolyData->ShallowCopy(appendFilter->GetOutput());
}

void HVisualDebugging::AddSphere(double* center, double radius, unsigned char r, unsigned char g, unsigned char b)
{
	HVector3 c{ center[0], center[1], center[2] };
	HVisualDebugging::AddSphere(c, radius, r, g, b);
}

void HVisualDebugging::AddSphere(const HVector3& center, double radius, unsigned char r, unsigned char g, unsigned char b)
{
	vtkNew<vtkSphereSource> sphereSource;
	sphereSource->SetCenter((double*)&center);
	sphereSource->SetRadius(radius);

	vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
	appendFilter->AddInputData(s_spherePolyData);
	appendFilter->AddInputConnection(sphereSource->GetOutputPort());
	appendFilter->Update();

	s_spherePolyData->ShallowCopy(appendFilter->GetOutput());
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
