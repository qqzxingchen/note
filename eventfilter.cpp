#include "eventfilter.h"

#include <QEvent>
#include <QDebug>
#include <QKeyEvent>
#include <QPlainTextEdit>

EventFilter::EventFilter(QObject *parent) :
    QObject(parent)
{
}

bool EventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if( keyEvent->key() == 0x1000001 )
        {
            static_cast<QPlainTextEdit *>(obj)->insertPlainText("    ");
            return true;
        }
    }

    return QObject::eventFilter(obj, event);
}
