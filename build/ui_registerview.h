/********************************************************************************
** Form generated from reading UI file 'registerview.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERVIEW_H
#define UI_REGISTERVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegisterView
{
public:
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QSpacerItem *topSpacer;
    QHBoxLayout *avatarLayout;
    QSpacerItem *avatarLeftSpacer;
    QLabel *avatarLabel;
    QSpacerItem *avatarRightSpacer;
    QPushButton *selectAvatarButton;
    QLineEdit *nickEdit;
    QLabel *nickErrorLabel;
    QLineEdit *emailEdit;
    QLabel *emailErrorLabel;
    QLineEdit *passwordEdit;
    QLabel *passwordErrorLabel;
    QLineEdit *confirmPasswordEdit;
    QHBoxLayout *birthdateLayout;
    QLabel *birthdateLabel;
    QDateEdit *birthdateEdit;
    QLabel *birthdateErrorLabel;
    QSpacerItem *middleSpacer;
    QPushButton *registerButton;
    QPushButton *backButton;

    void setupUi(QWidget *RegisterView)
    {
        if (RegisterView->objectName().isEmpty())
            RegisterView->setObjectName("RegisterView");
        RegisterView->resize(450, 600);
        mainLayout = new QVBoxLayout(RegisterView);
        mainLayout->setSpacing(15);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(50, 40, 50, 40);
        titleLabel = new QLabel(RegisterView);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setAlignment(Qt::AlignCenter);

        mainLayout->addWidget(titleLabel);

        topSpacer = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        mainLayout->addItem(topSpacer);

        avatarLayout = new QHBoxLayout();
        avatarLayout->setObjectName("avatarLayout");
        avatarLeftSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        avatarLayout->addItem(avatarLeftSpacer);

        avatarLabel = new QLabel(RegisterView);
        avatarLabel->setObjectName("avatarLabel");
        avatarLabel->setMinimumSize(QSize(80, 80));
        avatarLabel->setMaximumSize(QSize(80, 80));
        avatarLabel->setAlignment(Qt::AlignCenter);

        avatarLayout->addWidget(avatarLabel);

        avatarRightSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        avatarLayout->addItem(avatarRightSpacer);


        mainLayout->addLayout(avatarLayout);

        selectAvatarButton = new QPushButton(RegisterView);
        selectAvatarButton->setObjectName("selectAvatarButton");
        selectAvatarButton->setFlat(true);

        mainLayout->addWidget(selectAvatarButton);

        nickEdit = new QLineEdit(RegisterView);
        nickEdit->setObjectName("nickEdit");
        nickEdit->setMinimumHeight(38);

        mainLayout->addWidget(nickEdit);

        nickErrorLabel = new QLabel(RegisterView);
        nickErrorLabel->setObjectName("nickErrorLabel");

        mainLayout->addWidget(nickErrorLabel);

        emailEdit = new QLineEdit(RegisterView);
        emailEdit->setObjectName("emailEdit");
        emailEdit->setMinimumHeight(38);

        mainLayout->addWidget(emailEdit);

        emailErrorLabel = new QLabel(RegisterView);
        emailErrorLabel->setObjectName("emailErrorLabel");

        mainLayout->addWidget(emailErrorLabel);

        passwordEdit = new QLineEdit(RegisterView);
        passwordEdit->setObjectName("passwordEdit");
        passwordEdit->setEchoMode(QLineEdit::Password);
        passwordEdit->setMinimumHeight(38);

        mainLayout->addWidget(passwordEdit);

        passwordErrorLabel = new QLabel(RegisterView);
        passwordErrorLabel->setObjectName("passwordErrorLabel");

        mainLayout->addWidget(passwordErrorLabel);

        confirmPasswordEdit = new QLineEdit(RegisterView);
        confirmPasswordEdit->setObjectName("confirmPasswordEdit");
        confirmPasswordEdit->setEchoMode(QLineEdit::Password);
        confirmPasswordEdit->setMinimumHeight(38);

        mainLayout->addWidget(confirmPasswordEdit);

        birthdateLayout = new QHBoxLayout();
        birthdateLayout->setObjectName("birthdateLayout");
        birthdateLabel = new QLabel(RegisterView);
        birthdateLabel->setObjectName("birthdateLabel");

        birthdateLayout->addWidget(birthdateLabel);

        birthdateEdit = new QDateEdit(RegisterView);
        birthdateEdit->setObjectName("birthdateEdit");
        birthdateEdit->setMinimumHeight(38);
        birthdateEdit->setCalendarPopup(true);

        birthdateLayout->addWidget(birthdateEdit);


        mainLayout->addLayout(birthdateLayout);

        birthdateErrorLabel = new QLabel(RegisterView);
        birthdateErrorLabel->setObjectName("birthdateErrorLabel");

        mainLayout->addWidget(birthdateErrorLabel);

        middleSpacer = new QSpacerItem(20, 15, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        mainLayout->addItem(middleSpacer);

        registerButton = new QPushButton(RegisterView);
        registerButton->setObjectName("registerButton");
        registerButton->setMinimumHeight(45);

        mainLayout->addWidget(registerButton);

        backButton = new QPushButton(RegisterView);
        backButton->setObjectName("backButton");
        backButton->setFlat(true);

        mainLayout->addWidget(backButton);


        retranslateUi(RegisterView);

        QMetaObject::connectSlotsByName(RegisterView);
    } // setupUi

    void retranslateUi(QWidget *RegisterView)
    {
        titleLabel->setText(QCoreApplication::translate("RegisterView", "Crear Cuenta", nullptr));
        titleLabel->setStyleSheet(QCoreApplication::translate("RegisterView", "font-size: 22px; font-weight: bold; color: #2C3E50;", nullptr));
        avatarLabel->setStyleSheet(QCoreApplication::translate("RegisterView", "background-color: #BDC3C7; border-radius: 40px;", nullptr));
        avatarLabel->setText(QCoreApplication::translate("RegisterView", "\360\237\223\267", nullptr));
        selectAvatarButton->setText(QCoreApplication::translate("RegisterView", "Seleccionar Avatar", nullptr));
        selectAvatarButton->setStyleSheet(QCoreApplication::translate("RegisterView", "color: #3498DB; font-size: 11px;", nullptr));
        nickEdit->setPlaceholderText(QCoreApplication::translate("RegisterView", "Nick (6-15 caracteres)", nullptr));
        nickErrorLabel->setStyleSheet(QCoreApplication::translate("RegisterView", "color: #E74C3C; font-size: 11px;", nullptr));
        emailEdit->setPlaceholderText(QCoreApplication::translate("RegisterView", "Email", nullptr));
        emailErrorLabel->setStyleSheet(QCoreApplication::translate("RegisterView", "color: #E74C3C; font-size: 11px;", nullptr));
        passwordEdit->setPlaceholderText(QCoreApplication::translate("RegisterView", "Contrase\303\261a (8-20 caracteres)", nullptr));
        passwordErrorLabel->setStyleSheet(QCoreApplication::translate("RegisterView", "color: #E74C3C; font-size: 11px;", nullptr));
        confirmPasswordEdit->setPlaceholderText(QCoreApplication::translate("RegisterView", "Confirmar Contrase\303\261a", nullptr));
        birthdateLabel->setText(QCoreApplication::translate("RegisterView", "Fecha de nacimiento:", nullptr));
        birthdateEdit->setDisplayFormat(QCoreApplication::translate("RegisterView", "dd/MM/yyyy", nullptr));
        birthdateErrorLabel->setStyleSheet(QCoreApplication::translate("RegisterView", "color: #E74C3C; font-size: 11px;", nullptr));
        registerButton->setText(QCoreApplication::translate("RegisterView", "Crear Cuenta", nullptr));
        registerButton->setStyleSheet(QCoreApplication::translate("RegisterView", "background-color: #27AE60; color: white; font-size: 14px; font-weight: bold; border-radius: 5px;", nullptr));
        backButton->setText(QCoreApplication::translate("RegisterView", "\342\206\220 Volver al Login", nullptr));
        backButton->setStyleSheet(QCoreApplication::translate("RegisterView", "color: #7F8C8D; font-size: 12px;", nullptr));
        (void)RegisterView;
    } // retranslateUi

};

namespace Ui {
    class RegisterView: public Ui_RegisterView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERVIEW_H
