#include "noteinfo.h"
#include "eventfilter.h"

#include <QPlainTextEdit>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QScrollBar>
#include <QApplication>

#include <QDebug>

QImage * NoteInfo::quitIcon = new QImage("icon\\quit.ico");
QImage * NoteInfo::minIcon = new QImage("icon\\min.ico");
QImage * NoteInfo::lockIcon = new QImage("icon\\lock.ico");
QImage * NoteInfo::freelockIcon = new QImage("icon\\freelock.ico");
EventFilter * NoteInfo::ef = new EventFilter();

NoteInfo::NoteInfo(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(Qt::Drawer |                 //消除任务栏图标
                   Qt::CustomizeWindowHint |
                   Qt::WindowStaysOnBottomHint );

    setGeometry(800,600,200,150);

    isLeftButtonDown = false;
    isDeleting = false;
    isLocked = false;

    fileName = QString::null;
    waitToBeDeleted = false;

    plainTextEdit = new QPlainTextEdit(this);
    plainTextEdit->installEventFilter(ef);

    plainTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    plainTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    transparentSlider = new QSlider(Qt::Horizontal,this);
    transparentSlider->setToolTip(QString("透明度"));
    transparentSlider->setMinimum(10);
    transparentSlider->setMaximum(100);
    connect ( transparentSlider,SIGNAL(valueChanged(int)),
              this, SLOT(setWindowTransparent(int)) );
    transparentSlider->setValue(80);

    font = new QFont("华文隶书");

    fontSizeSlider = new QSlider(Qt::Horizontal,this);
    fontSizeSlider->setToolTip(QString("字体大小"));
    fontSizeSlider->setMinimum(5);
    fontSizeSlider->setMaximum(40);
    connect ( fontSizeSlider,SIGNAL(valueChanged(int)),
              this, SLOT(setFontSize(int)) );
    fontSizeSlider->setValue(15);


    setAutoFillBackground (true);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(255,255,0));
    setPalette (palette);

    setMinimumSize(100,100);
    setMaximumSize(600,600);
}

void NoteInfo::lockToSetTop(bool lock)
{
    if ( lock )
    {
        isLocked = true;
        this->setWindowFlags(Qt::Drawer |
                             Qt::CustomizeWindowHint |
                             Qt::WindowStaysOnTopHint);
    }
    else
    {
        isLocked = false;
        this->setWindowFlags(Qt::Drawer |
                             Qt::CustomizeWindowHint |
                             Qt::WindowStaysOnBottomHint);
    }
    this->show();
    update();
}


void NoteInfo::setFontFamily(QString fontFamily)
{
    font->setFamily((fontFamily==QString::null)?"华文隶书":fontFamily);
    plainTextEdit->setFont(*font);
}
QString NoteInfo::getFontFamily()
{
    return font->family();
}


void NoteInfo::setFontSize(int fontSize)
{
    font->setPointSize( fontSize );
    plainTextEdit->setFont(*font);
    fontSizeSlider->setValue(fontSize);
}
int NoteInfo::getFontSize()
{
    return plainTextEdit->fontInfo().pointSize();
}


void NoteInfo::setWindowTransparent(int p)
{
    setWindowOpacity( ((double)p)/100 );
    transparentSlider->setValue( p );
}
int NoteInfo::getWindowTransparent()
{
    return transparentSlider->value();
}


void NoteInfo::setNoteText(QString text)
{
    plainTextEdit->setPlainText(text);
}
QString NoteInfo::getNoteText()
{
    return plainTextEdit->document()->toPlainText();
}


bool NoteInfo::getIsLocked() const
{
    return isLocked;
}
void NoteInfo::setIsLocked(bool value)
{
    isLocked = value;
}


QString NoteInfo::getFileName()
{
    return fileName;
}
void NoteInfo::setFileName(const QString &value)
{
    fileName = value;
}


bool NoteInfo::getWaitToBeDeleted() const
{
    return waitToBeDeleted;
}
void NoteInfo::setWaitToBeDeleted(bool value)
{
    waitToBeDeleted = value;
}


void NoteInfo::mousePressEvent(QMouseEvent * event)
{
    if( event->button () == Qt::LeftButton )
    {
        if( event->pos().y() <= 16 &&
                event->pos().x() >= currentSize.width() - 16 &&
                event->pos().x() <= currentSize.width() )
        {
            if (isDeleting == false)
            {
                isDeleting = true;
                emit ( deletePointer(this) );
            }
        }
        else if( event->pos().y() <= 16 &&
                 event->pos().x() >= currentSize.width() - 36 &&
                 event->pos().x() <  currentSize.width() - 20 )
        {
            this->hide();
        }
        else if( event->pos().y() <= 16 &&
                 event->pos().x() >= 0 &&
                 event->pos().x() <= 16 )
        {
            lockToSetTop( !isLocked );
        }
        else
        {
            isLeftButtonDown = true;
            oldPos = event->globalPos() - frameGeometry().topLeft();
        }
    }
}
void NoteInfo::mouseReleaseEvent(QMouseEvent * event)
{
    if( event->button () == Qt::LeftButton )
    {
        isLeftButtonDown = false;
    }
}
void NoteInfo::mouseMoveEvent(QMouseEvent * event)
{
    if( isLeftButtonDown )
    {
        this->move( event->globalPos()-oldPos );
    }
}

void NoteInfo::keyPressEvent (QKeyEvent *event)
{
    int k = event->key ();
    if ( (k & Qt::Key_Control)  && (k & Qt::Key_S) )
    {
        emit ( saveDataSignal () );
    }
}



void NoteInfo::paintEvent(QPaintEvent *)
{
    QPainter * pt = new QPainter(this);
    pt->drawImage( QRect(currentSize.width()-16,0,16,16),*quitIcon );
    pt->drawImage( QRect(currentSize.width()-36,-2,16,16),*minIcon );
    pt->drawImage( QRect(0,0,16,16),isLocked?(*lockIcon):(*freelockIcon));
    delete pt;
}

void NoteInfo::resizeEvent( QResizeEvent * event )
{
    currentSize = event->size();
    plainTextEdit->setGeometry( 2,18,
                         currentSize.width()-4,
                         currentSize.height()-40 );
    transparentSlider->setGeometry( 3,
                         currentSize.height()-18,
                         currentSize.width()/2-6,
                         16);
    fontSizeSlider->setGeometry(
                         currentSize.width()/2+3,
                         currentSize.height()-18,
                         currentSize.width()/2-6,
                         16);
    update();
}
