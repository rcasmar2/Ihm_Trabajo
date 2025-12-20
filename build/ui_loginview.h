/********************************************************************************
** Form generated from reading UI file 'loginview.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINVIEW_H
#define UI_LOGINVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginView
{
public:
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QSpacerItem *topSpacer;
    QLineEdit *nickEdit;
    QLineEdit *passwordEdit;
    QLabel *errorLabel;
    QPushButton *loginButton;
    QSpacerItem *middleSpacer;
    QPushButton *registerButton;
    QSpacerItem *bottomSpacer;

    void setupUi(QWidget *LoginView)
    {
        if (LoginView->objectName().isEmpty())
            LoginView->setObjectName("LoginView");
        LoginView->resize(400, 500);
        mainLayout = new QVBoxLayout(LoginView);
        mainLayout->setSpacing(20);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(60, 80, 60, 80);
        titleLabel = new QLabel(LoginView);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setAlignment(Qt::AlignCenter);

        mainLayout->addWidget(titleLabel);

        subtitleLabel = new QLabel(LoginView);
        subtitleLabel->setObjectName("subtitleLabel");
        subtitleLabel->setAlignment(Qt::AlignCenter);

        mainLayout->addWidget(subtitleLabel);

        topSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        mainLayout->addItem(topSpacer);

        nickEdit = new QLineEdit(LoginView);
        nickEdit->setObjectName("nickEdit");
        nickEdit->setMinimumHeight(40);

        mainLayout->addWidget(nickEdit);

        passwordEdit = new QLineEdit(LoginView);
        passwordEdit->setObjectName("passwordEdit");
        passwordEdit->setEchoMode(QLineEdit::Password);
        passwordEdit->setMinimumHeight(40);

        mainLayout->addWidget(passwordEdit);

        errorLabel = new QLabel(LoginView);
        errorLabel->setObjectName("errorLabel");
        errorLabel->setAlignment(Qt::AlignCenter);

        mainLayout->addWidget(errorLabel);

        loginButton = new QPushButton(LoginView);
        loginButton->setObjectName("loginButton");
        loginButton->setMinimumHeight(45);

        mainLayout->addWidget(loginButton);

        middleSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        mainLayout->addItem(middleSpacer);

        registerButton = new QPushButton(LoginView);
        registerButton->setObjectName("registerButton");
        registerButton->setFlat(true);

        mainLayout->addWidget(registerButton);

        bottomSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        mainLayout->addItem(bottomSpacer);


        retranslateUi(LoginView);

        QMetaObject::connectSlotsByName(LoginView);
    } // setupUi

    void retranslateUi(QWidget *LoginView)
    {
        titleLabel->setText(QCoreApplication::translate("LoginView", "Patr\303\263n de Embarcaci\303\263n", nullptr));
        titleLabel->setStyleSheet(QCoreApplication::translate("LoginView", "font-size: 24px; font-weight: bold; color: #2C3E50;", nullptr));
        subtitleLabel->setText(QCoreApplication::translate("LoginView", "Iniciar Sesi\303\263n", nullptr));
        subtitleLabel->setStyleSheet(QCoreApplication::translate("LoginView", "font-size: 16px; color: #7F8C8D;", nullptr));
        nickEdit->setPlaceholderText(QCoreApplication::translate("LoginView", "Nick de usuario", nullptr));
        passwordEdit->setPlaceholderText(QCoreApplication::translate("LoginView", "Contrase\303\261a", nullptr));
        errorLabel->setText(QString());
        errorLabel->setStyleSheet(QCoreApplication::translate("LoginView", "color: #E74C3C; font-size: 12px;", nullptr));
        loginButton->setText(QCoreApplication::translate("LoginView", "Entrar", nullptr));
        loginButton->setStyleSheet(QCoreApplication::translate("LoginView", "background-color: #3498DB; color: white; font-size: 14px; font-weight: bold; border-radius: 5px;", nullptr));
        registerButton->setText(QCoreApplication::translate("LoginView", "\302\277No tienes cuenta? Reg\303\255strate", nullptr));
        registerButton->setStyleSheet(QCoreApplication::translate("LoginView", "color: #3498DB; font-size: 12px;", nullptr));
        (void)LoginView;
    } // retranslateUi

};

namespace Ui {
    class LoginView: public Ui_LoginView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINVIEW_H
