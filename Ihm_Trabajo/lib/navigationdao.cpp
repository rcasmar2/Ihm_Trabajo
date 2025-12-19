#include "navigationdao.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

NavigationDAO::NavigationDAO(const QString &dbFilePath)
    : m_dbFilePath(dbFilePath)
    , m_connectionName(QString("NavConnection_%1").arg(quintptr(this)))
{
    open();
    createTablesIfNeeded();
}

NavigationDAO::~NavigationDAO() {
    close();
}

void NavigationDAO::open() {
    m_db = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
    m_db.setDatabaseName(m_dbFilePath);
    
    if (!m_db.open()) {
        throwSqlError("open", m_db.lastError());
    }
}

void NavigationDAO::close() {
    if (m_db.isOpen()) {
        m_db.close();
    }
    QSqlDatabase::removeDatabase(m_connectionName);
}

void NavigationDAO::createTablesIfNeeded() {
    createUserTable();
    createSessionTable();
    createProblemTable();
}

void NavigationDAO::createUserTable() {
    QSqlQuery q(m_db);
    q.exec(R"(
        CREATE TABLE IF NOT EXISTS users (
            nick TEXT PRIMARY KEY,
            email TEXT NOT NULL,
            password TEXT NOT NULL,
            avatar BLOB,
            birthdate TEXT NOT NULL
        )
    )");
}

void NavigationDAO::createSessionTable() {
    QSqlQuery q(m_db);
    q.exec(R"(
        CREATE TABLE IF NOT EXISTS sessions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_nick TEXT NOT NULL,
            timestamp TEXT NOT NULL,
            hits INTEGER NOT NULL,
            faults INTEGER NOT NULL,
            FOREIGN KEY (user_nick) REFERENCES users(nick)
        )
    )");
}

void NavigationDAO::createProblemTable() {
    QSqlQuery q(m_db);
    q.exec(R"(
        CREATE TABLE IF NOT EXISTS problems (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            text TEXT NOT NULL,
            answers TEXT NOT NULL
        )
    )");
}

QMap<QString, User> NavigationDAO::loadUsers() {
    QMap<QString, User> users;
    
    QSqlQuery q(m_db);
    q.exec("SELECT nick, email, password, avatar, birthdate FROM users");
    
    while (q.next()) {
        User user = buildUserFromQuery(q);
        users.insert(user.nickName(), user);
    }
    
    return users;
}

QVector<Problem> NavigationDAO::loadProblems() {
    QVector<Problem> problems;
    
    QSqlQuery q(m_db);
    q.exec("SELECT id, text, answers FROM problems");
    
    while (q.next()) {
        Problem problem = buildProblemFromQuery(q);
        problems.append(problem);
    }
    
    return problems;
}

void NavigationDAO::saveUser(User &user) {
    QSqlQuery q(m_db);
    q.prepare(R"(
        INSERT INTO users (nick, email, password, avatar, birthdate)
        VALUES (:nick, :email, :password, :avatar, :birthdate)
    )");
    
    q.bindValue(":nick", user.nickName());
    q.bindValue(":email", user.email());
    q.bindValue(":password", user.password());
    q.bindValue(":avatar", imageToPng(user.avatar()));
    q.bindValue(":birthdate", dateToDb(user.birthdate()));
    
    if (!q.exec()) {
        throwSqlError("saveUser", q.lastError());
    }
}

void NavigationDAO::updateUser(const User &user) {
    QSqlQuery q(m_db);
    q.prepare(R"(
        UPDATE users SET email = :email, password = :password, 
                        avatar = :avatar, birthdate = :birthdate
        WHERE nick = :nick
    )");
    
    q.bindValue(":nick", user.nickName());
    q.bindValue(":email", user.email());
    q.bindValue(":password", user.password());
    q.bindValue(":avatar", imageToPng(user.avatar()));
    q.bindValue(":birthdate", dateToDb(user.birthdate()));
    
    if (!q.exec()) {
        throwSqlError("updateUser", q.lastError());
    }
}

void NavigationDAO::deleteUser(const QString &nickName) {
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM users WHERE nick = :nick");
    q.bindValue(":nick", nickName);
    
    if (!q.exec()) {
        throwSqlError("deleteUser", q.lastError());
    }
}

QVector<Session> NavigationDAO::loadSessionsFor(const QString &nickName) {
    QVector<Session> sessions;
    
    QSqlQuery q(m_db);
    q.prepare("SELECT timestamp, hits, faults FROM sessions WHERE user_nick = :nick ORDER BY timestamp DESC");
    q.bindValue(":nick", nickName);
    
    if (q.exec()) {
        while (q.next()) {
            Session session = buildSessionFromQuery(q);
            sessions.append(session);
        }
    }
    
    return sessions;
}

void NavigationDAO::addSession(const QString &nickName, const Session &session) {
    QSqlQuery q(m_db);
    q.prepare(R"(
        INSERT INTO sessions (user_nick, timestamp, hits, faults)
        VALUES (:nick, :timestamp, :hits, :faults)
    )");
    
    q.bindValue(":nick", nickName);
    q.bindValue(":timestamp", dateTimeToDb(session.timeStamp()));
    q.bindValue(":hits", session.hits());
    q.bindValue(":faults", session.faults());
    
    if (!q.exec()) {
        throwSqlError("addSession", q.lastError());
    }
}

