#ifndef JSONBASE_H
#define JSONBASE_H

#include <QObject>

#include <QFile>
#include <QTextStream>
#include <QByteArray>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>


class JsonBase : QObject
{

    Q_OBJECT

public:

    explicit JsonBase(QObject *parent = nullptr);

    void append(float latitude, float longitude, QString username,
                QString description, int danger);

    QJsonDocument getDoc();

    void completeDoc();
    void saveToFile();



private:
    QJsonArray m_jarr;
    QJsonDocument m_jdoc;

    QString filepath;


};

#endif // JSONBASE_H
