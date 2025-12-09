#pragma once

#include "navtypes.h"
#include "navigationdao.h"

#include <QMap>
#include <QVector>
#include <QString>

class Navigation
{
public:
    static Navigation &instance();

    const QMap<QString, User> &users() const { return m_users; }
    const QVector<Problem> &problems() const { return m_problems; }

    User *findUser(const QString &nick);
    const User *findUser(const QString &nick) const;

    User *authenticate(const QString &nick, const QString &password);

    void addUser(User &user);
    void updateUser(const User &user);
    void removeUser(const QString &nickName);

    void addSession(const QString &nickName, const Session &session);

    void reload();

    NavigationDAO &dao() { return m_dao; }
    const NavigationDAO &dao() const { return m_dao; }

private:
    Navigation();
    ~Navigation() = default;

    Navigation(const Navigation &) = delete;
    Navigation &operator=(const Navigation &) = delete;

    void loadFromDb();

    NavigationDAO       m_dao;
    QMap<QString, User> m_users;
    QVector<Problem>    m_problems;
};
