#include "traycontroller.h"
#include "notedata.h"

#include <QGuiApplication>
#include <QMenu>
#include <QAction>
#include <QDir>
#include <QDirIterator>
#include <QTextStream>
#include <QTime>
#include <QFile>

#include <QDebug>

TrayController::TrayController(QObject *parent) :
    QSystemTrayIcon(parent)
{
    this->setIcon(QIcon("icon\\abiword.png"));
    this->setToolTip("便签管理器");
    fontFamily = QString("华文隶书");

    fontMenu = new FontSelector();

    createContextMenu();

    // 接受托盘图标点击消息
    connect ( this,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
              this,SLOT(activatedSlot(QSystemTrayIcon::ActivationReason)) );

    readSavedData();
}

void TrayController::createContextMenu()
{
    menu = new QMenu();

    createOneAction = new QAction( tr("新建一个便签"),this );
    hideAllAction = new QAction( tr("隐藏所有便签"),this );
    disAllAction = new QAction( tr("显示所有便签"),this );
    saveAction = new QAction( tr("保存便签数据"),this );
    quitAction = new QAction( tr("退出程序"),this );
    resumeDeletedAction = new QAction( tr("恢复删除的标签"),this );

    connect( this->createOneAction,SIGNAL(triggered()),
             this,SLOT(createOneSlot()) );
    connect( this->hideAllAction,SIGNAL(triggered()),
             this,SLOT(hideAllSlot()) );
    connect( this->disAllAction,SIGNAL(triggered()),
             this,SLOT(disAllSlot()) );
    connect( this->saveAction,SIGNAL(triggered()),this,SLOT(writeToSaveData()) );
    connect( this->quitAction,SIGNAL(triggered()),
             this,SLOT(quitSlot()) );
    connect( this->resumeDeletedAction,SIGNAL(triggered()),
             this,SLOT(resumeDeletedSlot()) );

    QMenu * font = fontMenu->initFontSelectMenu();
    connect ( fontMenu,SIGNAL(selectedFontFamily(QString)),
              this,SLOT( setNewFontFamily(QString) ));

    menu->addAction(createOneAction);
    menu->addSeparator();
    menu->addAction(disAllAction);
    menu->addAction(hideAllAction);
    menu->addSeparator();
    menu->addMenu(font);
    menu->addSeparator();
    menu->addAction(saveAction);
    menu->addAction(resumeDeletedAction);
    menu->addSeparator();
    menu->addAction(quitAction);

    setContextMenu(menu);
}

void TrayController::activatedSlot(QSystemTrayIcon::ActivationReason reason)
{
    if( reason == QSystemTrayIcon::Trigger )
    {
        for (int i = 0 ; i < noteInfoArray.count() ; i ++)
        {
            if( noteInfoArray.at(i)->isHidden() && !noteInfoArray.at(i)->getWaitToBeDeleted ())
            {
                disAllSlot();
                return ;
            }
        }
        hideAllSlot();
        return ;
    }
}

void TrayController::readSavedData()
{
    // 遍历data文件夹内所有的文件
    QDir dir(QString("data\\"));
    QStringList filters;
    filters << "*.xct";
    dir.setNameFilters(filters);
    QFileInfoList allFiles = dir.entryInfoList ();

    NoteData data;
    for(int i = 0;i<allFiles.size ();i++)
    {
        // 读取文件内的内容，并创建对应的noteInfo并赋值
        QFileInfo fileInfo = allFiles.at (i);
        data.clearData();
        data.readDataFromFile(fileInfo.filePath());
        createOneNote(
                    data.getStation(),
                    data.getPercent(),
                    data.getText(),
                    data.getFontSize(),
                    fileInfo.filePath()
        );
    }
    disAllSlot ();

    // 读取配置信息（只有全局字体信息）
    QFile file("conf\\config.xct");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return ;
    QTextStream in(&file);
    QString fontFamily = in.readLine();
    setNewFontFamily(fontFamily);
}
QString TrayController::createAnUnExistingFileName()
{
    QTime t;
    t= QTime::currentTime();
    qsrand(t.msec()+t.second()*1000+t.hour ()*100);

    QString fName = "data\\";
    for ( int i = 0 ; i < 128 ; i ++ )
        fName.append ( qrand() % 10 + '0' );
    fName.append (".xct");

    QFile file(fName);
    if( file.exists () )
        return createAnUnExistingFileName();
    else
        return fName;
}

