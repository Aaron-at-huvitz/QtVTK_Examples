#include "HVisualDebugging.h"

HVisualDebugging* HVisualDebugging::s_instance = nullptr;

vtkSmartPointer<vtkRenderer> HVisualDebugging::s_renderer;
vtkSmartPointer<vtkRenderWindow> HVisualDebugging::s_renderWindow;

vtkSmartPointer<vtkActor> HVisualDebugging::s_lineActor;
vtkSmartPointer<vtkPolyDataMapper> HVisualDebugging::s_linePolyDataMapper;
vtkSmartPointer<vtkPolyData> HVisualDebugging::s_linePolyData;

vtkSmartPointer<vtkActor> HVisualDebugging::s_triangleActor;
vtkSmartPointer<vtkPolyDataMapper> HVisualDebugging::s_trianglePolyDataMapper;
vtkSmartPointer<vtkPolyData> HVisualDebugging::s_trianglePolyData;

vtkSmartPointer<vtkAssembly> HVisualDebugging::s_sphereAssembly;

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

	{
		s_sphereAssembly = vtkSmartPointer<vtkAssembly>::New();
		s_renderer->AddActor(s_sphereAssembly);
	}
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


	if (nullptr != s_sphereAssembly)
	{
		s_sphereAssembly = nullptr;
	}
}

void HVisualDebugging::AddLine(double* p0, double* p1, unsigned char r, unsigned char g, unsigned char b)
{
	auto points = s_linePolyData->GetPoints();
	auto pi0 = points->InsertNextPoint(p0);
	auto pi1 = points->InsertNextPoint(p1);

	auto lines = s_linePolyData->GetLines();
	vtkIdType ids[2] = { pi0, pi1 };
	lines->InsertNextCell(2, ids);

	auto cellData = s_linePolyData->GetCellData();
	auto scalars = cellData->GetScalars();
	scalars->InsertNextTuple3(r, g, b);

	s_linePolyDataMapper->Update();
}

void HVisualDebugging::AddTriangle(double* p0, double* p1, double* p2, unsigned char r, unsigned char g, unsigned char b)
{
	cout << "p0: " << p0[0] << ", " << p0[1] << ", " << p0[2] << endl;

	auto points = s_trianglePolyData->GetPoints();
	auto pi0 = points->InsertNextPoint(p0);
	auto pi1 = points->InsertNextPoint(p1);
	auto pi2 = points->InsertNextPoint(p2);

	auto triangles = s_trianglePolyData->GetPolys();
	vtkIdType ids[3] = { pi0, pi1, pi2 };
	triangles->InsertNextCell(3, ids);

	auto cellData = s_trianglePolyData->GetCellData();
	auto scalars = cellData->GetScalars();
	scalars->InsertNextTuple3(r, g, b);
}

void HVisualDebugging::AddSphere(double* center, double radius, unsigned char r, unsigned char g, unsigned char b)
{
	vtkNew<vtkSphereSource> sphereSource;
	sphereSource->SetCenter(center);
	sphereSource->SetRadius(radius);

	vtkNew<vtkPolyDataMapper> sphereMapper;
	sphereMapper->SetInputConnection(sphereSource->GetOutputPort());

	vtkNew<vtkActor> sphereActor;
	sphereActor->SetMapper(sphereMapper);
	sphereActor->GetProperty()->SetColor(static_cast<double>(r) / 255.0, static_cast<double>(g) / 255.0, static_cast<double>(b) / 255.0);

	s_sphereAssembly->AddPart(sphereActor);
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
	if (nullptr != s_sphereAssembly)
	{
		s_sphereAssembly->SetVisibility(bShow);
		s_renderer->GetRenderWindow()->Render();
	}
}

void HVisualDebugging::ToggleSpheres()
{
	if (nullptr != s_sphereAssembly)
	{
		s_sphereAssembly->SetVisibility(!s_sphereAssembly->GetVisibility());
		s_renderer->GetRenderWindow()->Render();
	}
}
