/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSalir;
    QAction *actionCerrarSesion;
    QAction *actionGuardar;
    QAction *actionDeshacer;
    QAction *actionLimpiar;
    QAction *actionZoomIn;
    QAction *actionZoomOut;
    QAction *actionZoomReset;
    QAction *actionMostrarCoordenadas;
    QAction *actionPerfil;
    QAction *actionEstadisticas;
    QAction *actionManual;
    QAction *actionAcercaDe;
    QWidget *centralwidget;
    QVBoxLayout *centralLayout;
    QStackedWidget *stackedWidget;
    QWidget *loginPage;
    QVBoxLayout *loginPageLayout;
    QSpacerItem *loginTopSpacer;
    QHBoxLayout *loginCenterLayout;
    QSpacerItem *loginLeftSpacer;
    QWidget *loginFormContainer;
    QVBoxLayout *loginFormLayout;
    QLabel *loginTitleLabel;
    QLabel *loginBrandLabel;
    QSpacerItem *loginSpacer1;
    QLineEdit *nickEdit;
    QLineEdit *passwordEdit;
    QLabel *loginErrorLabel;
    QPushButton *loginButton;
    QPushButton *goToRegisterButton;
    QSpacerItem *loginRightSpacer;
    QSpacerItem *loginBottomSpacer;
    QWidget *registerPage;
    QVBoxLayout *registerPageLayout;
    QSpacerItem *regTop;
    QHBoxLayout *regCenter;
    QSpacerItem *regLeft;
    QWidget *registerFormContainer;
    QVBoxLayout *registerFormLayout;
    QLabel *registerTitleLabel;
    QHBoxLayout *avatarLayout;
    QSpacerItem *avl;
    QLabel *avatarPreviewLabel;
    QSpacerItem *avr;
    QPushButton *selectAvatarButton;
    QLineEdit *registerNickEdit;
    QLineEdit *registerEmailEdit;
    QLineEdit *registerPasswordEdit;
    QLineEdit *registerConfirmPasswordEdit;
    QLabel *lblBirthReg;
    QDateEdit *registerBirthdateEdit;
    QLabel *registerErrorLabel;
    QPushButton *registerButton;
    QPushButton *backToLoginButton;
    QSpacerItem *regRight;
    QSpacerItem *regBottom;
    QWidget *dashboardPage;
    QHBoxLayout *dashboardLayout;
    QScrollArea *toolbarScrollArea;
    QWidget *toolbarContent;
    QVBoxLayout *toolbarVLayout;
    QLabel *navSectionLabel;
    QPushButton *toolPan;
    QPushButton *toolZoomIn;
    QPushButton *toolZoomOut;
    QFrame *separator1;
    QLabel *drawSectionLabel;
    QPushButton *toolPoint;
    QPushButton *toolLine;
    QPushButton *toolArc;
    QPushButton *toolText;
    QPushButton *toolEraser;
    QFrame *separator2;
    QLabel *measureSectionLabel;
    QPushButton *toolProtractor;
    QPushButton *toolRuler;
    QFrame *separator3;
    QPushButton *toolUndo;
    QPushButton *toolClear;
    QPushButton *toolQuiz;
    QWidget *chartContainer;
    QVBoxLayout *chartContainerLayout;
    QMenuBar *menubar;
    QMenu *menuArchivo;
    QMenu *menuEditar;
    QMenu *menuVer;
    QMenu *menuUsuario;
    QMenu *menuAyuda;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1400, 900);
        actionSalir = new QAction(MainWindow);
        actionSalir->setObjectName("actionSalir");
        actionCerrarSesion = new QAction(MainWindow);
        actionCerrarSesion->setObjectName("actionCerrarSesion");
        actionGuardar = new QAction(MainWindow);
        actionGuardar->setObjectName("actionGuardar");
        actionDeshacer = new QAction(MainWindow);
        actionDeshacer->setObjectName("actionDeshacer");
        actionLimpiar = new QAction(MainWindow);
        actionLimpiar->setObjectName("actionLimpiar");
        actionZoomIn = new QAction(MainWindow);
        actionZoomIn->setObjectName("actionZoomIn");
        actionZoomOut = new QAction(MainWindow);
        actionZoomOut->setObjectName("actionZoomOut");
        actionZoomReset = new QAction(MainWindow);
        actionZoomReset->setObjectName("actionZoomReset");
        actionMostrarCoordenadas = new QAction(MainWindow);
        actionMostrarCoordenadas->setObjectName("actionMostrarCoordenadas");
        actionMostrarCoordenadas->setCheckable(true);
        actionMostrarCoordenadas->setChecked(true);
        actionPerfil = new QAction(MainWindow);
        actionPerfil->setObjectName("actionPerfil");
        actionEstadisticas = new QAction(MainWindow);
        actionEstadisticas->setObjectName("actionEstadisticas");
        actionManual = new QAction(MainWindow);
        actionManual->setObjectName("actionManual");
        actionAcercaDe = new QAction(MainWindow);
        actionAcercaDe->setObjectName("actionAcercaDe");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralLayout = new QVBoxLayout(centralwidget);
        centralLayout->setSpacing(0);
        centralLayout->setContentsMargins(0, 0, 0, 0);
        centralLayout->setObjectName("centralLayout");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        loginPage = new QWidget();
        loginPage->setObjectName("loginPage");
        loginPageLayout = new QVBoxLayout(loginPage);
        loginPageLayout->setObjectName("loginPageLayout");
        loginTopSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        loginPageLayout->addItem(loginTopSpacer);

        loginCenterLayout = new QHBoxLayout();
        loginCenterLayout->setObjectName("loginCenterLayout");
        loginLeftSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        loginCenterLayout->addItem(loginLeftSpacer);

        loginFormContainer = new QWidget(loginPage);
        loginFormContainer->setObjectName("loginFormContainer");
        loginFormContainer->setMinimumSize(QSize(400, 500));
        loginFormContainer->setMaximumSize(QSize(400, 500));
        loginFormLayout = new QVBoxLayout(loginFormContainer);
        loginFormLayout->setSpacing(20);
        loginFormLayout->setObjectName("loginFormLayout");
        loginFormLayout->setContentsMargins(40, 50, 40, 50);
        loginTitleLabel = new QLabel(loginFormContainer);
        loginTitleLabel->setObjectName("loginTitleLabel");
        loginTitleLabel->setAlignment(Qt::AlignCenter);

        loginFormLayout->addWidget(loginTitleLabel);

        loginBrandLabel = new QLabel(loginFormContainer);
        loginBrandLabel->setObjectName("loginBrandLabel");
        loginBrandLabel->setAlignment(Qt::AlignCenter);

        loginFormLayout->addWidget(loginBrandLabel);

        loginSpacer1 = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        loginFormLayout->addItem(loginSpacer1);

        nickEdit = new QLineEdit(loginFormContainer);
        nickEdit->setObjectName("nickEdit");

        loginFormLayout->addWidget(nickEdit);

        passwordEdit = new QLineEdit(loginFormContainer);
        passwordEdit->setObjectName("passwordEdit");
        passwordEdit->setEchoMode(QLineEdit::Password);

        loginFormLayout->addWidget(passwordEdit);

        loginErrorLabel = new QLabel(loginFormContainer);
        loginErrorLabel->setObjectName("loginErrorLabel");
        loginErrorLabel->setAlignment(Qt::AlignCenter);

        loginFormLayout->addWidget(loginErrorLabel);

        loginButton = new QPushButton(loginFormContainer);
        loginButton->setObjectName("loginButton");
        loginButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        loginButton->setMinimumHeight(50);

        loginFormLayout->addWidget(loginButton);

        goToRegisterButton = new QPushButton(loginFormContainer);
        goToRegisterButton->setObjectName("goToRegisterButton");
        goToRegisterButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        goToRegisterButton->setFlat(true);

        loginFormLayout->addWidget(goToRegisterButton);


        loginCenterLayout->addWidget(loginFormContainer);

        loginRightSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        loginCenterLayout->addItem(loginRightSpacer);


        loginPageLayout->addLayout(loginCenterLayout);

        loginBottomSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        loginPageLayout->addItem(loginBottomSpacer);

        stackedWidget->addWidget(loginPage);
        registerPage = new QWidget();
        registerPage->setObjectName("registerPage");
        registerPageLayout = new QVBoxLayout(registerPage);
        registerPageLayout->setObjectName("registerPageLayout");
        regTop = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        registerPageLayout->addItem(regTop);

        regCenter = new QHBoxLayout();
        regCenter->setObjectName("regCenter");
        regLeft = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        regCenter->addItem(regLeft);

        registerFormContainer = new QWidget(registerPage);
        registerFormContainer->setObjectName("registerFormContainer");
        registerFormContainer->setMinimumSize(QSize(450, 650));
        registerFormContainer->setMaximumSize(QSize(450, 650));
        registerFormLayout = new QVBoxLayout(registerFormContainer);
        registerFormLayout->setSpacing(15);
        registerFormLayout->setObjectName("registerFormLayout");
        registerFormLayout->setContentsMargins(40, 40, 40, 40);
        registerTitleLabel = new QLabel(registerFormContainer);
        registerTitleLabel->setObjectName("registerTitleLabel");
        registerTitleLabel->setAlignment(Qt::AlignCenter);

        registerFormLayout->addWidget(registerTitleLabel);

        avatarLayout = new QHBoxLayout();
        avatarLayout->setObjectName("avatarLayout");
        avl = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        avatarLayout->addItem(avl);

        avatarPreviewLabel = new QLabel(registerFormContainer);
        avatarPreviewLabel->setObjectName("avatarPreviewLabel");
        avatarPreviewLabel->setMinimumSize(QSize(80, 80));
        avatarPreviewLabel->setMaximumSize(QSize(80, 80));
        avatarPreviewLabel->setAlignment(Qt::AlignCenter);

        avatarLayout->addWidget(avatarPreviewLabel);

        avr = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        avatarLayout->addItem(avr);


        registerFormLayout->addLayout(avatarLayout);

        selectAvatarButton = new QPushButton(registerFormContainer);
        selectAvatarButton->setObjectName("selectAvatarButton");
        selectAvatarButton->setFlat(true);
        selectAvatarButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        registerFormLayout->addWidget(selectAvatarButton);

        registerNickEdit = new QLineEdit(registerFormContainer);
        registerNickEdit->setObjectName("registerNickEdit");

        registerFormLayout->addWidget(registerNickEdit);

        registerEmailEdit = new QLineEdit(registerFormContainer);
        registerEmailEdit->setObjectName("registerEmailEdit");

        registerFormLayout->addWidget(registerEmailEdit);

        registerPasswordEdit = new QLineEdit(registerFormContainer);
        registerPasswordEdit->setObjectName("registerPasswordEdit");
        registerPasswordEdit->setEchoMode(QLineEdit::Password);

        registerFormLayout->addWidget(registerPasswordEdit);

        registerConfirmPasswordEdit = new QLineEdit(registerFormContainer);
        registerConfirmPasswordEdit->setObjectName("registerConfirmPasswordEdit");
        registerConfirmPasswordEdit->setEchoMode(QLineEdit::Password);

        registerFormLayout->addWidget(registerConfirmPasswordEdit);

        lblBirthReg = new QLabel(registerFormContainer);
        lblBirthReg->setObjectName("lblBirthReg");

        registerFormLayout->addWidget(lblBirthReg);

        registerBirthdateEdit = new QDateEdit(registerFormContainer);
        registerBirthdateEdit->setObjectName("registerBirthdateEdit");
        registerBirthdateEdit->setCalendarPopup(true);

        registerFormLayout->addWidget(registerBirthdateEdit);

        registerErrorLabel = new QLabel(registerFormContainer);
        registerErrorLabel->setObjectName("registerErrorLabel");
        registerErrorLabel->setAlignment(Qt::AlignCenter);

        registerFormLayout->addWidget(registerErrorLabel);

        registerButton = new QPushButton(registerFormContainer);
        registerButton->setObjectName("registerButton");
        registerButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        registerButton->setMinimumHeight(45);

        registerFormLayout->addWidget(registerButton);

        backToLoginButton = new QPushButton(registerFormContainer);
        backToLoginButton->setObjectName("backToLoginButton");
        backToLoginButton->setFlat(true);
        backToLoginButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        registerFormLayout->addWidget(backToLoginButton);


        regCenter->addWidget(registerFormContainer);

        regRight = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        regCenter->addItem(regRight);


        registerPageLayout->addLayout(regCenter);

        regBottom = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        registerPageLayout->addItem(regBottom);

        stackedWidget->addWidget(registerPage);
        dashboardPage = new QWidget();
        dashboardPage->setObjectName("dashboardPage");
        dashboardLayout = new QHBoxLayout(dashboardPage);
        dashboardLayout->setSpacing(0);
        dashboardLayout->setObjectName("dashboardLayout");
        dashboardLayout->setContentsMargins(0, 0, 0, 0);
        toolbarScrollArea = new QScrollArea(dashboardPage);
        toolbarScrollArea->setObjectName("toolbarScrollArea");
        toolbarScrollArea->setMinimumSize(QSize(90, 0));
        toolbarScrollArea->setMaximumSize(QSize(90, 16777215));
        toolbarScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        toolbarScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        toolbarScrollArea->setWidgetResizable(true);
        toolbarContent = new QWidget();
        toolbarContent->setObjectName("toolbarContent");
        toolbarContent->setGeometry(QRect(0, 0, 90, 800));
        toolbarVLayout = new QVBoxLayout(toolbarContent);
        toolbarVLayout->setSpacing(6);
        toolbarVLayout->setObjectName("toolbarVLayout");
        toolbarVLayout->setContentsMargins(18, 16, 18, 16);
        navSectionLabel = new QLabel(toolbarContent);
        navSectionLabel->setObjectName("navSectionLabel");
        navSectionLabel->setAlignment(Qt::AlignCenter);

        toolbarVLayout->addWidget(navSectionLabel);

        toolPan = new QPushButton(toolbarContent);
        toolPan->setObjectName("toolPan");
        toolPan->setMinimumSize(QSize(60, 50));
        toolPan->setCheckable(true);
        toolPan->setChecked(true);

        toolbarVLayout->addWidget(toolPan);

        toolZoomIn = new QPushButton(toolbarContent);
        toolZoomIn->setObjectName("toolZoomIn");
        toolZoomIn->setMinimumSize(QSize(60, 40));

        toolbarVLayout->addWidget(toolZoomIn);

        toolZoomOut = new QPushButton(toolbarContent);
        toolZoomOut->setObjectName("toolZoomOut");
        toolZoomOut->setMinimumSize(QSize(60, 40));

        toolbarVLayout->addWidget(toolZoomOut);

        separator1 = new QFrame(toolbarContent);
        separator1->setObjectName("separator1");
        separator1->setFrameShape(QFrame::HLine);

        toolbarVLayout->addWidget(separator1);

        drawSectionLabel = new QLabel(toolbarContent);
        drawSectionLabel->setObjectName("drawSectionLabel");
        drawSectionLabel->setAlignment(Qt::AlignCenter);

        toolbarVLayout->addWidget(drawSectionLabel);

        toolPoint = new QPushButton(toolbarContent);
        toolPoint->setObjectName("toolPoint");
        toolPoint->setMinimumSize(QSize(60, 50));
        toolPoint->setCheckable(true);

        toolbarVLayout->addWidget(toolPoint);

        toolLine = new QPushButton(toolbarContent);
        toolLine->setObjectName("toolLine");
        toolLine->setMinimumSize(QSize(60, 50));
        toolLine->setCheckable(true);

        toolbarVLayout->addWidget(toolLine);

        toolArc = new QPushButton(toolbarContent);
        toolArc->setObjectName("toolArc");
        toolArc->setMinimumSize(QSize(60, 50));
        toolArc->setCheckable(true);

        toolbarVLayout->addWidget(toolArc);

        toolText = new QPushButton(toolbarContent);
        toolText->setObjectName("toolText");
        toolText->setMinimumSize(QSize(60, 50));
        toolText->setCheckable(true);

        toolbarVLayout->addWidget(toolText);

        toolEraser = new QPushButton(toolbarContent);
        toolEraser->setObjectName("toolEraser");
        toolEraser->setMinimumSize(QSize(60, 50));
        toolEraser->setCheckable(true);

        toolbarVLayout->addWidget(toolEraser);

        separator2 = new QFrame(toolbarContent);
        separator2->setObjectName("separator2");
        separator2->setFrameShape(QFrame::HLine);

        toolbarVLayout->addWidget(separator2);

        measureSectionLabel = new QLabel(toolbarContent);
        measureSectionLabel->setObjectName("measureSectionLabel");
        measureSectionLabel->setAlignment(Qt::AlignCenter);

        toolbarVLayout->addWidget(measureSectionLabel);

        toolProtractor = new QPushButton(toolbarContent);
        toolProtractor->setObjectName("toolProtractor");
        toolProtractor->setMinimumSize(QSize(60, 50));
        toolProtractor->setCheckable(true);

        toolbarVLayout->addWidget(toolProtractor);

        toolRuler = new QPushButton(toolbarContent);
        toolRuler->setObjectName("toolRuler");
        toolRuler->setMinimumSize(QSize(60, 50));
        toolRuler->setCheckable(true);

        toolbarVLayout->addWidget(toolRuler);

        separator3 = new QFrame(toolbarContent);
        separator3->setObjectName("separator3");
        separator3->setFrameShape(QFrame::HLine);

        toolbarVLayout->addWidget(separator3);

        toolUndo = new QPushButton(toolbarContent);
        toolUndo->setObjectName("toolUndo");
        toolUndo->setMinimumSize(QSize(60, 40));

        toolbarVLayout->addWidget(toolUndo);

        toolClear = new QPushButton(toolbarContent);
        toolClear->setObjectName("toolClear");
        toolClear->setMinimumSize(QSize(60, 40));

        toolbarVLayout->addWidget(toolClear);

        toolQuiz = new QPushButton(toolbarContent);
        toolQuiz->setObjectName("toolQuiz");
        toolQuiz->setMinimumSize(QSize(60, 50));

        toolbarVLayout->addWidget(toolQuiz);

        toolbarScrollArea->setWidget(toolbarContent);

        dashboardLayout->addWidget(toolbarScrollArea);

        chartContainer = new QWidget(dashboardPage);
        chartContainer->setObjectName("chartContainer");
        chartContainerLayout = new QVBoxLayout(chartContainer);
        chartContainerLayout->setSpacing(0);
        chartContainerLayout->setContentsMargins(0, 0, 0, 0);
        chartContainerLayout->setObjectName("chartContainerLayout");
        chartContainerLayout->setContentsMargins(0, 0, 0, 0);

        dashboardLayout->addWidget(chartContainer);

        stackedWidget->addWidget(dashboardPage);

        centralLayout->addWidget(stackedWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1400, 28));
        menuArchivo = new QMenu(menubar);
        menuArchivo->setObjectName("menuArchivo");
        menuEditar = new QMenu(menubar);
        menuEditar->setObjectName("menuEditar");
        menuVer = new QMenu(menubar);
        menuVer->setObjectName("menuVer");
        menuUsuario = new QMenu(menubar);
        menuUsuario->setObjectName("menuUsuario");
        menuAyuda = new QMenu(menubar);
        menuAyuda->setObjectName("menuAyuda");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuArchivo->menuAction());
        menubar->addAction(menuEditar->menuAction());
        menubar->addAction(menuVer->menuAction());
        menubar->addAction(menuUsuario->menuAction());
        menubar->addAction(menuAyuda->menuAction());
        menuArchivo->addAction(actionGuardar);
        menuArchivo->addSeparator();
        menuArchivo->addAction(actionCerrarSesion);
        menuArchivo->addAction(actionSalir);
        menuEditar->addAction(actionDeshacer);
        menuEditar->addAction(actionLimpiar);
        menuVer->addAction(actionZoomIn);
        menuVer->addAction(actionZoomOut);
        menuVer->addAction(actionZoomReset);
        menuVer->addSeparator();
        menuVer->addAction(actionMostrarCoordenadas);
        menuUsuario->addAction(actionPerfil);
        menuUsuario->addAction(actionEstadisticas);
        menuAyuda->addAction(actionManual);
        menuAyuda->addAction(actionAcercaDe);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Patr\303\263n de Embarcaci\303\263n de Recreo - Carta N\303\241utica Digital", nullptr));
        MainWindow->setStyleSheet(QString());
        actionSalir->setText(QCoreApplication::translate("MainWindow", "Salir", nullptr));
