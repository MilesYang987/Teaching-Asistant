#include "databasehelper.h"

#include <QtSql/QSqlError>
#include <QThread>

QSqlDatabase DatabaseHelper::getDatabase()
{
    QString connectionName = QString("db_conn_1%").arg((quintptr)QThread::currentThreadId());

    if (QSqlDatabase::contains(connectionName)){
        return QSqlDatabase::database(connectionName);
    }
    else{
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL",connectionName);
        db.setDatabaseName(database);
        db.setHostName(ip);
        db.setPort(port.toInt());
        db.setUserName(user);
        db.setPassword(password);

    }

}

void DatabaseHelper::Setup(const QString& _database,const QString &_user, const QString &_password, const QString &_ip, const QString &_port)
{
    database = _database;
    user = _user;
    password = _password;
    ip = _ip;
    port = _port;
}

DatabaseHelper::DatabaseHelper() : ip("localhost"),port("3306"),user(""),password(""),database("")
{

}


