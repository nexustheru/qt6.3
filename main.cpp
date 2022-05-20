#include "widget.h"
#include "GL/glew.h"
#include "GL/glut.h"
#include <QApplication>
#include <QSurface>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    glutInit(&argc, argv);
    glewInit();

    Widget w;
    w.show();
    return a.exec();
}