#if QT_CONFIG(shortcut)
        actionSalir->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionCerrarSesion->setText(QCoreApplication::translate("MainWindow", "Cerrar Sesi\303\263n", nullptr));
        actionGuardar->setText(QCoreApplication::translate("MainWindow", "Guardar Trabajo", nullptr));
#if QT_CONFIG(shortcut)
        actionGuardar->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionDeshacer->setText(QCoreApplication::translate("MainWindow", "Deshacer", nullptr));
#if QT_CONFIG(shortcut)
        actionDeshacer->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Z", nullptr));
#endif // QT_CONFIG(shortcut)
        actionLimpiar->setText(QCoreApplication::translate("MainWindow", "Limpiar Carta", nullptr));
        actionZoomIn->setText(QCoreApplication::translate("MainWindow", "Acercar", nullptr));
#if QT_CONFIG(shortcut)
        actionZoomIn->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl++", nullptr));
#endif // QT_CONFIG(shortcut)
        actionZoomOut->setText(QCoreApplication::translate("MainWindow", "Alejar", nullptr));
#if QT_CONFIG(shortcut)
        actionZoomOut->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+-", nullptr));
#endif // QT_CONFIG(shortcut)
        actionZoomReset->setText(QCoreApplication::translate("MainWindow", "Zoom 100%", nullptr));
