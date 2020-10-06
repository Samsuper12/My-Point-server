#include "database.h"



DataBase::DataBase(QObject *parent)
{

}

void DataBase::connectToDataBase()
{
    if(!QFile("C:/example/" DATABASE_NAME).exists()){
        this->restoreDataBase();
    } else {
        this->openDataBase();
    }
}

bool DataBase::restoreDataBase()
{

    if(this->openDataBase()){
        return (this->createMarkerTable() && this->createUserTable()) ? true : false;
    } else {
        qDebug() << "Failed to restore the database";
        return false;
    }

}

bool DataBase::openDataBase()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setHostName(DATABASE_HOSTNAME);
    m_db.setDatabaseName("C:/example/" DATABASE_NAME);
    if(m_db.open()){
        return true;
    } else {
        return false;
    }
}

void DataBase::closeDataBase()
{
    m_db.close();
}

bool DataBase::createMarkerTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " TABLE " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            TABLE_LATITUDE      " FLOAT(255)    NOT NULL,"
                            TABLE_LONGITUDE     " FLOAT(255)    NOT NULL,"
                            TABLE_USERLOGIN     " VARCHAR(255)    NOT NULL,"
                            TABLE_DESCRIPTION   " VARCHAR(255)    NOT NULL,"
                            TABLE_DANGER        " INT(255)    NOT NULL"
                        " )"
                    )){
        qDebug() << "DataBase: error of create " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
}

bool DataBase::createUserTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " TABLE_USER " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            TABLE_LOGIN      " VARCHAR(255)    NOT NULL,"
                            TABLE_PASSWORD   " VARCHAR(255)    NOT NULL"
                        " )"
                    )){
        qDebug() << "DataBase: error of create " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
}

bool DataBase::insertIntoMarkerTable(const float lati, const float longi, const QString& usrlog, const QString& description, const int danger)
{

        QSqlQuery query(TABLE);
        query.prepare("INSERT INTO " TABLE " ( " TABLE_LATITUDE ","
                                                TABLE_LONGITUDE ","
                                                TABLE_USERLOGIN ","
                                                TABLE_DESCRIPTION ","
                                                TABLE_DANGER " ) "
                    "VALUES (:Latitude, :Longitude, :UserLogin, :Description, :Danger)");

        query.bindValue(":Latitude",            lati);
        query.bindValue(":Longitude",           longi);
        query.bindValue(":UserLogin",           usrlog);
        query.bindValue(":Description",         description);
        query.bindValue(":Danger",              danger);

        if(!query.exec()){
            qDebug() << "error insert into " << TABLE;
            qDebug() << query.lastError().text();
            return false;
        } else {
            return true;
        }



}

bool DataBase::insertIntoUserTable(const QString &login, const QString &password)
{
    QSqlQuery query(TABLE_USER);
    query.prepare("INSERT INTO " TABLE_USER " ( " TABLE_LOGIN ","
                                                  TABLE_PASSWORD " ) "
                "VALUES (:Login, :Password)");

    query.bindValue(":Login",               login);
    query.bindValue(":Password",            password);

    if(!query.exec()){
        qDebug() << "error insert into " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }

}

bool DataBase::findUserInTable(const QString& login, const QString& password)
{
    //SELECT * FROM course WHERE dept_name='Comp. Sci.' AND credits>3; // example

    QSqlQuery query(TABLE_USER);
    query.exec("SELECT * FROM " TABLE_USER " WHERE " TABLE_LOGIN "='" + login + "' AND " TABLE_PASSWORD"='" +password+"'" );
    while (query.next()) {

        qDebug() << query.value(0);
        qDebug() << query.value(1);
        qDebug() << query.value(2);

        qDebug() << "____________________";
        if((query.value(1).toString() == login) && (query.value(2).toString() == password)){
            return true;
        }
    }

    return false;

}

void DataBase::testfunk()
{
    QSqlQuery query(TABLE); // constructor have parameter to change database

    //query.exec("SELECT " TABLE_LONGITUDE "," TABLE_DANGER" FROM " TABLE " WHERE " TABLE_DANGER " > 1");

    query.exec("SELECT * FROM " TABLE );
    while (query.next()) {

        qDebug() << query.value(0);
        qDebug() << query.value(1);
        qDebug() << query.value(2);
        qDebug() << query.value(3);
        qDebug() << query.value(4);
        qDebug() << query.value(5);


        qDebug() << "____________________";
    }

    QSqlQuery query2(TABLE_USER);
    query2.exec("SELECT * FROM " TABLE_USER );
    while (query2.next()) {

        qDebug() << query2.value(0);
        qDebug() << query2.value(1);
        qDebug() << query2.value(2);

        qDebug() << "____________________";
    }
}


QJsonDocument DataBase::getAllBadeJson()
{
    QSqlQuery query;
    query.exec("SELECT * FROM " TABLE );
    while (query.next()) {
    m_jb.append(query.value(1).toFloat(), query.value(2).toFloat(), query.value(3).toString(),
                query.value(4).toString(),  query.value(5).toInt());
    }
    m_jb.completeDoc();

    return m_jb.getDoc();
}

//bool DataBase::inserIntoTable(const float lati, const float longi, const QString& usrlog, const QString& description, const int danger) //rework
//{

//    DataStruct data;
//    data.latitude = lati;
//    data.longitude = longi;
//    data.usrnamewhocreate = usrlog;
//    data.description = description;
//    data.danger = danger;

//    if(insertIntoMarkerTable(1, data))
//        return true;
//    else
//        return false;
//}

bool DataBase::removeRecordMarkerTable(const int id)
{


    QSqlQuery query(TABLE);
    query.prepare("DELETE FROM " TABLE " WHERE id= :ID ;");
    query.bindValue(":ID", id);

    if(!query.exec()){
        qDebug() << "error delete row " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }

}

bool DataBase::removeRecordUserTable(const int id)
{
    QSqlQuery query(TABLE_USER);
    query.prepare("DELETE FROM " TABLE_USER " WHERE id= :ID ;");
    query.bindValue(":ID", id);

    if(!query.exec()){
        qDebug() << "error delete row " << TABLE_USER;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
}