void TrayController::writeToSaveData (bool isSaveDeletedNote)
{
    NoteInfo * note;
    NoteData data;

    for(int i = 0 ; i < noteInfoArray.count() ; i ++)
    {
        note = noteInfoArray.at(i);
        if( !note->getWaitToBeDeleted () || isSaveDeletedNote )
        {
            data.clearData();
            data.setStation( note->geometry() );
            data.setText( note->getNoteText() );
            data.setPercent( note->getWindowTransparent() );
            data.setFontSize( note->getFontSize() );
            data.writeDataToFile(note->getFileName ());
        }
        else
        {
            //删除文件、销毁内存、从链表中移除指针、重定位当前 i 值
            QFile::remove ( note->getFileName () );
            delete noteInfoArray.at (i);
            noteInfoArray.remove (i);
            i --;
        }
    }

    QFile file("conf\\config.xct");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return ;
    QTextStream out(&file);
    out << fontMenu->getCurrentFontFamily();
    file.close();
}

void TrayController::setNewFontFamily(QString fontFamily)
{
    this->fontFamily = fontFamily;
    for(int i = 0 ; i< noteInfoArray.count() ; i ++ )
    {
        noteInfoArray.at(i)->setFontFamily(fontFamily);
    }
}

void TrayController::createOneNote(QRect station,int percent,
                                   QString text,int fontSize,QString fileName)
{
    // 创建一个新的标签，但是不显示（读取完毕信息并设置后显示）
    NoteInfo * note = createOneSlot(false);
    note->setNoteText(text);
    note->setGeometry(station);
    note->setWindowTransparent(percent);
    note->setFontSize(fontSize);
    note->setFileName(fileName);
}

NoteInfo * TrayController::createOneSlot(bool isShow)
{
    NoteInfo * newOne = new NoteInfo();
    connect ( newOne,SIGNAL(deletePointer(NoteInfo*)),
              this,SLOT(deletePointerSlot(NoteInfo*)));
    connect ( newOne,SIGNAL(saveDataSignal()),this,SLOT(writeToSaveData()) );
    newOne->setFileName (createAnUnExistingFileName ());
    newOne->setFontFamily(fontFamily);

    if( isShow )
        newOne->show();

    noteInfoArray.append( newOne );
    return newOne;
}
void TrayController::deletePointerSlot(NoteInfo * pointer)
{
    for( int i = 0 ; i < noteInfoArray.count() ; i ++ )
    {
        if( noteInfoArray.at(i) == pointer )
        {
            noteInfoArray.at(i)->setWaitToBeDeleted (true);
            noteInfoArray.at(i)->hide ();
            return ;
        }
    }
}


void TrayController::hideAllSlot()
{
    for(int i = 0 ; i < noteInfoArray.count() ; i ++)
    {
        if( !(noteInfoArray.at(i)->getIsLocked()) )
            noteInfoArray.at(i)->hide();
    }
}
void TrayController::disAllSlot()
{
    for(int i = 0 ; i < noteInfoArray.count() ; i ++)
    {
        if(!noteInfoArray.at(i)->getWaitToBeDeleted ())
            noteInfoArray.at(i)->show();
    }
}
void TrayController::resumeDeletedSlot ()
{
    for(int i = 0 ; i < noteInfoArray.count() ; i ++)
    {
        noteInfoArray.at(i)->setWaitToBeDeleted (false);
        noteInfoArray.at(i)->show ();
    }
}
void TrayController::quitSlot()
{
    writeToSaveData (false);
    qApp->quit();
}