#if QT_CONFIG(shortcut)
        actionZoomReset->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+0", nullptr));
#endif // QT_CONFIG(shortcut)
        actionMostrarCoordenadas->setText(QCoreApplication::translate("MainWindow", "Mostrar Coordenadas", nullptr));
        actionPerfil->setText(QCoreApplication::translate("MainWindow", "Mi Perfil", nullptr));
        actionEstadisticas->setText(QCoreApplication::translate("MainWindow", "Estad\303\255sticas", nullptr));
        actionManual->setText(QCoreApplication::translate("MainWindow", "Manual de Uso", nullptr));
#if QT_CONFIG(shortcut)
        actionManual->setShortcut(QCoreApplication::translate("MainWindow", "F1", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAcercaDe->setText(QCoreApplication::translate("MainWindow", "Acerca de...", nullptr));
        loginPage->setStyleSheet(QString());
        loginFormContainer->setStyleSheet(QString());
        loginTitleLabel->setText(QCoreApplication::translate("MainWindow", "\342\232\223", nullptr));
        loginTitleLabel->setStyleSheet(QString());
        loginBrandLabel->setText(QCoreApplication::translate("MainWindow", "Patr\303\263n Digital", nullptr));
        loginBrandLabel->setStyleSheet(QString());
        nickEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Usuario", nullptr));
        passwordEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Contrase\303\261a", nullptr));
        loginErrorLabel->setText(QString());
        loginErrorLabel->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #e94560; font-size: 12px; border: none;", nullptr));
        loginButton->setText(QCoreApplication::translate("MainWindow", "Iniciar Sesi\303\263n", nullptr));
        loginButton->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"                  QPushButton {\n"
"                   background-color: #e94560;\n"
"                   color: white;\n"
"                   font-size: 16px;\n"
"                   font-weight: bold;\n"
"                   border-radius: 8px;\n"
"                   border: none;\n"
"                  }\n"
"                  QPushButton:hover {\n"
"                   background-color: #ff6b6b;\n"
"                  }\n"
"                 ", nullptr));
        goToRegisterButton->setText(QCoreApplication::translate("MainWindow", "Crear cuenta nueva", nullptr));
        goToRegisterButton->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #888888; font-size: 12px; border: none;", nullptr));
        registerPage->setStyleSheet(QString());
        registerFormContainer->setStyleSheet(QString());
        registerTitleLabel->setText(QCoreApplication::translate("MainWindow", "Crear Cuenta", nullptr));
        registerTitleLabel->setStyleSheet(QString());
        avatarPreviewLabel->setText(QCoreApplication::translate("MainWindow", "\360\237\223\267", nullptr));
        avatarPreviewLabel->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #333; border-radius: 40px; font-size: 24px; border: 1px dashed #555;", nullptr));
        selectAvatarButton->setText(QCoreApplication::translate("MainWindow", "Seleccionar Avatar", nullptr));
        selectAvatarButton->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #3498db; font-size: 12px; border: none;", nullptr));
        registerNickEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Usuario (6-15 caracteres)", nullptr));
        registerEmailEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Correo electr\303\263nico", nullptr));
        registerPasswordEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Contrase\303\261a (8-20, A-Z, 0-9, #)", nullptr));
        registerConfirmPasswordEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Confirmar contrase\303\261a", nullptr));
        lblBirthReg->setText(QCoreApplication::translate("MainWindow", "Fecha de Nacimiento:", nullptr));
        registerBirthdateEdit->setDisplayFormat(QCoreApplication::translate("MainWindow", "dd/MM/yyyy", nullptr));
        registerErrorLabel->setText(QString());
        registerErrorLabel->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #e94560; font-size: 11px; border: none;", nullptr));
        registerButton->setText(QCoreApplication::translate("MainWindow", "Registrarse", nullptr));
        registerButton->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"                  QPushButton {\n"
"                   background-color: #27ae60;\n"
"                   color: white;\n"
"                   font-size: 15px;\n"
"                   font-weight: bold;\n"
"                   border-radius: 8px;\n"
"                   border: none;\n"
"                  }\n"
"                  QPushButton:hover {\n"
"                   background-color: #2ecc71;\n"
"                  }\n"
"                 ", nullptr));
        backToLoginButton->setText(QCoreApplication::translate("MainWindow", "Ya tengo cuenta", nullptr));
        backToLoginButton->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #888; font-size: 12px; border: none;", nullptr));
        dashboardPage->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: transparent;", nullptr));
        toolbarScrollArea->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"            QScrollArea { \n"