void NavigationDAO::replaceAllProblems(const QVector<Problem> &problems) {
    QSqlQuery q(m_db);
    q.exec("DELETE FROM problems");
    
    for (const Problem &p : problems) {
        q.prepare(R"(
            INSERT INTO problems (text, answers) VALUES (:text, :answers)
        )");
        
        // Serializar respuestas como JSON
        QJsonArray answersArray;
        for (const Answer &a : p.answers()) {
            QJsonObject ansObj;
            ansObj["text"] = a.text();
            ansObj["valid"] = a.validity();
            answersArray.append(ansObj);
        }
        
        q.bindValue(":text", p.text());
        q.bindValue(":answers", QJsonDocument(answersArray).toJson(QJsonDocument::Compact));
        
        if (!q.exec()) {
            qWarning() << "Error insertando problema:" << q.lastError().text();
        }
    }
}

void NavigationDAO::addProblem(const Problem &problem) {
    QSqlQuery q(m_db);
    q.prepare(R"(
        INSERT INTO problems (text, answers) VALUES (:text, :answers)
    )");
    
    QJsonArray answersArray;
    for (const Answer &a : problem.answers()) {
        QJsonObject ansObj;
        ansObj["text"] = a.text();
        ansObj["valid"] = a.validity();
        answersArray.append(ansObj);
    }
    
    q.bindValue(":text", problem.text());
    q.bindValue(":answers", QJsonDocument(answersArray).toJson(QJsonDocument::Compact));
    
    if (!q.exec()) {
        throwSqlError("addProblem", q.lastError());
    }
}

void NavigationDAO::importProblemsFromJson(const QString &jsonFilePath) {
    QFile file(jsonFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open problems.json:" << jsonFilePath;
        return;
    }
    
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    
    if (!doc.isArray()) return;
    
    QVector<Problem> problems;
    QJsonArray arr = doc.array();
    
    for (const QJsonValue &val : arr) {
        QJsonObject obj = val.toObject();
        QString text = obj["text"].toString();
        QJsonArray ansArr = obj["answers"].toArray();
        QVector<Answer> answers;
        
        for (const QJsonValue &aVal : ansArr) {
            QJsonObject aObj = aVal.toObject();
            answers.append(Answer(aObj["text"].toString(), aObj["valid"].toBool()));
        }
        
        problems.append(Problem(text, answers));
    }
    
    if (!problems.isEmpty()) {
       replaceAllProblems(problems);
    }
}

// === BUILDERS ===

User NavigationDAO::buildUserFromQuery(QSqlQuery &q) {
    QString nick = q.value(0).toString();
    QString email = q.value(1).toString();
    QString password = q.value(2).toString();
    QImage avatar = imageFromPng(q.value(3).toByteArray());
    QDate birthdate = dateFromDb(q.value(4).toString());
    
    User user(nick, email, password, avatar, birthdate);
    user.setInsertedInDb(true);
    return user;
}

Session NavigationDAO::buildSessionFromQuery(QSqlQuery &q) {
    QDateTime timestamp = dateTimeFromDb(q.value(0).toString());
    int hits = q.value(1).toInt();
    int faults = q.value(2).toInt();
    
    return Session(timestamp, hits, faults);
}

Problem NavigationDAO::buildProblemFromQuery(QSqlQuery &q) {
    QString text = q.value(1).toString();
    QString answersJson = q.value(2).toString();
    
    QVector<Answer> answers;
    QJsonDocument doc = QJsonDocument::fromJson(answersJson.toUtf8());
    QJsonArray arr = doc.array();
    
    for (const QJsonValue &val : arr) {
        QJsonObject obj = val.toObject();
        answers.append(Answer(obj["text"].toString(), obj["valid"].toBool()));
    }
    
    return Problem(text, answers);
}

// === CONVERSIONES ===

QByteArray NavigationDAO::imageToPng(const QImage &img) {
    if (img.isNull()) return QByteArray();
    
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer, "PNG");
    return ba;
}

QImage NavigationDAO::imageFromPng(const QByteArray &bytes) {
    if (bytes.isEmpty()) return QImage();
    return QImage::fromData(bytes, "PNG");
}

QString NavigationDAO::dateToDb(const QDate &date) const {
    return date.toString(Qt::ISODate);
}

QDate NavigationDAO::dateFromDb(const QString &s) const {
    return QDate::fromString(s, Qt::ISODate);
}

QString NavigationDAO::dateTimeToDb(const QDateTime &dt) const {
    return dt.toString(Qt::ISODate);
}

QDateTime NavigationDAO::dateTimeFromDb(const QString &s) const {
    return QDateTime::fromString(s, Qt::ISODate);
}

QString NavigationDAO::boolToDb(bool v) const {
    return v ? "1" : "0";
}

bool NavigationDAO::boolFromDb(const QString &s) const {
    return s == "1";
}

void NavigationDAO::throwSqlError(const QString &where, const QSqlError &err) const {
    throw NavDAOException(QString("%1: %2").arg(where, err.text()));
}
