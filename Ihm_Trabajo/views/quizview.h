#pragma once

#include <QWidget>
#include <QButtonGroup>
#include "controllers/sessioncontroller.h"

namespace Ui {
class QuizView;
}

class QuizView : public QWidget
{
    Q_OBJECT

public:
    explicit QuizView(QWidget *parent = nullptr);
    ~QuizView();

    void setController(SessionController *controller);
    void startQuiz(User *user); // Resets to initial view and sets user context

signals:
    void backRequested();

private slots:
    // Menu
    void onRandomProblemClicked();
    void onSelectProblemClicked();
    
    // List
    void onProblemSelected(int index);
    
    // Problem
    void onVerifyClicked();
    
    // Creation
    void onAddQuestionClicked();
    void onSaveQuestionClicked();
    void onCancelCreateClicked();

    // Navigation
    void onBackToChartClicked();
    
    // Controller Signals
    void onProblemChanged(int index);
    void onAnswerSubmitted(bool correct);

private:
    Ui::QuizView *ui;
    SessionController *m_controller = nullptr;
    QButtonGroup *m_answerGroup = nullptr;
    QButtonGroup *m_correctAnswerGroup = nullptr;
    User *m_user = nullptr;
    
    void updateProblemView();
    void showFeedback(bool correct);
    void clearFeedback();
};
