#pragma once

#include <QObject>
#include <QVector>
#include <QDateTime>
#include "navtypes.h"  // Include completo para Problem, Session, User

/**
 * Controlador de sesiones de quiz/examen
 */
class SessionController : public QObject {
    Q_OBJECT

public:
    explicit SessionController(QObject *parent = nullptr);

    /**
     * Inicia una nueva sesión de examen
     * @param user usuario que realiza el examen
     */
    void startSession(User *user);

    /**
     * @return el problema actual
     */
    const Problem* currentProblem() const;

    /**
     * @return índice del problema actual (0-based)
     */
    int currentProblemIndex() const { return m_currentIndex; }

    /**
     * @return número total de problemas
     */
    int totalProblems() const { return m_problems.size(); }

    /**
     * Envía una respuesta para el problema actual
     * @param answerIndex índice de la respuesta (0-3)
     * @return true si la respuesta es correcta
     */
    bool submitAnswer(int answerIndex);

    /**
     * Avanza al siguiente problema
     * @return true si hay más problemas, false si era el último
     */
    bool nextProblem();

    /**
     * Termina la sesión y guarda los resultados
     */
    void endSession();

    /**
     * @return número de aciertos actuales
     */
    int hits() const { return m_hits; }

    /**
     * @return número de fallos actuales
     */
    int faults() const { return m_faults; }

    /**
     * @return true si hay una sesión activa
     */
    bool isSessionActive() const { return m_sessionActive; }

    /**
     * Obtiene el historial de sesiones de un usuario entre fechas
     */
    QVector<Session> getSessionHistory(const User *user,
                                        const QDate &from = QDate(),
                                        const QDate &to = QDate()) const;

signals:
    void sessionStarted();
    void problemChanged(int index);  // Simplificado para evitar tipo incompleto
    void answerSubmitted(bool correct);
    void sessionEnded(int hits, int faults);

private:
    User *m_currentUser = nullptr;
    QVector<Problem> m_problems;
    int m_currentIndex = 0;
    int m_hits = 0;
    int m_faults = 0;
    bool m_sessionActive = false;
    QDateTime m_sessionStart;

    void shuffleProblems();
};
