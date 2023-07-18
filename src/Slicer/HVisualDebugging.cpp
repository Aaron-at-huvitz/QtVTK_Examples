#include "HVisualDebugging.h"

HVisualDebugging* HVisualDebugging::s_instance = nullptr;

vtkRenderer* HVisualDebugging::s_renderer = nullptr;
vtkRenderWindow* HVisualDebugging::s_renderWindow = nullptr;
vtkRenderWindowInteractor* HVisualDebugging::s_renderWindowInteractor = nullptr;

vtkActor* HVisualDebugging::s_lineActor = nullptr;
vtkPolyDataMapper* HVisualDebugging::s_linePolyDataMapper = nullptr;
vtkPolyData* HVisualDebugging::s_linePolyData = nullptr;

HVisualDebugging::HVisualDebugging()
{

}

HVisualDebugging::~HVisualDebugging()
{

}

void HVisualDebugging::Initialize(vtkRenderer* renderer)
{
	if (nullptr == s_instance)
	{
		s_instance = new HVisualDebugging();
	}

	s_renderer = renderer;
	s_renderWindow = s_renderer->GetRenderWindow();
	s_renderWindowInteractor = s_renderWindow->GetInteractor();

	s_linePolyData = vtkPolyData::New();
	s_linePolyDataMapper = vtkPolyDataMapper::New();
	s_linePolyDataMapper->SetInputData(s_linePolyData);
	s_lineActor = vtkActor::New();
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

void HVisualDebugging::Terminate()
{
	if (nullptr != s_linePolyData)
	{
		s_linePolyData->Delete();
		s_linePolyData = nullptr;
	}

	if (nullptr != s_linePolyDataMapper)
	{
		s_linePolyDataMapper->Delete();
		s_linePolyDataMapper = nullptr;
	}

	if (nullptr != s_lineActor)
	{
		s_lineActor->Delete();
		s_lineActor = nullptr;
	}
}

void HVisualDebugging::AddLine(double* p0, double* p1, char r, char g, char b)
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
}

void HVisualDebugging::AddSphere(double* center, double radius, char r, char g, char b)
{
	vtkNew<vtkSphereSource> sphereSource;
	sphereSource->SetCenter(center);
	sphereSource->SetRadius(radius);

	vtkNew<vtkPolyDataMapper> sphereMapper;
	sphereMapper->SetInputConnection(sphereSource->GetOutputPort());

	vtkNew<vtkActor> sphereActor;
	sphereActor->SetMapper(sphereMapper);
	sphereActor->GetProperty()->SetColor((double)r, (double)g, (double)b);

	s_renderer->AddActor(sphereActor);
}
