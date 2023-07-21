#include "QVoxelizationOptionDialog.h"

QVoxelizationOptionDialog::QVoxelizationOptionDialog(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.pushButton_0_1, SIGNAL(clicked()), this, SLOT(OnPushButton_0_1()));
	connect(ui.pushButton_0_2, SIGNAL(clicked()), this, SLOT(OnPushButton_0_2()));
	connect(ui.pushButton_0_25, SIGNAL(clicked()), this, SLOT(OnPushButton_0_25()));
	connect(ui.pushButtonVoxelize, SIGNAL(clicked()), this, SLOT(OnPushButtonVoxelize()));
	connect(ui.pushButtonCancel, SIGNAL(clicked()), this, SLOT(OnPushButtonCancel()));
}

QVoxelizationOptionDialog::~QVoxelizationOptionDialog()
{

}

void QVoxelizationOptionDialog::OnPushButton_0_1()
{
	ui.doubleSpinBox->setValue(0.1);
}

void QVoxelizationOptionDialog::OnPushButton_0_2()
{
	ui.doubleSpinBox->setValue(0.2);
}

void QVoxelizationOptionDialog::OnPushButton_0_25()
{
	ui.doubleSpinBox->setValue(0.25);
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