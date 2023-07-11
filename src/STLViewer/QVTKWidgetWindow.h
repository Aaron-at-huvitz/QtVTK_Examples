#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QVTKWidgetWindow.h"

class QVTKWidgetWindow : public QMainWindow
{
    Q_OBJECT

public:
    QVTKWidgetWindow(QWidget *parent = nullptr);
    ~QVTKWidgetWindow();

private:
    Ui::QVTKWidgetWindowClass ui;
};