"              background-color: #0d0d0d; \n"
"              border: none; \n"
"              border-radius: 16px;\n"
"            }\n"
"            QWidget#toolbarContent { \n"
"              background-color: #0d0d0d; \n"
"              border-radius: 16px;\n"
"            }\n"
"           ", nullptr));
        navSectionLabel->setText(QCoreApplication::translate("MainWindow", "Navegar", nullptr));
        navSectionLabel->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #888888; font-size: 9px; font-weight: bold; background-color: #0d0d0d; padding: 4px 8px; border-radius: 6px;", nullptr));
        toolPan->setText(QCoreApplication::translate("MainWindow", "\342\234\213", nullptr));
#if QT_CONFIG(tooltip)
        toolPan->setToolTip(QCoreApplication::translate("MainWindow", "Mover carta (Pan)", nullptr));
#endif // QT_CONFIG(tooltip)
        toolPan->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"                QPushButton { \n"
"                 background-color: #0f3460; \n"
"                 color: white; \n"
"                 border-radius: 10px; \n"
"                 font-size: 22px; \n"
"                 border: 2px solid transparent;\n"
"                } \n"
"                QPushButton:checked { \n"
"                 background-color: #e94560; \n"
"                 border-color: #ff6b6b;\n"
"                }\n"
"                QPushButton:hover:!checked { \n"
"                 background-color: #1a4a7a; \n"
"                }\n"
"               ", nullptr));
        toolZoomIn->setText(QCoreApplication::translate("MainWindow", "\360\237\224\215+", nullptr));
