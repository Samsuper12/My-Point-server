#include "jsonbase.h"


JsonBase::JsonBase(QObject *parent)
{
    filepath = "C:/example/myfirst.json";
}

void JsonBase::append(float latitude, float longitude, QString username, QString description, int danger)
{
    QJsonObject obj;
    obj.insert("latitude", latitude);
    obj.insert("longitude", longitude);
    obj.insert("username", username);
    obj.insert("description", description);
    obj.insert("danger", danger);

    m_jarr.append(std::move(obj));
}

QJsonDocument JsonBase::getDoc()
{
    return std::move(m_jdoc);
}

void JsonBase::completeDoc()
{
    m_jdoc.setArray(std::move(m_jarr));
    while(m_jarr.count()){
        m_jarr.pop_back();
    }
    qDebug() << m_jarr.count() << "from JsonBase metod: completeDoc";

}

void JsonBase::saveToFile()
{
    QFile file(filepath);
    if(!file.open(QFile::WriteOnly)){
       qDebug() << "caanot write to file";
    }

     file.write(m_jdoc.toJson());
}
