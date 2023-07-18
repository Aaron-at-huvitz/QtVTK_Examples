#include "QVoxelizationOptionDialog.h"

QVoxelizationOptionDialog::QVoxelizationOptionDialog(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.pushButtonVoxelize, SIGNAL(clicked()), this, SLOT(OnPushButtonVoxelize()));
	connect(ui.pushButtonCancel, SIGNAL(clicked()), this, SLOT(OnPushButtonCancel()));
}

QVoxelizationOptionDialog::~QVoxelizationOptionDialog()
{

}

void QVoxelizationOptionDialog::OnPushButtonVoxelize()
{
	voxelSize = ui.doubleSpinBox->value();
	voxelize = true;
	this->close();
}

void QVoxelizationOptionDialog::OnPushButtonCancel()
{
	this->close();
}