#if QT_CONFIG(tooltip)
        toolZoomIn->setToolTip(QCoreApplication::translate("MainWindow", "Acercar (Zoom In)", nullptr));
#endif // QT_CONFIG(tooltip)
        toolZoomIn->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"                QPushButton { \n"
"                 background-color: #0f3460; \n"
"                 color: white; \n"
"                 border-radius: 8px; \n"
"                 font-size: 14px;\n"
"                } \n"
"                QPushButton:pressed { background-color: #e94560; }\n"
"                QPushButton:hover { background-color: #1a4a7a; }\n"
"               ", nullptr));
        toolZoomOut->setText(QCoreApplication::translate("MainWindow", "\360\237\224\215-", nullptr));
#if QT_CONFIG(tooltip)
        toolZoomOut->setToolTip(QCoreApplication::translate("MainWindow", "Alejar (Zoom Out)", nullptr));
#endif // QT_CONFIG(tooltip)
        toolZoomOut->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"                QPushButton { \n"
"                 background-color: #0f3460; \n"
"                 color: white; \n"
"                 border-radius: 8px; \n"
"                 font-size: 14px;\n"
"                } \n"
"                QPushButton:pressed { background-color: #e94560; }\n"
"                QPushButton:hover { background-color: #1a4a7a; }\n"
"               ", nullptr));
        separator1->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #333333; margin: 8px 0;", nullptr));
        drawSectionLabel->setText(QCoreApplication::translate("MainWindow", "Dibujar", nullptr));
        drawSectionLabel->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #888888; font-size: 9px; font-weight: bold; background-color: #0d0d0d; padding: 4px 8px; border-radius: 6px;", nullptr));
        toolPoint->setText(QCoreApplication::translate("MainWindow", "\360\237\223\215", nullptr));
