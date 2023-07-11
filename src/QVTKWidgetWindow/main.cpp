#include "QVTKWidgetWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QVTKWidgetWindow w;
    w.show();
    return a.exec();
}
