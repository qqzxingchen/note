#include <QApplication>

#include "traycontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TrayController * t = new TrayController();
    t->show();

    return a.exec();
}