#if QT_CONFIG(tooltip)
        toolPoint->setToolTip(QCoreApplication::translate("MainWindow", "Marcar Punto", nullptr));
#endif // QT_CONFIG(tooltip)
        toolPoint->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"                QPushButton { \n"
"                 background-color: #0f3460; \n"
"                 color: white; \n"
"                 border-radius: 10px; \n"
"                 font-size: 22px;\n"
"                 border: 2px solid transparent;\n"
"                } \n"
"                QPushButton:checked { \n"
"                 background-color: #e94560;\n"
"                 border-color: #ff6b6b;\n"
"                }\n"
"                QPushButton:hover:!checked { background-color: #1a4a7a; }\n"
"               ", nullptr));
        toolLine->setText(QCoreApplication::translate("MainWindow", "\360\237\223\220", nullptr));
#if QT_CONFIG(tooltip)
        toolLine->setToolTip(QCoreApplication::translate("MainWindow", "Trazar L\303\255nea/Rumbo", nullptr));
#endif // QT_CONFIG(tooltip)
        toolLine->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"                QPushButton { \n"
"                 background-color: #0f3460; \n"
"                 color: white; \n"
"                 border-radius: 10px; \n"
"                 font-size: 22px;\n"
"                 border: 2px solid transparent;\n"
"                } \n"
"                QPushButton:checked { \n"
"                 background-color: #e94560;\n"
"                 border-color: #ff6b6b;\n"
"                }\n"
"                QPushButton:hover:!checked { background-color: #1a4a7a; }\n"
"               ", nullptr));
        toolArc->setText(QCoreApplication::translate("MainWindow", "\342\227\240", nullptr));
