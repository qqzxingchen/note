#include "notedata.h"

#include <QFile>
#include <QTextStream>
#include <QRect>

NoteData::NoteData(QObject *parent) :
    QObject(parent)
{
    clearData();
}


void NoteData::clearData()
{
    station = QRect(0,0,0,0);
    percent = 100;
    text = QString::null;
    fontSize = 0;
}


bool NoteData::writeDataToFile(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);
    out<< this->station.x() << "\n"
       << this->station.y() << "\n"
       << this->station.width() << "\n"
       << this->station.height() << "\n"
       << this->percent << "\n"
       << this->fontSize << "\n"
       << this->text;


    file.close();
    return true;
}

bool NoteData::readDataFromFile(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);

    station.setX( in.readLine().toInt() );
    station.setY( in.readLine().toInt() );
    station.setWidth( in.readLine().toInt() );
    station.setHeight( in.readLine().toInt() );
    percent = in.readLine().toInt();
    fontSize = in.readLine().toInt();

    text = QString::null;
    while (!in.atEnd())
    {
        QString line = in.readLine();
        text += line;
        text += "\n";
    }
    file.close();
    return true;
}


QString NoteData::getText() const
{
    return text;
}
void NoteData::setText(const QString &value)
{
    text = value;
}


int NoteData::getFontSize() const
{
    return fontSize;
}
void NoteData::setFontSize(int value)
{
    fontSize = value;
}


int NoteData::getPercent() const
{
    return percent;
}
void NoteData::setPercent(int value)
{
    percent = value;
}


QRect NoteData::getStation() const
{
    return station;
}
void NoteData::setStation(const QRect &value)
{
    station = value;
}

