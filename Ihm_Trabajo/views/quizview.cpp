#include "quizview.h"
#include "ui_quizview.h"
#include "lib/include/navigation.h" 
#include <QMessageBox>
#include <QDebug>

QuizView::QuizView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuizView)
{
    ui->setupUi(this);
    
    // Configure as popup dialog
    setWindowTitle("Problemas de Navegación");
    setModal(true);
    setFixedSize(700, 550);
    
    // Apply dark theme styling
    setStyleSheet(R"(
        QDialog {
            background-color: #1a1a2e;
            border: 2px solid #333;
            border-radius: 16px;
        }
    )");

    // Group radio buttons
    m_answerGroup = new QButtonGroup(this);
    // Important: IDs must match 0-3 index
    m_answerGroup->addButton(ui->rbAnswer1, 0);
    m_answerGroup->addButton(ui->rbAnswer2, 1);
    m_answerGroup->addButton(ui->rbAnswer3, 2);
    m_answerGroup->addButton(ui->rbAnswer4, 3);

    // Connect to update styling when selection changes
    connect(m_answerGroup, &QButtonGroup::idClicked, this, &QuizView::updateAnswerStyles);

    // Connections
    connect(ui->btnRandom, &QPushButton::clicked, this, &QuizView::onRandomProblemClicked);
    connect(ui->btnSelect, &QPushButton::clicked, this, &QuizView::onSelectProblemClicked);
    connect(ui->problemList, &QListWidget::itemClicked, this, [this](QListWidgetItem *item) {
        if (item) {
            onProblemSelected(item->data(Qt::UserRole).toInt());
        }
    });
    connect(ui->btnBackFromList, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->btnVerify, &QPushButton::clicked, this, &QuizView::onVerifyClicked);
    connect(ui->btnBackFromProblem, &QPushButton::clicked, this, [this]() {
        if (m_controller) m_controller->endSession();
        ui->stackedWidget->setCurrentIndex(0);
    });

    // START EDIT: New connections
    connect(ui->btnBackToChart, &QPushButton::clicked, this, &QuizView::onBackToChartClicked);
    connect(ui->btnAddQuestion, &QPushButton::clicked, this, &QuizView::onAddQuestionClicked);
    connect(ui->btnSaveQuestion, &QPushButton::clicked, this, &QuizView::onSaveQuestionClicked);
    connect(ui->btnCancelCreate, &QPushButton::clicked, this, &QuizView::onCancelCreateClicked);
    // END EDIT

    // Init Create Group
    m_correctAnswerGroup = new QButtonGroup(this);
    m_correctAnswerGroup->addButton(ui->rbCorrect1, 0);
    m_correctAnswerGroup->addButton(ui->rbCorrect2, 1);
    m_correctAnswerGroup->addButton(ui->rbCorrect3, 2);
    m_correctAnswerGroup->addButton(ui->rbCorrect4, 3);
}

QuizView::~QuizView() {
    delete ui;
}

void QuizView::setController(SessionController *controller) {
    m_controller = controller;
    if (m_controller) {
        connect(m_controller, &SessionController::problemChanged, this, &QuizView::onProblemChanged);
        connect(m_controller, &SessionController::answerSubmitted, this, &QuizView::onAnswerSubmitted);
    }
}

void QuizView::startQuiz(User *user) {
    m_user = user;
    ui->stackedWidget->setCurrentIndex(0); // Menu
    clearFeedback();
}

void QuizView::onRandomProblemClicked() {
    if (!m_controller || !m_user) return;
    m_controller->startRandomProblem(m_user);
    // UI switching handled in onProblemChanged
}

void QuizView::onSelectProblemClicked() {
    // Populate list
    ui->problemList->clear();
    const auto &problems = Navigation::instance().problems();
    for (int i = 0; i < problems.size(); ++i) {
        QListWidgetItem *item = new QListWidgetItem(problems[i].text());
        item->setData(Qt::UserRole, i);
        ui->problemList->addItem(item);
    }
    ui->stackedWidget->setCurrentIndex(1);
}

void QuizView::onProblemSelected(int index) {
    if (!m_controller || !m_user) return;
    m_controller->startSpecificProblem(m_user, index);
}

void QuizView::onVerifyClicked() {
    if (!m_controller) return;
    int id = m_answerGroup->checkedId();
    if (id != -1) {
        m_controller->submitAnswer(id);
    } else {
        QMessageBox::warning(this, "Aviso", "Selecciona una respuesta primero.");
    }
}

void QuizView::onProblemChanged(int index) {
    Q_UNUSED(index)
    updateProblemView();
    ui->stackedWidget->setCurrentIndex(2);
}

void QuizView::onAnswerSubmitted(bool correct) {
    showFeedback(correct);
}

