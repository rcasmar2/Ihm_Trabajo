#pragma once

#include <QString>
#include <QDate>
#include <QDateTime>
#include <QImage>
#include <QVector>

class Answer {
public:
    Answer() = default;
    Answer(const QString &text, bool validity)
        : m_text(text), m_validity(validity) {}

    const QString &text() const { return m_text; }
    void setText(const QString &text) { m_text = text; }

    bool validity() const { return m_validity; }
    void setValidity(bool v) { m_validity = v; }

private:
    QString m_text;
    bool    m_validity = false;
};

class Session {
public:
    Session() = default;
    Session(const QDateTime &ts, int hits, int faults)
        : m_timeStamp(ts), m_hits(hits), m_faults(faults) {}

    const QDateTime &timeStamp() const { return m_timeStamp; }
    int hits() const { return m_hits; }
    int faults() const { return m_faults; }

private:
    QDateTime m_timeStamp;
    int       m_hits   = 0;
    int       m_faults = 0;
};

class Problem {
public:
    Problem() = default;
    Problem(const QString &text, const QVector<Answer> &answers)
        : m_text(text), m_answers(answers) {}

    const QString &text() const { return m_text; }
    void setText(const QString &t) { m_text = t; }

    const QVector<Answer> &answers() const { return m_answers; }
    void setAnswers(const QVector<Answer> &ans) { m_answers = ans; }

private:
    QString         m_text;
    QVector<Answer> m_answers;
};

class User {
public:
    User() = default;

    User(const QString &nickName,
         const QString &email,
         const QString &password,
         const QImage  &avatar,
         const QDate   &birthdate)
        : m_nickName(nickName),
          m_email(email),
          m_password(password),
          m_avatar(avatar),
          m_birthdate(birthdate) {}

    const QString &nickName() const { return m_nickName; }
    const QString &email() const { return m_email; }
    const QString &password() const { return m_password; }
    const QImage  &avatar() const { return m_avatar; }
    const QDate   &birthdate() const { return m_birthdate; }

    void setEmail(const QString &e) { m_email = e; }
    void setPassword(const QString &p) { m_password = p; }
    void setAvatar(const QImage &img) { m_avatar = img; }
    void setBirthdate(const QDate &d) { m_birthdate = d; }

    const QVector<Session> &sessions() const { return m_sessions; }
    void setSessions(const QVector<Session> &s) { m_sessions = s; }

    void addSession(const Session &s) { m_sessions.push_back(s); }
    void addSession(int hits, int faults, const QDateTime &ts) {
        m_sessions.emplace_back(ts, hits, faults);
    }

    bool insertedInDb() const { return m_insertedDb; }
    void setInsertedInDb(bool v) { m_insertedDb = v; }

private:
    QString          m_nickName;
    QString          m_email;
    QString          m_password;
    QImage           m_avatar;
    QDate            m_birthdate;
    QVector<Session> m_sessions;

    bool             m_insertedDb = false;
};
