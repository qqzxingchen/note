#include <QVector>
#include <QMenu>
#include <QIcon>
#include "fontselector.h"


FontSelector::FontSelector(QObject *parent) :
    QObject(parent)
{
    fontSelectedIcon = new QIcon("icon\\fontselected.ico");

    actionVector.clear();
    fontFamilyVector.clear();
}

QMenu * FontSelector::initFontSelectMenu()
{
    fontMenu = new QMenu( tr("字体设置") );

    fontFamilyVector.append( tr("方正舒体") );
    fontFamilyVector.append( tr("微软雅黑") );
    fontFamilyVector.append( tr("华文隶书") );
    fontFamilyVector.append( tr("华文新魏") );
    fontFamilyVector.append( tr("华文中宋") );
    fontFamilyVector.append( tr("Verdana") );

    QAction * FZST = new QAction( fontFamilyVector.at(0),this );
    QAction * WRYH = new QAction( fontFamilyVector.at(1),this );
    QAction * HWLS = new QAction( fontFamilyVector.at(2),this );
    QAction * HWXW = new QAction( fontFamilyVector.at(3),this );
    QAction * HWZS = new QAction( fontFamilyVector.at(4),this );
    QAction * Verdana = new QAction( fontFamilyVector.at(5),this );

    HWLS->setIcon( *fontSelectedIcon );
    currentFont = HWLS;

    actionVector.append( FZST );
    actionVector.append( WRYH );
    actionVector.append( HWLS );
    actionVector.append( HWXW );
    actionVector.append( HWZS );
    actionVector.append( Verdana );

    for ( int i = 0 ; i < actionVector.count() ; i ++ )
        fontMenu->addAction ( actionVector.at(i) );

    connect(fontMenu,SIGNAL(triggered(QAction*)),
            this,SLOT(fontActionSlot(QAction*)));

    return fontMenu;
}

void FontSelector::fontActionSlot(QAction * action)
{
    if( currentFont != 0 )
        currentFont->setIcon( QIcon() );
    action->setIcon( *fontSelectedIcon );
    currentFont = action;

    for ( int i = 0 ; i < actionVector.count() ; i ++ )
        if( actionVector.at(i) == action )
            emit ( selectedFontFamily( fontFamilyVector.at(i) ) );
}

QString FontSelector::getCurrentFontFamily()
{
    for ( int i = 0 ; i < actionVector.count() ; i ++ )
        if( actionVector.at(i) == currentFont )
            return fontFamilyVector.at(i);
    return QString("华文隶书");
}
void FontSelector::setCurrentFontAction(QString fontFamily)
{
    for ( int i = 0 ; i < fontFamilyVector.count() ; i ++ )
        if( fontFamilyVector.at(i) == fontFamily )
            fontMenu->triggered( actionVector.at(i) );
}