#if QT_CONFIG(tooltip)
        toolArc->setToolTip(QCoreApplication::translate("MainWindow", "Dibujar Arco (Comp\303\241s)", nullptr));
#endif // QT_CONFIG(tooltip)
        toolArc->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"                QPushButton { \n"
"                 background-color: #0f3460; \n"
"                 color: white; \n"
"                 border-radius: 10px; \n"
"                 font-size: 28px;\n"
"                 border: 2px solid transparent;\n"
"                } \n"
"                QPushButton:checked { \n"
"                 background-color: #e94560;\n"
"                 border-color: #ff6b6b;\n"
"                }\n"
"                QPushButton:hover:!checked { background-color: #1a4a7a; }\n"
"               ", nullptr));
        toolText->setText(QCoreApplication::translate("MainWindow", "T", nullptr));
#if QT_CONFIG(tooltip)
        toolText->setToolTip(QCoreApplication::translate("MainWindow", "A\303\261adir Texto", nullptr));
#endif // QT_CONFIG(tooltip)
        toolText->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"                QPushButton { \n"
"                 background-color: #0f3460; \n"
"                 color: white; \n"
"                 border-radius: 10px; \n"
"                 font-size: 20px;\n"
"                 font-weight: bold;\n"
"                 border: 2px solid transparent;\n"
"                } \n"
"                QPushButton:checked { \n"
"                 background-color: #e94560;\n"
"                 border-color: #ff6b6b;\n"
"                }\n"
"                QPushButton:hover:!checked { background-color: #1a4a7a; }\n"
"               ", nullptr));
        toolEraser->setText(QCoreApplication::translate("MainWindow", "\360\237\247\275", nullptr));
#if QT_CONFIG(tooltip)
        toolEraser->setToolTip(QCoreApplication::translate("MainWindow", "Borrar Elemento", nullptr));
#endif // QT_CONFIG(tooltip)
        toolEraser->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"                QPushButton { \n"
"                 background-color: #0f3460; \n"
"                 color: white; \n"
"                 border-radius: 10px; \n"
"                 font-size: 22px;\n"
"                 border: 2px solid transparent;\n"
"                } \n"
"                QPushButton:checked { \n"
"                 background-color: #e94560;\n"
"                 border-color: #ff6b6b;\n"
"                }\n"
"                QPushButton:hover:!checked { background-color: #1a4a7a; }\n"
"               ", nullptr));
        separator2->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #333333; margin: 8px 0;", nullptr));
        measureSectionLabel->setText(QCoreApplication::translate("MainWindow", "Medir", nullptr));
        measureSectionLabel->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #888888; font-size: 9px; font-weight: bold; background-color: #0d0d0d; padding: 4px 8px; border-radius: 6px;", nullptr));
        toolProtractor->setText(QCoreApplication::translate("MainWindow", "\360\237\247\255", nullptr));
