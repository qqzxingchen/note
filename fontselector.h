#ifndef FONTSELECTOR_H
#define FONTSELECTOR_H

#include <QObject>
#include <QMenu>

class FontSelector : public QObject
{
    Q_OBJECT
public:
    explicit FontSelector(QObject *parent = 0);

    QMenu * initFontSelectMenu();
    QString getCurrentFontFamily();
    void setCurrentFontAction( QString );
private:
    QVector<QAction *> actionVector;
    QVector<QString  > fontFamilyVector;

    QIcon * fontSelectedIcon;

    QMenu * fontMenu;
    QAction * currentFont;
signals:
    void selectedFontFamily(QString);

public slots:
    void fontActionSlot(QAction *);
};

#endif // FONTSELECTOR_H
