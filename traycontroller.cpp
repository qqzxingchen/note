#include "traycontroller.h"
#include "notedata.h"

#include <QGuiApplication>
#include <QMenu>
#include <QAction>
#include <QDir>
#include <QDirIterator>
#include <QTextStream>

TrayController::TrayController(QObject *parent) :
    QSystemTrayIcon(parent)
{
    this->setIcon(QIcon("icon\\abiword.png"));
    this->setToolTip("便签管理器");
    fontFamily = QString("华文隶书");

    fontMenu = new FontSelector();

    createContextMenu();

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

    connect( this->createOneAction,SIGNAL(triggered()),
             this,SLOT(createOneSlot()) );
    connect( this->hideAllAction,SIGNAL(triggered()),
             this,SLOT(hideAllSlot()) );
    connect( this->disAllAction,SIGNAL(triggered()),
             this,SLOT(disAllSlot()) );
    connect( this->saveAction,SIGNAL(triggered()),this,SLOT(writeToSaveData()) );
    connect( this->quitAction,SIGNAL(triggered()),
             this,SLOT(quitSlot()) );

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
    menu->addSeparator();
    menu->addAction(quitAction);

    setContextMenu(menu);
}

void TrayController::activatedSlot(QSystemTrayIcon::ActivationReason reason)
{
    if( reason == QSystemTrayIcon::Trigger
            && noteInfoArray.count() !=0 )
    {
        for (int i = 0 ; i < noteInfoArray.count() ; i ++)
        {
            if( noteInfoArray.at(i)->isHidden() )
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
    QDir dir(QString("data\\"));
    QStringList filters;
    filters << "*.xct";
    dir.setNameFilters(filters);
    QFileInfoList allFiles = dir.entryInfoList ();

    NoteData data;
    for(int i = 0;i<allFiles.size ();i++)
    {
        QFileInfo fileInfo = allFiles.at (i);
        data.clearData();
        data.readDataFromFile(fileInfo.filePath());
        createOneNote(data.getStation(),data.getPercent(),
                      data.getText(),data.getFontSize());
    }

    QFile file("conf\\config.xct");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return ;
    QTextStream in(&file);
    QString fontFamily = in.readLine();
    setNewFontFamily(fontFamily);
}
void TrayController::writeToSaveData ()
{
    QDirIterator it("data\\");
    while (it.hasNext())
    {
        QFile file;
        file.setFileName(it.next());
        file.remove();
    }

    NoteInfo * note;
    NoteData data;

    for(int i = 0 ; i < noteInfoArray.count() ; i ++)
    {
        note = noteInfoArray.at(i);
        data.clearData();
        data.setStation( note->geometry() );
        data.setText( note->getNoteText() );
        data.setPercent( note->getWindowTransparent() );
        data.setFontSize( note->getFontSize() );
        data.writeDataToFile(QString("data\\%1.xct").arg(i));
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
                                   QString text,int fontSize)
{
    NoteInfo * note = createOneSlot();
    note->setNoteText(text);
    note->setGeometry(station);
    note->setWindowTransparent(percent);
    note->setFontSize(fontSize);
}

NoteInfo * TrayController::createOneSlot()
{
    NoteInfo * newOne = new NoteInfo();
    connect ( newOne,SIGNAL(deletePointer(NoteInfo*)),
              this,SLOT(deletePointerSlot(NoteInfo*)));
    connect ( newOne,SIGNAL(saveDataSignal()),this,SLOT(writeToSaveData()) );
    newOne->setFontFamily(fontFamily);
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
            noteInfoArray.remove(i);
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
        noteInfoArray.at(i)->show();
    }
}
void TrayController::quitSlot()
{
    writeToSaveData ();
    qApp->quit();
}