void QuizView::updateProblemView() {
    if (!m_controller) return;
    const Problem *p = m_controller->currentProblem();
    if (!p) return;

    ui->labelQuestion->setText(p->text());

    // Get shuffled answers
    QVector<Answer> answers = m_controller->currentShuffledAnswers();
    
    // Ensure we have 4 answers or handle dynamic count
    // The requirement says "las 4 respuestas".
    QList<QRadioButton*> rbs = {ui->rbAnswer1, ui->rbAnswer2, ui->rbAnswer3, ui->rbAnswer4};
    
    for (int i = 0; i < rbs.size(); ++i) {
        if (i < answers.size()) {
            rbs[i]->setText(answers[i].text());
            rbs[i]->setVisible(true);
            rbs[i]->setChecked(false);
            rbs[i]->setEnabled(true);
            rbs[i]->setStyleSheet("padding: 10px; font-size: 14px; color: white;"); 
        } else {
            rbs[i]->setVisible(false);
        }
    }
    
    clearFeedback();
    ui->btnVerify->setEnabled(true);
}

void QuizView::showFeedback(bool correct) {
    if (correct) {
        ui->labelFeedback->setText("¡Correcto!");
        ui->labelFeedback->setStyleSheet("font-size: 18px; font-weight: bold; color: #27ae60; margin: 15px;");
    } else {
        ui->labelFeedback->setText("Incorrecto. Inténtalo de nuevo.");
        ui->labelFeedback->setStyleSheet("font-size: 18px; font-weight: bold; color: #e74c3c; margin: 15px;");
    }
    // Keep btnVerify enabled so user can try again
}

void QuizView::updateAnswerStyles() {
    QList<QRadioButton*> rbs = {ui->rbAnswer1, ui->rbAnswer2, ui->rbAnswer3, ui->rbAnswer4};
    
    QString selectedStyle = R"(
        QRadioButton {
            padding: 12px 16px;
            font-size: 14px;
            color: white;
            background-color: rgba(233, 69, 96, 0.25);
            border: 2px solid #e94560;
            border-radius: 8px;
        }
        QRadioButton::indicator {
            width: 18px;
            height: 18px;
        }
    )";
    
    QString defaultStyle = R"(
        QRadioButton {
            padding: 12px 16px;
            font-size: 14px;
            color: white;
            background-color: #2a2a2a;
            border: 2px solid transparent;
            border-radius: 8px;
        }
        QRadioButton:hover {
            background-color: #3a3a3a;
            border-color: #555;
        }
        QRadioButton::indicator {
            width: 18px;
            height: 18px;
        }
    )";
    
    for (QRadioButton *rb : rbs) {
        if (rb->isChecked()) {
            rb->setStyleSheet(selectedStyle);
        } else {
            rb->setStyleSheet(defaultStyle);
        }
    }
}

void QuizView::clearFeedback() {
    ui->labelFeedback->clear();
}

void QuizView::onBackToChartClicked() {
    if (m_controller) m_controller->endSession();
    accept(); // Close dialog
}

void QuizView::onAddQuestionClicked() {
    // Clear inputs
    ui->editQuestionText->clear();
    ui->editAnswer1->clear();
    ui->editAnswer2->clear();
    ui->editAnswer3->clear();
    ui->editAnswer4->clear();
    
    // Clear selection
    // QButtonGroup doesn't have clear(), have to uncheck button
    if (m_correctAnswerGroup->checkedButton()) {
        m_correctAnswerGroup->setExclusive(false);
        m_correctAnswerGroup->checkedButton()->setChecked(false);
        m_correctAnswerGroup->setExclusive(true);
    }
    
    ui->stackedWidget->setCurrentIndex(3); // pageCreate
}

void QuizView::onSaveQuestionClicked() {
    QString text = ui->editQuestionText->text().trimmed();
    QString a1 = ui->editAnswer1->text().trimmed();
    QString a2 = ui->editAnswer2->text().trimmed();
    QString a3 = ui->editAnswer3->text().trimmed();
    QString a4 = ui->editAnswer4->text().trimmed();
    
    if (text.isEmpty() || a1.isEmpty() || a2.isEmpty() || a3.isEmpty() || a4.isEmpty()) {
        QMessageBox::warning(this, "Aviso", "Todos los campos son obligatorios.");
        return;
    }
    
    int correctId = m_correctAnswerGroup->checkedId();
    if (correctId == -1) {
        QMessageBox::warning(this, "Aviso", "Debes seleccionar cuál es la respuesta correcta.");
        return;
    }
    
    QVector<Answer> answers;
    answers.append(Answer(a1, correctId == 0));
    answers.append(Answer(a2, correctId == 1));
    answers.append(Answer(a3, correctId == 2));
    answers.append(Answer(a4, correctId == 3));
    
    if (m_controller) {
        m_controller->addProblem(text, answers);
        QMessageBox::information(this, "Éxito", "Pregunta creada correctamente.");
        onCancelCreateClicked(); // Go back to list and refresh?
        // Refresh list
        onSelectProblemClicked(); 
    }
}

void QuizView::onCancelCreateClicked() {
    ui->stackedWidget->setCurrentIndex(1); // Back to List
}
