#ifndef TRAYCONTROLLER_H
#define TRAYCONTROLLER_H

#include <QSystemTrayIcon>

#include <QAction>
#include "noteinfo.h"
#include "fontselector.h"

class TrayController : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit TrayController(QObject *parent = 0);

    void readSavedData();

    void createOneNote( QRect,int,QString,int );
private:
    QVector<NoteInfo *> noteInfoArray;

    QMenu * menu;
    FontSelector * fontMenu;
    QString fontFamily;

    QAction * createOneAction;
    QAction * hideAllAction;
    QAction * disAllAction;
    QAction * saveAction;
    QAction * quitAction;

    void createContextMenu();

public slots:
    NoteInfo * createOneSlot();
    void hideAllSlot();
    void disAllSlot();
    void writeToSaveData();
    void quitSlot();

    void activatedSlot(QSystemTrayIcon::ActivationReason);

    void deletePointerSlot(NoteInfo*);

    void setNewFontFamily(QString);
};

#endif // TRAYCONTROLLER_H
