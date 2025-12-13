#include "sessioncontroller.h"
#include "navigation.h"
#include <QRandomGenerator>
#include <algorithm>

SessionController::SessionController(QObject *parent)
    : QObject(parent)
{
}

void SessionController::startSession(User *user) {
    if (!user) return;

    m_currentUser = user;
    m_problems = Navigation::instance().problems();
    m_currentIndex = 0;
    m_hits = 0;
    m_faults = 0;
    m_sessionActive = true;
    m_sessionStart = QDateTime::currentDateTime();

    shuffleProblems();

    emit sessionStarted();
    if (!m_problems.isEmpty()) {
        emit problemChanged(0);
    }
}

const Problem* SessionController::currentProblem() const {
    if (m_currentIndex >= 0 && m_currentIndex < m_problems.size()) {
        return &m_problems[m_currentIndex];
    }
    return nullptr;
}

bool SessionController::submitAnswer(int answerIndex) {
    const Problem *problem = currentProblem();
    if (!problem || !m_sessionActive) return false;

    const auto &answers = problem->answers();
    if (answerIndex < 0 || answerIndex >= answers.size()) return false;

    bool correct = answers[answerIndex].validity();

    if (correct) {
        m_hits++;
    } else {
        m_faults++;
    }

    emit answerSubmitted(correct);
    return correct;
}

bool SessionController::nextProblem() {
    if (!m_sessionActive) return false;

    m_currentIndex++;
    if (m_currentIndex < m_problems.size()) {
        emit problemChanged(m_currentIndex);
        return true;
    }

    // No hay más problemas
    return false;
}

void SessionController::endSession() {
    if (!m_sessionActive) return;

    m_sessionActive = false;

    // Guardar sesión en el usuario
    if (m_currentUser) {
        Session session(m_sessionStart, m_hits, m_faults);
        Navigation::instance().addSession(m_currentUser->nickName(), session);
    }

    emit sessionEnded(m_hits, m_faults);
}

QVector<Session> SessionController::getSessionHistory(const User *user,
                                                       const QDate &from,
                                                       const QDate &to) const {
    if (!user) return {};

    const auto &sessions = user->sessions();

    if (!from.isValid() && !to.isValid()) {
        return sessions;
    }

    QVector<Session> filtered;
    for (const Session &s : sessions) {
        QDate sessionDate = s.timeStamp().date();
        bool afterFrom = !from.isValid() || sessionDate >= from;
        bool beforeTo = !to.isValid() || sessionDate <= to;

        if (afterFrom && beforeTo) {
            filtered.append(s);
        }
    }

    return filtered;
}

void SessionController::shuffleProblems() {
    // Fisher-Yates shuffle
    for (int i = m_problems.size() - 1; i > 0; --i) {
        int j = QRandomGenerator::global()->bounded(i + 1);
        m_problems.swapItemsAt(i, j);
    }
}
