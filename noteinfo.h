#ifndef NOTEINFO_H
#define NOTEINFO_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QSlider>
#include "eventfilter.h"

class NoteInfo : public QWidget
{
    Q_OBJECT
public:
    explicit NoteInfo(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

    void keyPressEvent (QKeyEvent *);

private:
    static QImage * quitIcon;
    static QImage * minIcon;
    static QImage * lockIcon;
    static QImage * freelockIcon;

    static EventFilter * ef;

    QString fileName;
    bool waitToBeDeleted;

    QFont * font;
    QPlainTextEdit * plainTextEdit;
    QSlider * transparentSlider;
    QSlider * fontSizeSlider;

    QSize currentSize;
    QPoint oldPos;

    bool isLeftButtonDown;
    bool isDeleting;
    bool isLocked;

    void lockToSetTop(bool);
signals:
    void deletePointer(NoteInfo *);
    void saveDataSignal();
    void focusChange( );
public slots:
    void setWindowTransparent(int);
    void setFontSize(int);
public:
    int getWindowTransparent();
    int getFontSize();

    void setFontFamily(QString);
    QString getFontFamily();

    void setNoteText(QString text);
    QString getNoteText();

    bool getIsLocked() const;
    void setIsLocked(bool value);

    QString getFileName();
    void setFileName(const QString &value);

    bool getWaitToBeDeleted() const;
    void setWaitToBeDeleted(bool value);
};

#endif // NOTEINFO_H
