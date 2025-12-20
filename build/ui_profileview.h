/********************************************************************************
** Form generated from reading UI file 'profileview.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROFILEVIEW_H
#define UI_PROFILEVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProfileView
{
public:
    QVBoxLayout *mainLayout;
    QFrame *headerFrame;
    QHBoxLayout *headerLayout;
    QLabel *titleLabel;
    QSpacerItem *headerSpacer;
    QPushButton *backToChartButton;
    QHBoxLayout *contentLayout;
    QSpacerItem *leftContentSpacer;
    QFrame *cardFrame;
    QVBoxLayout *cardLayout;
    QHBoxLayout *avatarLayout;
    QSpacerItem *avatarLeft;
    QVBoxLayout *avatarInnerLayout;
    QLabel *avatarLabel;
    QPushButton *changeAvatarButton;
    QSpacerItem *avatarRight;
    QLabel *nickLabel;
    QLabel *lblEmail;
    QLineEdit *emailEdit;
    QLabel *lblBirth;
    QDateEdit *birthdateEdit;
    QLabel *lblNewPass;
    QLineEdit *passwordEdit;
    QSpacerItem *formSpacer;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    QSpacerItem *rightContentSpacer;
    QSpacerItem *bottomSpacer;

    void setupUi(QWidget *ProfileView)
    {
        if (ProfileView->objectName().isEmpty())
            ProfileView->setObjectName("ProfileView");
        ProfileView->resize(800, 600);
        mainLayout = new QVBoxLayout(ProfileView);
        mainLayout->setSpacing(0);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(0, 0, 0, 0);
        headerFrame = new QFrame(ProfileView);
        headerFrame->setObjectName("headerFrame");
        headerFrame->setMinimumSize(QSize(0, 60));
        headerLayout = new QHBoxLayout(headerFrame);
        headerLayout->setObjectName("headerLayout");
        headerLayout->setContentsMargins(20, -1, 20, -1);
        titleLabel = new QLabel(headerFrame);
        titleLabel->setObjectName("titleLabel");

        headerLayout->addWidget(titleLabel);

        headerSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        headerLayout->addItem(headerSpacer);

        backToChartButton = new QPushButton(headerFrame);
        backToChartButton->setObjectName("backToChartButton");
        backToChartButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        headerLayout->addWidget(backToChartButton);


        mainLayout->addWidget(headerFrame);

        contentLayout = new QHBoxLayout();
        contentLayout->setObjectName("contentLayout");
        leftContentSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        contentLayout->addItem(leftContentSpacer);

        cardFrame = new QFrame(ProfileView);
        cardFrame->setObjectName("cardFrame");
        cardFrame->setMinimumSize(QSize(450, 550));
        cardFrame->setMaximumSize(QSize(450, 600));
        cardLayout = new QVBoxLayout(cardFrame);
        cardLayout->setSpacing(15);
        cardLayout->setObjectName("cardLayout");
        cardLayout->setContentsMargins(40, 40, 40, 40);
        avatarLayout = new QHBoxLayout();
        avatarLayout->setObjectName("avatarLayout");
        avatarLeft = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        avatarLayout->addItem(avatarLeft);

        avatarInnerLayout = new QVBoxLayout();
        avatarInnerLayout->setObjectName("avatarInnerLayout");
        avatarLabel = new QLabel(cardFrame);
        avatarLabel->setObjectName("avatarLabel");
        avatarLabel->setMinimumSize(QSize(100, 100));
        avatarLabel->setMaximumSize(QSize(100, 100));
        avatarLabel->setAlignment(Qt::AlignCenter);

        avatarInnerLayout->addWidget(avatarLabel);

        changeAvatarButton = new QPushButton(cardFrame);
        changeAvatarButton->setObjectName("changeAvatarButton");
        changeAvatarButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        changeAvatarButton->setFlat(true);

        avatarInnerLayout->addWidget(changeAvatarButton);


        avatarLayout->addLayout(avatarInnerLayout);

        avatarRight = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        avatarLayout->addItem(avatarRight);


        cardLayout->addLayout(avatarLayout);

        nickLabel = new QLabel(cardFrame);
        nickLabel->setObjectName("nickLabel");
        nickLabel->setAlignment(Qt::AlignCenter);

        cardLayout->addWidget(nickLabel);

        lblEmail = new QLabel(cardFrame);
        lblEmail->setObjectName("lblEmail");

        cardLayout->addWidget(lblEmail);

        emailEdit = new QLineEdit(cardFrame);
        emailEdit->setObjectName("emailEdit");

        cardLayout->addWidget(emailEdit);

        lblBirth = new QLabel(cardFrame);
        lblBirth->setObjectName("lblBirth");

        cardLayout->addWidget(lblBirth);

        birthdateEdit = new QDateEdit(cardFrame);
        birthdateEdit->setObjectName("birthdateEdit");
        birthdateEdit->setCalendarPopup(true);

        cardLayout->addWidget(birthdateEdit);

        lblNewPass = new QLabel(cardFrame);
        lblNewPass->setObjectName("lblNewPass");

        cardLayout->addWidget(lblNewPass);

        passwordEdit = new QLineEdit(cardFrame);
        passwordEdit->setObjectName("passwordEdit");
        passwordEdit->setEchoMode(QLineEdit::Password);

        cardLayout->addWidget(passwordEdit);

        formSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        cardLayout->addItem(formSpacer);

        saveButton = new QPushButton(cardFrame);
        saveButton->setObjectName("saveButton");
        saveButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        saveButton->setMinimumHeight(45);

        cardLayout->addWidget(saveButton);

        cancelButton = new QPushButton(cardFrame);
        cancelButton->setObjectName("cancelButton");
        cancelButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        cancelButton->setFlat(true);

        cardLayout->addWidget(cancelButton);


        contentLayout->addWidget(cardFrame);

        rightContentSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        contentLayout->addItem(rightContentSpacer);


        mainLayout->addLayout(contentLayout);

        bottomSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        mainLayout->addItem(bottomSpacer);


        retranslateUi(ProfileView);

        QMetaObject::connectSlotsByName(ProfileView);
    } // setupUi

    void retranslateUi(QWidget *ProfileView)
    {
        ProfileView->setStyleSheet(QString());
        headerFrame->setStyleSheet(QCoreApplication::translate("ProfileView", "background-color: #1a1a1a; border-bottom: 1px solid #333333;", nullptr));
        titleLabel->setText(QCoreApplication::translate("ProfileView", "\360\237\221\244 Mi Perfil", nullptr));
        titleLabel->setStyleSheet(QCoreApplication::translate("ProfileView", "font-size: 20px; font-weight: bold; color: white; border: none;", nullptr));
        backToChartButton->setText(QCoreApplication::translate("ProfileView", "Volver a la Carta \360\237\227\272\357\270\217", nullptr));
        backToChartButton->setStyleSheet(QCoreApplication::translate("ProfileView", "\n"
"          QPushButton {\n"
"           background-color: #e94560;\n"
"           color: white;\n"
"           border: none;\n"
"           padding: 8px 16px;\n"
"           border-radius: 6px;\n"
"           font-weight: bold;\n"
"           font-size: 13px;\n"
"          }\n"
"          QPushButton:hover {\n"
"           background-color: #ff6b6b;\n"
"          }\n"
"         ", nullptr));
        cardFrame->setStyleSheet(QCoreApplication::translate("ProfileView", "\n"
"         QFrame#cardFrame {\n"
"          background-color: #1a1a1a;\n"
"          border-radius: 16px;\n"
"          border: 1px solid #333333;\n"
"         }\n"
"         QLabel { color: #cccccc; font-size: 14px; border: none; text-decoration: none; }\n"
"         QLineEdit, QDateEdit {\n"
"          background-color: #2a2a2a;\n"
"          color: white;\n"
"          border: 1px solid #444;\n"
"          border-radius: 8px;\n"
"          padding: 10px;\n"
"          font-size: 13px;\n"
"          text-decoration: none;\n"
"         }\n"
"         QLineEdit:focus, QDateEdit:focus {\n"
"          border-color: #e94560;\n"
"         }\n"
"        ", nullptr));
        avatarLabel->setStyleSheet(QCoreApplication::translate("ProfileView", "background-color: #333; border-radius: 50px; border: 2px solid #e94560;", nullptr));
        avatarLabel->setText(QString());
        changeAvatarButton->setText(QCoreApplication::translate("ProfileView", "Cambiar Foto", nullptr));
        changeAvatarButton->setStyleSheet(QCoreApplication::translate("ProfileView", "color: #3498db; font-weight: bold; font-size: 12px; border: none; margin-top: 5px;", nullptr));
        nickLabel->setText(QCoreApplication::translate("ProfileView", "@usuario", nullptr));
        nickLabel->setStyleSheet(QCoreApplication::translate("ProfileView", "font-size: 18px; color: #888; font-weight: bold; margin-bottom: 20px; border: none;", nullptr));
        lblEmail->setText(QCoreApplication::translate("ProfileView", "Email", nullptr));
        lblBirth->setText(QCoreApplication::translate("ProfileView", "Fecha de Nacimiento", nullptr));
        birthdateEdit->setDisplayFormat(QCoreApplication::translate("ProfileView", "dd/MM/yyyy", nullptr));
        lblNewPass->setText(QCoreApplication::translate("ProfileView", "Nueva Contrase\303\261a (Opcional)", nullptr));
        passwordEdit->setPlaceholderText(QCoreApplication::translate("ProfileView", "\342\200\242\342\200\242\342\200\242\342\200\242\342\200\242\342\200\242\342\200\242\342\200\242", nullptr));
        saveButton->setText(QCoreApplication::translate("ProfileView", "Guardar Cambios", nullptr));
        saveButton->setStyleSheet(QCoreApplication::translate("ProfileView", "\n"
"            QPushButton {\n"
"             background-color: #27ae60;\n"
"             color: white;\n"
"             font-weight: bold;\n"
"             border-radius: 8px;\n"
"             font-size: 15px;\n"
"             border: none;\n"
"            }\n"
"            QPushButton:hover {\n"
"             background-color: #2ecc71;\n"
"            }\n"
"           ", nullptr));
        cancelButton->setText(QCoreApplication::translate("ProfileView", "Cancelar", nullptr));
        cancelButton->setStyleSheet(QCoreApplication::translate("ProfileView", "color: #888; margin-top: 10px; border: none;", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProfileView: public Ui_ProfileView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROFILEVIEW_H
