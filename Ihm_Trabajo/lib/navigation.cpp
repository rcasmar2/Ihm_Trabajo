#include "navigation.h"

Navigation::Navigation()
    : m_dao("navdb.sqlite")
{
    loadFromDb();
}

Navigation& Navigation::instance() {
    static Navigation nav;
    return nav;
}

User* Navigation::findUser(const QString &nick) {
    auto it = m_users.find(nick);
    if (it != m_users.end()) {
        return &it.value();
    }
    return nullptr;
}

const User* Navigation::findUser(const QString &nick) const {
    auto it = m_users.find(nick);
    if (it != m_users.end()) {
        return &it.value();
    }
    return nullptr;
}

User* Navigation::authenticate(const QString &nick, const QString &password) {
    User *user = findUser(nick);
    if (user && user->password() == password) {
        return user;
    }
    return nullptr;
}

void Navigation::addUser(User &user) {
    m_dao.saveUser(user);
    user.setInsertedInDb(true);
    m_users.insert(user.nickName(), user);
}

void Navigation::updateUser(const User &user) {
    m_dao.updateUser(user);
    m_users[user.nickName()] = user;
}

void Navigation::removeUser(const QString &nickName) {
    m_dao.deleteUser(nickName);
    m_users.remove(nickName);
}

void Navigation::addSession(const QString &nickName, const Session &session) {
    m_dao.addSession(nickName, session);
    
    User *user = findUser(nickName);
    if (user) {
        user->addSession(session);
    }
}

void Navigation::reload() {
    m_users.clear();
    m_problems.clear();
    loadFromDb();
}

void Navigation::loadFromDb() {
    m_users = m_dao.loadUsers();
    m_problems = m_dao.loadProblems();
    
    // Cargar sesiones para cada usuario
    for (auto it = m_users.begin(); it != m_users.end(); ++it) {
        QVector<Session> sessions = m_dao.loadSessionsFor(it.key());
        it.value().setSessions(sessions);
    }
}
