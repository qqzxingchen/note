#include <QApplication>
#include <QCoreApplication>

#include "traycontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TrayController * t = new TrayController();
    t->show();

    // 捕获 commitDataRequest(QSessionManager&) 消息
    // 使得程序得知系统将要关机或者注销
    QObject::connect (&a,SIGNAL(commitDataRequest(QSessionManager&)),
                      t,SLOT(quitSlot()));

    return a.exec();
}
