#ifndef NOTEDATA_H
#define NOTEDATA_H

#include <QObject>
#include <QRect>

class NoteData : public QObject
{
    Q_OBJECT
public:
    explicit NoteData(QObject *parent = 0);

    bool readDataFromFile( QString filename );
    bool writeDataToFile( QString filename );

    void clearData();

    QRect getStation() const;
    void setStation(const QRect &value);

    int getPercent() const;
    void setPercent(int value);

    QString getText() const;
    void setText(const QString &value);

    int getFontSize() const;
    void setFontSize(int value);
private:
    QRect station;
    int percent;            // 透明度，0为不透明，100为全透明
    QString text;
    int fontSize;


};

#endif // NOTEDATA_H
