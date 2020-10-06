#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>
#include <QVariantList>
#include <QVector>

#include "jsonbase.h"

#define DATABASE_HOSTNAME   "KharkovDataBase"
#define DATABASE_NAME       "Kharkov.db"

#define TABLE                "Kharkov"
#define TABLE_LATITUDE       "Latitude"
#define TABLE_LONGITUDE      "Longitude"
#define TABLE_USERLOGIN      "UserLogin"
#define TABLE_DESCRIPTION    "Description"
#define TABLE_DANGER         "Danger"


#define TABLE_USER           "People"
#define TABLE_LOGIN          "Login"
#define TABLE_PASSWORD       "Password"

//#define TABLE_USER 1
//#define TABLE_CITY 0

struct DataStruct
{
    float latitude;
    float longitude;
    QString usrnamewhocreate;
    QString description;
    int danger;
};


class DataBase : public QObject
{

    Q_OBJECT

public:
    explicit DataBase(QObject *parent = nullptr);
    void connectToDataBase();


    //bool inserIntoTable(const float lati, const float longi, const QString& usrlog,
     //                   const QString& description, const int danger);

    bool insertIntoMarkerTable(const float lati, const float longi, const QString& usrlog, const QString& description, const int danger); // а здесь убрать переключатель
    bool insertIntoUserTable(const QString& login, const QString& password); // завтра добавить определение

    bool findUserInTable(const QString& login, const QString& password);

    void testfunk();



    QJsonDocument getAllBadeJson();

private:
    QSqlDatabase    m_db;
    JsonBase        m_jb;

    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();

    bool createMarkerTable();
    bool createUserTable();



public slots:


    bool removeRecordMarkerTable(const int id); // 1 - markertable 2 - usertable
    bool removeRecordUserTable(const int id);


};

#endif // DATABASE_H
