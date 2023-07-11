#pragma once

#include <vector>

#include "Qt_header_files.h"
#include "ui_QVTKWidgetWindow.h"

#include "vtk_header_files.h"

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

public slots:
    void OnMenuActionOpen();

public:
    void LoadModel(const QString& fileName);
};
