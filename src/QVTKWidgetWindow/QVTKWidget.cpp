#include "QVTKWidget.h"

#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkConeSource.h>
#include <vtkCubeSource.h>
#include <vtkDataObjectToTable.h>
#include <vtkElevationFilter.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkQtTableView.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkVersion.h>
#include <QVBoxLayout>

QVTKWidget::QVTKWidget(QWidget* parent)
	: QWidget(parent)
{
	vtkWidget = new QVTKOpenGLNativeWidget(this);

	QSizePolicy sizePolicy;
	sizePolicy.setHorizontalPolicy(QSizePolicy::Expanding);
	sizePolicy.setVerticalPolicy(QSizePolicy::Expanding);
	vtkWidget->setSizePolicy(sizePolicy);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(vtkWidget);
	this->setLayout(layout);

	//this->updateTimer = new QTimer(this);
	//this->updateTimer->setInterval(16);

	//connect(updateTimer, SIGNAL(timeout()), this, SLOT(update()));

	//this->updateTimer->start();
}

QVTKWidget::~QVTKWidget()
{
}

void QVTKWidget::update()
{
	//cout << "QVTKWidget::update()" << endl;
}
