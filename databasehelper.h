#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <Qstring>

class DatabaseHelper
{
public:
    static DatabaseHelper& GetInstance(){
        static DatabaseHelper ins;
        return ins;
    }

    QSqlDatabase getDatabase();

    bool TryConnection();
    void Setup(const QString& database, const QString& user, const QString& password, const QString& ip="localhost", const QString& port="3306");

    std::string ErrorMsg();

private:
    DatabaseHelper();
    ~DatabaseHelper() = default;
    DatabaseHelper(const DatabaseHelper&) = delete;
    DatabaseHelper& operator=(const DatabaseHelper&) = delete;

private:
    QString database;
    QString ip;
    QString port;
    QString user;
    QString password;

};

#endif // DATABASEHELPER_H
