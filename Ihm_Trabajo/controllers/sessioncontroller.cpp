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
    
    m_sessionActive = true; 
    // Added in previous turn but missed connection here if startSession was kept as is, 
    // but looking at file content, startSession is at line 11.
    // Wait, I am replacing lines 22-27.
    
    // Reset counters
    m_currentIndex = 0;
    
    // We need to ensure we have problems before preparing answers
    if (!m_problems.isEmpty()) {
       prepareProblemAnswers();
       emit sessionStarted();
       emit problemChanged(0);
    } else {
       emit sessionStarted();
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

    const auto &answers = m_currentShuffledAnswers;
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
        prepareProblemAnswers();
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

void SessionController::startRandomProblem(User *user) {
    if (!user) return;
    m_currentUser = user;
    m_problems = Navigation::instance().problems(); // Refresh list
    
    if (m_problems.isEmpty()) return;

    // Select one random problem
    int randomIndex = QRandomGenerator::global()->bounded(m_problems.size());
    // We can swap it to 0 and pretend we have a list of 1? 
    // Or just store the index. 
    // To minimize code change, let's just swap it to 0 and set list size to 1 if we wanted "session of 1",
    // but the requirement says "user asks for A problem".
    // simpler: keep all problems but jump to the random one? 
    // No, better to having a clean state.
    
    Problem p = m_problems[randomIndex];
    m_problems.clear();
    m_problems.append(p); 
    
    m_currentIndex = 0;
    m_hits = 0;
    m_faults = 0;
    m_sessionActive = true;
    m_sessionStart = QDateTime::currentDateTime();

    prepareProblemAnswers();
    emit sessionStarted();
    emit problemChanged(0);
}

void SessionController::startSpecificProblem(User *user, int index) {
    if (!user) return;
    auto allProblems = Navigation::instance().problems();
    if (index < 0 || index >= allProblems.size()) return;

    m_currentUser = user;
    m_problems.clear();
    m_problems.append(allProblems[index]);

    m_currentIndex = 0;
    m_hits = 0;
    m_faults = 0;
    m_sessionActive = true;
    m_sessionStart = QDateTime::currentDateTime();

    prepareProblemAnswers();
    emit sessionStarted();
    emit problemChanged(0);
}

void SessionController::addProblem(const QString &text, const QVector<Answer> &answers) {
    Problem p(text, answers);
    Navigation::instance().addProblem(p);
}

void SessionController::prepareProblemAnswers() {
    const Problem *p = currentProblem();
    if (!p) {
        m_currentShuffledAnswers.clear();
        return;
    }

    m_currentShuffledAnswers = p->answers();
    
    // Shuffle answers
    for (int i = m_currentShuffledAnswers.size() - 1; i > 0; --i) {
        int j = QRandomGenerator::global()->bounded(i + 1);
        m_currentShuffledAnswers.swapItemsAt(i, j);
    }
}
