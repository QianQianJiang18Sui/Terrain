#include "mainwindow.h"
#include "terrain_widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WidgetTerrain w;
    w.show();
    return a.exec();
}
