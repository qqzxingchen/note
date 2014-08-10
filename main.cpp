#include <QApplication>
#include <QCoreApplication>
#include <QMessageBox>

#include "traycontroller.h"
#include "singleapplication.h"

int main(int argc, char *argv[])
{
    SingleApplication * a = new SingleApplication(argc, argv,"note by xingchen");

    if(a->isRunning ())
    {
        a->sendMessage ("note by xingchen is running");
        QMessageBox::warning (0,"note by xingchen","note已经打开");
        return 0;
    }

    TrayController * t = new TrayController();
    t->show();

    // 捕获 commitDataRequest(QSessionManager&) 消息
    // 使得程序得知系统将要关机或者注销
    QObject::connect (a,SIGNAL(commitDataRequest(QSessionManager&)),
                      t,SLOT(quitSlot()));

    return a->exec();
}
