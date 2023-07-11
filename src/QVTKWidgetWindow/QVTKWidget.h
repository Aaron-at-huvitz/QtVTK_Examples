#pragma once

#include <QTimer>
#include <QWidget>
#include <QVTKOpenGLNativeWidget.h>

class QVTKWidget :
    public QWidget
{
    Q_OBJECT

public:
    explicit QVTKWidget(QWidget* parent = nullptr);
    virtual ~QVTKWidget();

    inline QVTKOpenGLNativeWidget* GetVTKOpenGLNativeWidget() { return vtkWidget; }

public slots:
    void update();

protected:
    QVTKOpenGLNativeWidget* vtkWidget = nullptr;
    QTimer* updateTimer = nullptr;
};
