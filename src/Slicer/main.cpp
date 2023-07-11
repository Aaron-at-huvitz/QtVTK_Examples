#include "QVTKWidgetWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QVTKWidgetWindow w;
    w.show();
    return a.exec();
}
