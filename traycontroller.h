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


private:
    // 保存所有现存的noteInfo对象指针
    QVector<NoteInfo *> noteInfoArray;

    // 菜单
    QMenu * menu;
    FontSelector * fontMenu;
    QString fontFamily;

    QAction * createOneAction;
    QAction * hideAllAction;
    QAction * disAllAction;
    QAction * saveAction;
    QAction * resumeDeletedAction;
    QAction * quitAction;

    // 创建菜单
    void createContextMenu();

    // 随机生成一个data下不存在的文件名
    QString createAnUnExistingFileName();

    // 读取保存在文件的数据
    void readSavedData();

    // 根据文件中的信息创建一个新的noteInfo并设置初始值
    void createOneNote( QRect,int,QString,int,QString );
public slots:
    // 新建一个noteInfo的内存并加入到noteInfoArray中
    // isShow：创建完毕之后是否进行显示
    //          true：创建完毕之后立即显示
    //          false：创建完毕之后不显示
    NoteInfo * createOneSlot(bool isShow = true);

    // 隐藏所有标签（不包括置顶的标签）
    void hideAllSlot();

    // 显示所有标签（不包括等待被删除的标签）
    void disAllSlot();

    // 由于被点击关闭的标签的内存实际上并没有被清除，而只是设置了
    // 等待被删除，因此可以制作恢复数据的功能：
    // isSaveDeletedNote：程序中等待被删除的note对应的内存以及硬盘文件是否要保留
    //      false：保留，即文件不被删除，且数据也正常保存（Ctrl+s）
    //      true：不保留，即文件被删除，NoteInfoArray中的内存也将被清除（常在程序退出时）
    void writeToSaveData(bool isSaveDeletedNote=true);

    // 程序退出，将调用writeToSaveData（false）
    void quitSlot();

    // 恢复被删除的标签，即将等待被删除标志设为false
    void resumeDeletedSlot();

    // 接受来自noteInfo的消息，将等待被删除标志设为true
    void deletePointerSlot(NoteInfo*);

    // 重写了托盘图标的点击事件
    void activatedSlot(QSystemTrayIcon::ActivationReason);

    // 将所有存在的标签的字体都设置成被选中的字体
    void setNewFontFamily(QString);
};

#endif // TRAYCONTROLLER_H
