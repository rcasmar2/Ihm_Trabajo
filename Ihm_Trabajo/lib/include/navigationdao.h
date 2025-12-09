#pragma once

#include "navtypes.h"
#include "navdaoexception.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QByteArray>
#include <QBuffer>
#include <QMap>

class NavigationDAO
{
public:
    explicit NavigationDAO(const QString &dbFilePath);
    ~NavigationDAO();

    QMap<QString, User> loadUsers();
    QVector<Problem>    loadProblems();

    void saveUser(User &user);
    void updateUser(const User &user);
    void deleteUser(const QString &nickName);

    QVector<Session> loadSessionsFor(const QString &nickName);
    void addSession(const QString &nickName, const Session &session);

    void replaceAllProblems(const QVector<Problem> &problems);

private:
    QString      m_dbFilePath;
    QString      m_connectionName;
    QSqlDatabase m_db;

    void open();
    void close();
    void createTablesIfNeeded();

    void createUserTable();
    void createSessionTable();
    void createProblemTable();

    User    buildUserFromQuery(QSqlQuery &q);
    Session buildSessionFromQuery(QSqlQuery &q);
    Problem buildProblemFromQuery(QSqlQuery &q);

    QByteArray imageToPng(const QImage &img);
    QImage     imageFromPng(const QByteArray &bytes);

    QString    dateToDb(const QDate &date) const;
    QDate      dateFromDb(const QString &s) const;

    QString    dateTimeToDb(const QDateTime &dt) const;
    QDateTime  dateTimeFromDb(const QString &s) const;

    QString    boolToDb(bool v) const;
    bool       boolFromDb(const QString &s) const;

    [[noreturn]] void throwSqlError(const QString &where, const QSqlError &err) const;
};