#if QT_CONFIG(tooltip)
        toolProtractor->setToolTip(QCoreApplication::translate("MainWindow", "Transportador de \303\201ngulos", nullptr));
#endif // QT_CONFIG(tooltip)
        toolProtractor->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"                QPushButton { \n"
"                 background-color: #0f3460; \n"
"                 color: white; \n"
"                 border-radius: 10px; \n"
"                 font-size: 22px;\n"
"                 border: 2px solid transparent;\n"
"                } \n"
"                QPushButton:checked { \n"
"                 background-color: #27ae60;\n"
"                 border-color: #2ecc71;\n"
"                }\n"
"                QPushButton:hover:!checked { background-color: #1a4a7a; }\n"
"               ", nullptr));
        toolRuler->setText(QCoreApplication::translate("MainWindow", "\360\237\223\217", nullptr));
#if QT_CONFIG(tooltip)
        toolRuler->setToolTip(QCoreApplication::translate("MainWindow", "Regla / Medir Distancia", nullptr));
#endif // QT_CONFIG(tooltip)
        toolRuler->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"                QPushButton { \n"
"                 background-color: #0f3460; \n"
"                 color: white; \n"
"                 border-radius: 10px; \n"
"                 font-size: 22px;\n"
"                 border: 2px solid transparent;\n"
"                } \n"
"                QPushButton:checked { \n"
"                 background-color: #27ae60;\n"
"                 border-color: #2ecc71;\n"
"                }\n"
"                QPushButton:hover:!checked { background-color: #1a4a7a; }\n"
"               ", nullptr));
        separator3->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #333333; margin: 8px 0;", nullptr));
        toolUndo->setText(QCoreApplication::translate("MainWindow", "\342\206\251\357\270\217", nullptr));
#if QT_CONFIG(tooltip)
        toolUndo->setToolTip(QCoreApplication::translate("MainWindow", "Deshacer (Ctrl+Z)", nullptr));
#endif // QT_CONFIG(tooltip)
        toolUndo->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"                QPushButton { \n"
"                 background-color: #0f3460; \n"
"                 color: white; \n"
"                 border-radius: 8px; \n"
"                 font-size: 18px;\n"
"                } \n"
"                QPushButton:pressed { background-color: #e94560; }\n"
"                QPushButton:hover { background-color: #1a4a7a; }\n"
"               ", nullptr));
        toolClear->setText(QCoreApplication::translate("MainWindow", "\360\237\227\221\357\270\217", nullptr));
#if QT_CONFIG(tooltip)
        toolClear->setToolTip(QCoreApplication::translate("MainWindow", "Limpiar Todo", nullptr));
#endif // QT_CONFIG(tooltip)
        toolClear->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"                QPushButton { \n"
"                 background-color: #c0392b; \n"
"                 color: white; \n"
"                 border-radius: 8px; \n"
"                 font-size: 18px;\n"
"                } \n"
"                QPushButton:pressed { background-color: #e74c3c; }\n"
"                QPushButton:hover { background-color: #e74c3c; }\n"
"               ", nullptr));
        toolQuiz->setText(QCoreApplication::translate("MainWindow", "\360\237\223\235", nullptr));
#if QT_CONFIG(tooltip)
        toolQuiz->setToolTip(QCoreApplication::translate("MainWindow", "Iniciar Quiz de Navegaci\303\263n", nullptr));
#endif // QT_CONFIG(tooltip)
        toolQuiz->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"                QPushButton { \n"
"                 background-color: #f39c12; \n"
"                 color: white; \n"
"                 border-radius: 10px; \n"
"                 font-size: 22px;\n"
"                } \n"
"                QPushButton:pressed { background-color: #e67e22; }\n"
"                QPushButton:hover { background-color: #e67e22; }\n"
"               ", nullptr));
        chartContainer->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: transparent;", nullptr));
        menuArchivo->setTitle(QCoreApplication::translate("MainWindow", "Archivo", nullptr));
        menuEditar->setTitle(QCoreApplication::translate("MainWindow", "Editar", nullptr));
        menuVer->setTitle(QCoreApplication::translate("MainWindow", "Ver", nullptr));
        menuUsuario->setTitle(QCoreApplication::translate("MainWindow", "Usuario", nullptr));
        menuAyuda->setTitle(QCoreApplication::translate("MainWindow", "Ayuda", nullptr));
        statusbar->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"     QStatusBar { \n"
"      background-color: #0d0d0d; \n"
"      color: #ffffff; \n"
"      padding: 8px 16px;\n"
"      font-size: 12px;\n"
"     }\n"
"    ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
