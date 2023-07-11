#include "QVTKWidget.h"

#include "vtk_header_files.h"

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
