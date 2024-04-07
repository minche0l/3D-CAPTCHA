#include "SWProject.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SWProject w;
    w.show();
    return a.exec();
}
