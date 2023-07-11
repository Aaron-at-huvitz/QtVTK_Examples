#pragma once

#include <vector>

#include "Qt_header_files.h"
#include "ui_QVTKWidgetWindow.h"

#include "vtk_header_files.h"

class PrintingModel;

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

public slots:
    void OnMenuActionOpen();
    void OnSlider1ValueChaned(int value);
    void OnSlider2ValueChaned(int value);
    void OnSlider3ValueChaned(int value);

protected:
    PrintingModel* printingModel = nullptr;

public:
    void LoadModel(const QString& fileName);
};
