/********************************************************************************
** Form generated from reading UI file 'quizview.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QUIZVIEW_H
#define UI_QUIZVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QuizView
{
public:
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *pageMenu;
    QVBoxLayout *layoutMenu;
    QLabel *lblTitle;
    QHBoxLayout *layoutButtons;
    QPushButton *btnRandom;
    QPushButton *btnSelect;
    QPushButton *btnBackToChart;
    QSpacerItem *spacer;
    QWidget *pageList;
    QVBoxLayout *layoutList;
    QLabel *lblListTitle;
    QHBoxLayout *layoutListActions;
    QPushButton *btnAddQuestion;
    QSpacerItem *spacerListAction;
    QListWidget *problemList;
    QPushButton *btnBackFromList;
    QWidget *pageProblem;
    QVBoxLayout *layoutProblem;
    QLabel *labelQuestion;
    QRadioButton *rbAnswer1;
    QRadioButton *rbAnswer2;
    QRadioButton *rbAnswer3;
    QRadioButton *rbAnswer4;
    QLabel *labelFeedback;
    QHBoxLayout *layoutActions;
    QPushButton *btnVerify;
    QPushButton *btnBackFromProblem;
    QSpacerItem *spacerProb;
    QWidget *pageCreate;
    QVBoxLayout *layoutCreate;
    QLabel *lblCreateTitle;
    QLineEdit *editQuestionText;
    QLabel *lblAnswers;
    QHBoxLayout *layoutAns1;
    QRadioButton *rbCorrect1;
    QLineEdit *editAnswer1;
    QHBoxLayout *layoutAns2;
    QRadioButton *rbCorrect2;
    QLineEdit *editAnswer2;
    QHBoxLayout *layoutAns3;
    QRadioButton *rbCorrect3;
    QLineEdit *editAnswer3;
    QHBoxLayout *layoutAns4;
    QRadioButton *rbCorrect4;
    QLineEdit *editAnswer4;
    QHBoxLayout *layoutCreateActions;
    QPushButton *btnSaveQuestion;
    QPushButton *btnCancelCreate;
    QSpacerItem *spacerCreate;

    void setupUi(QWidget *QuizView)
    {
        if (QuizView->objectName().isEmpty())
            QuizView->setObjectName("QuizView");
        QuizView->resize(800, 600);
        verticalLayout = new QVBoxLayout(QuizView);
        verticalLayout->setObjectName("verticalLayout");
        stackedWidget = new QStackedWidget(QuizView);
        stackedWidget->setObjectName("stackedWidget");
        pageMenu = new QWidget();
        pageMenu->setObjectName("pageMenu");
        layoutMenu = new QVBoxLayout(pageMenu);
        layoutMenu->setObjectName("layoutMenu");
        lblTitle = new QLabel(pageMenu);
        lblTitle->setObjectName("lblTitle");
        lblTitle->setAlignment(Qt::AlignCenter);

        layoutMenu->addWidget(lblTitle);

        layoutButtons = new QHBoxLayout();
        layoutButtons->setObjectName("layoutButtons");
        btnRandom = new QPushButton(pageMenu);
        btnRandom->setObjectName("btnRandom");
        btnRandom->setMinimumHeight(50);

        layoutButtons->addWidget(btnRandom);

        btnSelect = new QPushButton(pageMenu);
        btnSelect->setObjectName("btnSelect");
        btnSelect->setMinimumHeight(50);

        layoutButtons->addWidget(btnSelect);


        layoutMenu->addLayout(layoutButtons);

        btnBackToChart = new QPushButton(pageMenu);
        btnBackToChart->setObjectName("btnBackToChart");

        layoutMenu->addWidget(btnBackToChart);

        spacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        layoutMenu->addItem(spacer);

        stackedWidget->addWidget(pageMenu);
        pageList = new QWidget();
        pageList->setObjectName("pageList");
        layoutList = new QVBoxLayout(pageList);
        layoutList->setObjectName("layoutList");
        lblListTitle = new QLabel(pageList);
        lblListTitle->setObjectName("lblListTitle");

        layoutList->addWidget(lblListTitle);

        layoutListActions = new QHBoxLayout();
        layoutListActions->setObjectName("layoutListActions");
        btnAddQuestion = new QPushButton(pageList);
        btnAddQuestion->setObjectName("btnAddQuestion");

        layoutListActions->addWidget(btnAddQuestion);

        spacerListAction = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        layoutListActions->addItem(spacerListAction);


        layoutList->addLayout(layoutListActions);

        problemList = new QListWidget(pageList);
        problemList->setObjectName("problemList");

        layoutList->addWidget(problemList);

        btnBackFromList = new QPushButton(pageList);
        btnBackFromList->setObjectName("btnBackFromList");

        layoutList->addWidget(btnBackFromList);

        stackedWidget->addWidget(pageList);
        pageProblem = new QWidget();
        pageProblem->setObjectName("pageProblem");
        layoutProblem = new QVBoxLayout(pageProblem);
        layoutProblem->setObjectName("layoutProblem");
        labelQuestion = new QLabel(pageProblem);
        labelQuestion->setObjectName("labelQuestion");
        labelQuestion->setWordWrap(true);

        layoutProblem->addWidget(labelQuestion);

        rbAnswer1 = new QRadioButton(pageProblem);
        rbAnswer1->setObjectName("rbAnswer1");

        layoutProblem->addWidget(rbAnswer1);

        rbAnswer2 = new QRadioButton(pageProblem);
        rbAnswer2->setObjectName("rbAnswer2");

        layoutProblem->addWidget(rbAnswer2);

        rbAnswer3 = new QRadioButton(pageProblem);
        rbAnswer3->setObjectName("rbAnswer3");

        layoutProblem->addWidget(rbAnswer3);

        rbAnswer4 = new QRadioButton(pageProblem);
        rbAnswer4->setObjectName("rbAnswer4");

        layoutProblem->addWidget(rbAnswer4);

        labelFeedback = new QLabel(pageProblem);
        labelFeedback->setObjectName("labelFeedback");
        labelFeedback->setAlignment(Qt::AlignCenter);

        layoutProblem->addWidget(labelFeedback);

        layoutActions = new QHBoxLayout();
        layoutActions->setObjectName("layoutActions");
        btnVerify = new QPushButton(pageProblem);
        btnVerify->setObjectName("btnVerify");
        btnVerify->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        layoutActions->addWidget(btnVerify);

        btnBackFromProblem = new QPushButton(pageProblem);
        btnBackFromProblem->setObjectName("btnBackFromProblem");

        layoutActions->addWidget(btnBackFromProblem);


        layoutProblem->addLayout(layoutActions);

        spacerProb = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        layoutProblem->addItem(spacerProb);

        stackedWidget->addWidget(pageProblem);
        pageCreate = new QWidget();
        pageCreate->setObjectName("pageCreate");
        layoutCreate = new QVBoxLayout(pageCreate);
        layoutCreate->setObjectName("layoutCreate");
        lblCreateTitle = new QLabel(pageCreate);
        lblCreateTitle->setObjectName("lblCreateTitle");

        layoutCreate->addWidget(lblCreateTitle);

        editQuestionText = new QLineEdit(pageCreate);
        editQuestionText->setObjectName("editQuestionText");

        layoutCreate->addWidget(editQuestionText);

        lblAnswers = new QLabel(pageCreate);
        lblAnswers->setObjectName("lblAnswers");

        layoutCreate->addWidget(lblAnswers);

        layoutAns1 = new QHBoxLayout();
        layoutAns1->setObjectName("layoutAns1");
        rbCorrect1 = new QRadioButton(pageCreate);
        rbCorrect1->setObjectName("rbCorrect1");

        layoutAns1->addWidget(rbCorrect1);

        editAnswer1 = new QLineEdit(pageCreate);
        editAnswer1->setObjectName("editAnswer1");

        layoutAns1->addWidget(editAnswer1);


        layoutCreate->addLayout(layoutAns1);

        layoutAns2 = new QHBoxLayout();
        layoutAns2->setObjectName("layoutAns2");
        rbCorrect2 = new QRadioButton(pageCreate);
        rbCorrect2->setObjectName("rbCorrect2");

        layoutAns2->addWidget(rbCorrect2);

        editAnswer2 = new QLineEdit(pageCreate);
        editAnswer2->setObjectName("editAnswer2");

        layoutAns2->addWidget(editAnswer2);


        layoutCreate->addLayout(layoutAns2);

        layoutAns3 = new QHBoxLayout();
        layoutAns3->setObjectName("layoutAns3");
        rbCorrect3 = new QRadioButton(pageCreate);
        rbCorrect3->setObjectName("rbCorrect3");

        layoutAns3->addWidget(rbCorrect3);

        editAnswer3 = new QLineEdit(pageCreate);
        editAnswer3->setObjectName("editAnswer3");

        layoutAns3->addWidget(editAnswer3);


        layoutCreate->addLayout(layoutAns3);

        layoutAns4 = new QHBoxLayout();
        layoutAns4->setObjectName("layoutAns4");
        rbCorrect4 = new QRadioButton(pageCreate);
        rbCorrect4->setObjectName("rbCorrect4");

        layoutAns4->addWidget(rbCorrect4);

        editAnswer4 = new QLineEdit(pageCreate);
        editAnswer4->setObjectName("editAnswer4");

        layoutAns4->addWidget(editAnswer4);


        layoutCreate->addLayout(layoutAns4);

        layoutCreateActions = new QHBoxLayout();
        layoutCreateActions->setObjectName("layoutCreateActions");
        btnSaveQuestion = new QPushButton(pageCreate);
        btnSaveQuestion->setObjectName("btnSaveQuestion");

        layoutCreateActions->addWidget(btnSaveQuestion);

        btnCancelCreate = new QPushButton(pageCreate);
        btnCancelCreate->setObjectName("btnCancelCreate");

        layoutCreateActions->addWidget(btnCancelCreate);


        layoutCreate->addLayout(layoutCreateActions);

        spacerCreate = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        layoutCreate->addItem(spacerCreate);

        stackedWidget->addWidget(pageCreate);

        verticalLayout->addWidget(stackedWidget);


        retranslateUi(QuizView);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(QuizView);
    } // setupUi

    void retranslateUi(QWidget *QuizView)
    {
        lblTitle->setText(QCoreApplication::translate("QuizView", "Men\303\272 de Problemas", nullptr));
        lblTitle->setStyleSheet(QCoreApplication::translate("QuizView", "font-size: 24px; font-weight: bold; color: white;", nullptr));
        btnRandom->setText(QCoreApplication::translate("QuizView", "Problema Aleatorio", nullptr));
        btnSelect->setText(QCoreApplication::translate("QuizView", "Seleccionar de Lista", nullptr));
        btnBackToChart->setText(QCoreApplication::translate("QuizView", "Volver a la Carta", nullptr));
        btnBackToChart->setStyleSheet(QCoreApplication::translate("QuizView", "margin-top: 20px;", nullptr));
        lblListTitle->setText(QCoreApplication::translate("QuizView", "Selecciona un problema", nullptr));
        lblListTitle->setStyleSheet(QCoreApplication::translate("QuizView", "font-size: 18px; color: white;", nullptr));
        btnAddQuestion->setText(QCoreApplication::translate("QuizView", "Crear Pregunta", nullptr));
        btnAddQuestion->setStyleSheet(QCoreApplication::translate("QuizView", "background-color: #3498db; color: white;", nullptr));
        btnBackFromList->setText(QCoreApplication::translate("QuizView", "Volver", nullptr));
        labelQuestion->setText(QCoreApplication::translate("QuizView", "Pregunta...", nullptr));
        labelQuestion->setStyleSheet(QCoreApplication::translate("QuizView", "font-size: 18px; font-weight: bold; color: white; margin-bottom: 20px;", nullptr));
        rbAnswer1->setText(QCoreApplication::translate("QuizView", "Respuesta 1", nullptr));
        rbAnswer1->setStyleSheet(QCoreApplication::translate("QuizView", "padding: 10px; font-size: 14px;", nullptr));
        rbAnswer2->setText(QCoreApplication::translate("QuizView", "Respuesta 2", nullptr));
        rbAnswer2->setStyleSheet(QCoreApplication::translate("QuizView", "padding: 10px; font-size: 14px;", nullptr));
        rbAnswer3->setText(QCoreApplication::translate("QuizView", "Respuesta 3", nullptr));
        rbAnswer3->setStyleSheet(QCoreApplication::translate("QuizView", "padding: 10px; font-size: 14px;", nullptr));
        rbAnswer4->setText(QCoreApplication::translate("QuizView", "Respuesta 4", nullptr));
        rbAnswer4->setStyleSheet(QCoreApplication::translate("QuizView", "padding: 10px; font-size: 14px;", nullptr));
        labelFeedback->setText(QString());
        labelFeedback->setStyleSheet(QCoreApplication::translate("QuizView", "font-size: 16px; font-weight: bold; margin: 15px;", nullptr));
        btnVerify->setText(QCoreApplication::translate("QuizView", "Verificar Soluci\303\263n", nullptr));
        btnVerify->setStyleSheet(QCoreApplication::translate("QuizView", "background-color: #27ae60; color: white; padding: 10px;", nullptr));
        btnBackFromProblem->setText(QCoreApplication::translate("QuizView", "Volver al Men\303\272", nullptr));
        btnBackFromProblem->setStyleSheet(QCoreApplication::translate("QuizView", "padding: 10px;", nullptr));
        lblCreateTitle->setText(QCoreApplication::translate("QuizView", "Crear Nueva Pregunta", nullptr));
        lblCreateTitle->setStyleSheet(QCoreApplication::translate("QuizView", "font-size: 18px; font-weight: bold; color: white; margin-bottom: 10px;", nullptr));
        editQuestionText->setPlaceholderText(QCoreApplication::translate("QuizView", "Enunciado de la pregunta...", nullptr));
        lblAnswers->setText(QCoreApplication::translate("QuizView", "Respuestas (Marca la correcta):", nullptr));
        rbCorrect1->setText(QString());
        editAnswer1->setPlaceholderText(QCoreApplication::translate("QuizView", "Opci\303\263n 1", nullptr));
        rbCorrect2->setText(QString());
        editAnswer2->setPlaceholderText(QCoreApplication::translate("QuizView", "Opci\303\263n 2", nullptr));
        rbCorrect3->setText(QString());
        editAnswer3->setPlaceholderText(QCoreApplication::translate("QuizView", "Opci\303\263n 3", nullptr));
        rbCorrect4->setText(QString());
        editAnswer4->setPlaceholderText(QCoreApplication::translate("QuizView", "Opci\303\263n 4", nullptr));
        btnSaveQuestion->setText(QCoreApplication::translate("QuizView", "Guardar Pregunta", nullptr));
        btnSaveQuestion->setStyleSheet(QCoreApplication::translate("QuizView", "background-color: #27ae60; color: white;", nullptr));
        btnCancelCreate->setText(QCoreApplication::translate("QuizView", "Cancelar", nullptr));
        (void)QuizView;
    } // retranslateUi

};

namespace Ui {
    class QuizView: public Ui_QuizView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QUIZVIEW_H
