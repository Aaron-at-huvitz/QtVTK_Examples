#pragma once

#include <vector>

#include "Qt_header_files.h"
#include "ui_QVTKWidgetWindow.h"

#include "vtk_header_files.h"

#include "QVoxelizationOptionDialog.h"

#include "HEventDispatcher.h"

class HPrintingModel;
class HVisualDebugging;
class CustomInteractorStyle;

class QVTKWidgetWindow : public QMainWindow
{
    Q_OBJECT

public:
    QVTKWidgetWindow(QWidget *parent = nullptr);
    ~QVTKWidgetWindow();

private:
    Ui::QVTKWidgetWindowClass ui;

    void InitializeVTK();
    void InitializeMenuBar();
    void InitializeSliders();

    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

public slots:
    void OnMenuActionOpen();
    void OnMenuActionFindOverhang();
    void OnMenuActionFindSupportPoint();
    void OnMenuActionVoxelize();
    void OnMenuActionFindIsland();
    void OnSlider1ValueChaned(int value);
    void OnSlider2ValueChaned(int value);
    void OnSlider3ValueChaned(int value);


protected:
    vtkSmartPointer<CustomInteractorStyle> customInteractorStyle = nullptr;

    HPrintingModel* printingModel = nullptr;
    QVoxelizationOptionDialog* voxelizationOptionDialog = nullptr;
    vtkSmartPointer<vtkOrientationMarkerWidget> orientationMarkerWidget = nullptr;

    QPoint lastMouseLButtonPosition;
    QPoint lastMouseRButtonPosition;
    QPoint lastMouseMButtonPosition;

    HEventDispatcher eventDispatcher;

public:
    void LoadModel(const QString& fileName);
};
