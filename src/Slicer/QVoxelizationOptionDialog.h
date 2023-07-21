#pragma once

#include "Qt_header_files.h"

#include "ui_QVoxelizationOptionDialog.h"

class QVoxelizationOptionDialog :
    public QDialog
{
    Q_OBJECT

public:
    explicit QVoxelizationOptionDialog(QWidget* parent = nullptr);
    virtual ~QVoxelizationOptionDialog();

    bool Voxelize() { return voxelize; }
    double VoxelSize() { return voxelSize; }

public slots:
    void OnPushButton_0_1();
    void OnPushButton_0_2();
    void OnPushButton_0_25();
    void OnPushButtonVoxelize();
    void OnPushButtonCancel();

private:
    Ui::Dialog ui;

    bool voxelize = false;
    double voxelSize = 0.5;
};
