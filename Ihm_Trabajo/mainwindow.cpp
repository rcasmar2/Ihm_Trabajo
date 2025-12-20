#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "controllers/chartcontroller.h"
#include "controllers/logincontroller.h"
#include "controllers/registercontroller.h"
#include "controllers/profilecontroller.h"
#include "controllers/sessioncontroller.h"
#include "navigation.h"
#include "utils/charttypes.h"
#include "utils/validators.h"
#include "views/profileview.h"
#include "views/resultsview.h"
#include "views/quizview.h"
#include "widgets/chartwidget.h"
#include "widgets/strokesettingspopup.h"

#include <QButtonGroup>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFrame>
#include <QHBoxLayout>
#include <QIcon>
#include <QImage>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>
#include <QUrl>
#include <QToolButton>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_loginController(nullptr),
    m_registerController(nullptr), m_sessionController(nullptr), m_profileController(nullptr),
    m_chartWidget(nullptr), m_coordLabel(nullptr), m_zoomLabel(nullptr),
    m_angleLabel(nullptr), m_toolGroup(nullptr), m_strokePopup(nullptr), m_profileView(nullptr), m_resultsView(nullptr), m_quizView(nullptr) {
    ui->setupUi(this);

    setupControllers();
    setupChartWidget();
    setupViews(); // Instantiate new views
    setupToolbar();
    setupStatusBar();
    setupConnections();
    setupRegisterValidation(); // Setup real-time validation

    // Mostrar la página de login al iniciar
    showLoginPage();

    // Inicializar indicador con la herramienta por defecto (Pan -> "Moverse")
    onToolChanged(static_cast<int>(ToolMode::Pan));
}

MainWindow::~MainWindow() { delete ui; }

// === SETUP ===

void MainWindow::setupControllers() {
    m_loginController = new LoginController(this);
    m_registerController = new RegisterController(this);
    m_registerController = new RegisterController(this);
    m_sessionController = new SessionController(this);
    m_profileController = new ProfileController(this);
}

void MainWindow::setupViews() {
    // Instantiate views
    m_profileView = new ProfileView(this);
    m_resultsView = new ResultsView(this);
    m_quizView = new QuizView(this);

    // Pass controllers
    m_profileView->setController(m_profileController);
    m_resultsView->setController(m_sessionController);
    m_quizView->setController(m_sessionController);

    // Add to StackedWidget (quizView is now a dialog, not added here)
    ui->stackedWidget->addWidget(m_profileView); // Index 3
    ui->stackedWidget->addWidget(m_resultsView); // Index 4
    
    // Connect back signals - quizView closes itself as dialog
}

void MainWindow::setupChartWidget() {
    m_chartWidget = new ChartWidget(this);
    ui->chartContainerLayout->addWidget(m_chartWidget);
}

void MainWindow::setupToolbar() {
    // Crear grupo de botones mutuamente exclusivos para herramientas
    m_toolGroup = new QButtonGroup(this);
    m_toolGroup->setExclusive(true);

    // Añadir botones al grupo con IDs correspondientes a ToolMode
    m_toolGroup->addButton(ui->toolPan, static_cast<int>(ToolMode::Pan));
    m_toolGroup->addButton(ui->toolPoint, static_cast<int>(ToolMode::Point));
    m_toolGroup->addButton(ui->toolLine, static_cast<int>(ToolMode::Line));
    m_toolGroup->addButton(ui->toolArc, static_cast<int>(ToolMode::Arc));
    m_toolGroup->addButton(ui->toolText, static_cast<int>(ToolMode::Text));
    m_toolGroup->addButton(ui->toolEraser, static_cast<int>(ToolMode::Eraser));
    m_toolGroup->addButton(ui->toolProtractor,
                           static_cast<int>(ToolMode::Protractor));
    m_toolGroup->addButton(ui->toolRuler, static_cast<int>(ToolMode::Ruler));

    // Seleccionar Pan por defecto
    ui->toolPan->setChecked(true);

    // === FUNCIÓN PARA CREAR ICONOS BLANCOS ===
    auto createWhiteIcon = [](const QString &iconPath) -> QIcon {
        QPixmap pixmap(iconPath);
        if (pixmap.isNull())
            return QIcon(iconPath);

        // Crear una imagen con el pixmap
        QImage img = pixmap.toImage();

        // Colorear todos los píxeles negros/oscuros a blanco
        for (int y = 0; y < img.height(); ++y) {
            for (int x = 0; x < img.width(); ++x) {
                QColor color = img.pixelColor(x, y);
                if (color.alpha() > 0) {
                    // Mantener el alpha, pero hacer el color blanco
                    img.setPixelColor(x, y, QColor(255, 255, 255, color.alpha()));
                }
            }
        }
        return QIcon(QPixmap::fromImage(img));
    };

    // === CONFIGURAR BOTONES CON ICONOS BLANCOS ===
    auto setupToolButton =
        [&createWhiteIcon](QPushButton *btn, const QString &iconPath,
                           const QString &bgColor = "#1a1a1a") {
            btn->setText("");
            btn->setIcon(createWhiteIcon(iconPath));
            btn->setIconSize(QSize(24, 24));
            QString style = QString(R"(
            QPushButton {
                background-color: %1;
                border-radius: 12px;
                border: 2px solid transparent;
            }
            QPushButton:checked {
                background-color: #e94560;
                border-color: #ff6b6b;
            }
            QPushButton:hover:!checked {
                background-color: #2a2a2a;
            }
            QPushButton:pressed {
                background-color: #e94560;
            }
        )")
                                .arg(bgColor);
            btn->setStyleSheet(style);
        };

    // Navegación
    
    // === Botón Mover/Seleccionar (Combo) ===
    QToolButton *btnMode = new QToolButton(this);
    btnMode->setPopupMode(QToolButton::InstantPopup);
    btnMode->setToolTip("Herramienta de Navegación");
    btnMode->setToolButtonStyle(Qt::ToolButtonIconOnly);
    btnMode->setFixedSize(60, 50);
    // Estilo base con triángulo pequeño en esquina inferior derecha
    btnMode->setStyleSheet(R"(
        QToolButton { 
            background-color: #1a1a1a; 
            color: white; 
            border: 2px solid transparent; 
            border-radius: 12px;
            padding: 4px;
        }
        QToolButton::menu-indicator {
            image: url(:/resources/icons/triangle_small.svg);
            width: 6px;
            height: 6px;
            padding-right: 4px;
            padding-bottom: 4px;
        }
        QToolButton:checked {
            background-color: #e94560;
            border-color: #ff6b6b;
        }
        QToolButton:hover:!checked {
            background-color: #2a2a2a;
        }
    )");

    // Menú desplegable
    QMenu *modeMenu = new QMenu(btnMode);
    modeMenu->setStyleSheet("QMenu { background-color: #1a1a1a; color: white; border: 1px solid #333; } QMenu::item:selected { background-color: #e94560; }");
    
    QAction *actPan = modeMenu->addAction(createWhiteIcon(":/resources/icons/hand.svg"), "Moverse (Mano)");
    QAction *actSelect = modeMenu->addAction(createWhiteIcon(":/resources/icons/cursor.svg"), "Seleccionar");

    btnMode->setMenu(modeMenu);
    
    // Icono inicial
    btnMode->setIcon(createWhiteIcon(":/resources/icons/hand.svg"));
    btnMode->setIconSize(QSize(24, 24));
    
    // Conexiones del menú
    connect(actPan, &QAction::triggered, this, [=]() {
        m_chartWidget->controller()->setTool(ToolMode::Pan);
        btnMode->setIcon(createWhiteIcon(":/resources/icons/hand.svg"));
    });
    
    connect(actSelect, &QAction::triggered, this, [=]() {
        m_chartWidget->controller()->setTool(ToolMode::Select);
        // Si no existe cursor.svg, usar fallback
        QIcon icon = createWhiteIcon(":/resources/icons/cursor.svg");
        if (icon.isNull()) {
            // Fallback visual si no hay icono
            btnMode->setText("Sel"); 
        } else {
            btnMode->setIcon(icon);
        }
    });

    // Acción principal del botón (click directo) -> Alternar o mantener actual?
    // Generalmente repite la última acción o abre menú. Con MenuButtonPopup, el click ejecuta defaultAction.
    // Vamos a hacer que el click principal active la herramienta que muestra el icono actualmente.
    // Pero QToolButton::MenuButtonPopup separa el click del menú.
    connect(btnMode, &QToolButton::clicked, this, [=]() {
        // Ejecutar la herramienta activa visualmente
        // Esto requiere saber cuál es estado actual. Lo podemos deducir del controller.
        ToolMode current = m_chartWidget->controller()->currentTool();
        if (current == ToolMode::Pan) {
            m_chartWidget->controller()->setTool(ToolMode::Pan);
        } else if (current == ToolMode::Select) {
             m_chartWidget->controller()->setTool(ToolMode::Select);
        } else {
            // Si venimos de otra herramienta (ej. Lápiz), volver a Pan por defecto
            m_chartWidget->controller()->setTool(ToolMode::Pan);
            btnMode->setIcon(createWhiteIcon(":/resources/icons/hand.svg"));
        }
    });

    // Insertar en toolbar (reemplazando toolPan viejo si es posible, o insertando al inicio)
    if (auto layout = ui->centralwidget->findChild<QVBoxLayout*>("toolbarVLayout")) {
        // Remover toolPan original del layout si existe
        layout->removeWidget(ui->toolPan);
        ui->toolPan->hide(); // Ocultarlo
        
        layout->insertWidget(1, btnMode); // Index 1 (tras etiqueta)
    }
    
    // Añadir al grupo para exclusividad visual (aunque el QToolButton no es checkable igual que los otros, 
    // lo haremos checkable para que se ilumine cuando activas Pan o Select)
    btnMode->setCheckable(true);
    btnMode->setChecked(true); // Default Pan
    
    // El grupo de herramientas necesita IDs únicos. 
    // Como Pan y Select son modos distintos, pero comparten botón... esto es complejo para QButtonGroup.
    // Solución: No añadir este botón al grupo principal de forma estándar, O
    // gestionar su estado visual manualmente en updateToolButtonStates.
    // Vamos a gestionarlo manualmente y sacar 'Pan' y 'Select' del grupo automático si da problemas,
    // pero Select y Pan son ToolModes.
    // Mejor: asignamos ID Pan al botón. Si estamos en Select, también lo iluminamos?
    // O mejor, asignamos ID Pan. UpdateToolStates iluminará este botón si mode == Pan.
    // Si mode == Select, tendremos que iluminarlo también.
    m_toolGroup->addButton(btnMode, static_cast<int>(ToolMode::Pan)); 
    // EL ID Select no tendrá botón en el grupo, lo gestionaremos en el switch de updateToolButtonStates.

    setupToolButton(ui->toolZoomIn, ":/resources/icons/zoom-in.svg");
    setupToolButton(ui->toolZoomOut, ":/resources/icons/zoom-out.svg");

    // Dibujo
    setupToolButton(ui->toolPoint, ":/resources/icons/point.svg");
    setupToolButton(ui->toolLine, ":/resources/icons/line.svg");
    setupToolButton(ui->toolArc, ":/resources/icons/compass_icon_bar.svg");
    setupToolButton(ui->toolText, ":/resources/icons/text.svg");
    setupToolButton(ui->toolEraser, ":/resources/icons/eraser.svg");

    // === Botón Herramientas de Medición (Combo) ===
    QToolButton *btnMeasure = new QToolButton(this);
    btnMeasure->setPopupMode(QToolButton::InstantPopup);
    btnMeasure->setToolTip("Herramientas de Medición");
    btnMeasure->setToolButtonStyle(Qt::ToolButtonIconOnly);
    btnMeasure->setFixedSize(60, 50);
    btnMeasure->setStyleSheet(R"(
        QToolButton { 
            background-color: #1a1a1a; 
            color: white; 
            border: 2px solid transparent; 
            border-radius: 12px;
            padding: 4px;
        }
        QToolButton::menu-indicator {
            image: url(:/resources/icons/triangle_small.svg);
            width: 6px;
            height: 6px;
            padding-right: 4px;
            padding-bottom: 4px;
        }
        QToolButton:checked {
            background-color: #27ae60;
            border-color: #2ecc71;
        }
        QToolButton:hover:!checked {
            background-color: #2a2a2a;
        }
    )");

    // Menú de herramientas de medición (solo regla y transportador)
    QMenu *measureMenu = new QMenu(btnMeasure);
    measureMenu->setStyleSheet("QMenu { background-color: #1a1a1a; color: white; border: 1px solid #333; } QMenu::item:selected { background-color: #27ae60; }");
    
    QAction *actRuler = measureMenu->addAction(createWhiteIcon(":/resources/icons/rule_icon_bar.svg"), "Regla");
    QAction *actProtractor = measureMenu->addAction(createWhiteIcon(":/resources/icons/transportador_icon_bar.svg"), "Transportador");

    btnMeasure->setMenu(measureMenu);
    
    // Icono inicial (regla)
    btnMeasure->setIcon(createWhiteIcon(":/resources/icons/rule_icon_bar.svg"));
    btnMeasure->setIconSize(QSize(24, 24));
    
    // Conexiones del menú de medición
    connect(actRuler, &QAction::triggered, this, [=]() {
        m_chartWidget->controller()->setTool(ToolMode::Ruler);
        btnMeasure->setIcon(createWhiteIcon(":/resources/icons/rule_icon_bar.svg"));
    });
    
    connect(actProtractor, &QAction::triggered, this, [=]() {
        m_chartWidget->controller()->setTool(ToolMode::Protractor);
        btnMeasure->setIcon(createWhiteIcon(":/resources/icons/transportador_icon_bar.svg"));
    });

    // Ocultar botones originales de medición
    ui->toolProtractor->hide();
    ui->toolRuler->hide();
    
    // Insertar botón de medición en el layout (en la sección Medir)
    if (auto layout = ui->centralwidget->findChild<QVBoxLayout*>("toolbarVLayout")) {
        // Insertar después de los botones de dibujo, en la posición de toolProtractor
        int insertIndex = layout->indexOf(ui->toolProtractor);
        if (insertIndex >= 0) {
            layout->insertWidget(insertIndex, btnMeasure);
        }
    }
    
    // Añadir al grupo para exclusividad (usar ID de Ruler como default)
    btnMeasure->setCheckable(true);
    m_toolGroup->addButton(btnMeasure, static_cast<int>(ToolMode::Ruler));

    // Acciones
    setupToolButton(ui->toolUndo, ":/resources/icons/undo-2.svg");
    setupToolButton(ui->toolClear, ":/resources/icons/trash-2.svg");

    // Quiz (naranja con icono negro que contrasta bien)
    ui->toolQuiz->setText("");
    ui->toolQuiz->setIcon(QIcon(":/resources/icons/quiz.svg"));
    ui->toolQuiz->setIconSize(QSize(24, 24));
    ui->toolQuiz->setStyleSheet(R"(
        QPushButton {
            background-color: #f39c12;
            border-radius: 12px;
        }
        QPushButton:pressed { background-color: #e67e22; }
        QPushButton:hover { background-color: #f1c40f; }
    )");

    connect(ui->toolQuiz, &QPushButton::clicked, this, &MainWindow::onStartQuiz);

    // Crear popup de configuración de trazo
    m_strokePopup = new StrokeSettingsPopup(this);
    m_strokePopup->setColor(m_chartWidget->controller()->strokeColor());
    m_strokePopup->setStrokeWidth(m_chartWidget->controller()->strokeWidth());

    // Conectar cambios del popup al controller
    connect(m_strokePopup, &StrokeSettingsPopup::colorChanged, this,
            &MainWindow::onStrokeColorChanged);
    connect(m_strokePopup, &StrokeSettingsPopup::strokeWidthChanged, this,
            &MainWindow::onStrokeWidthChanged);
    connect(m_strokePopup, &StrokeSettingsPopup::fontChanged,
            m_chartWidget->controller(), &ChartController::setFont);

    // ... (rest of connections)
    // Conectar botones de dibujo para mostrar popup al hacer clic (incluso si ya
    // están seleccionados)
    connect(ui->toolPoint, &QPushButton::clicked, this,
            [this]() { showStrokeSettings(ui->toolPoint); });
    connect(ui->toolLine, &QPushButton::clicked, this,
            [this]() { showStrokeSettings(ui->toolLine); });
    connect(ui->toolArc, &QPushButton::clicked, this,
            [this]() { showStrokeSettings(ui->toolArc); });
    connect(ui->toolText, &QPushButton::clicked, this,
            [this]() { showStrokeSettings(ui->toolText); });

    // === SELECCIÓN (NUEVO) ===
    connect(m_chartWidget->scene(), &QGraphicsScene::selectionChanged, this, &MainWindow::onSelectionChanged);
    connect(m_strokePopup, &StrokeSettingsPopup::toggleProjectionsRequested,
            this, &MainWindow::onToggleProjections);
    
    // === CONEXIÓN RADIO COMPÁS ===
    connect(m_strokePopup, &StrokeSettingsPopup::radiusChanged, this, [this](double radius) {
        m_chartWidget->controller()->setCompassRadius(radius);
    });

    // === HACER LA TOOLBAR FLOTANTE ===
    // Sacar la toolbar del layout y posicionarla como overlay
    ui->toolbarScrollArea->setParent(ui->dashboardPage);
    ui->toolbarScrollArea->raise();

    // Posicionar la toolbar flotante
    ui->toolbarScrollArea->move(16, 8);

    // Aplicar estilo flotante con sombra sutil
    ui->toolbarScrollArea->setStyleSheet(R"(
        QScrollArea {
            background-color: #0d0d0d;
            border: none;
            border-radius: 16px;
        }
        QWidget#toolbarContent {
            background-color: #0d0d0d;
            border-radius: 16px;
        }
    )");
}

void MainWindow::onAngleChanged(double angle) {
    m_angleLabel->setText(QString("%1°").arg(angle, 0, 'f', 1));
    m_angleLabel->setVisible(true);
}

void MainWindow::updateToolButtonStates() {
    // Actualizar visualmente qué botón está seleccionado
    ToolMode current = m_chartWidget->controller()->currentTool();
    int currentId = static_cast<int>(current);

    // Caso especial: Select comparte botón con Pan (ID Pan)
    if (current == ToolMode::Select) {
        currentId = static_cast<int>(ToolMode::Pan);
    }

    QAbstractButton *button = m_toolGroup->button(currentId);
    if (button) {
        button->setChecked(true);
    }
}

// ... (setupStatusBar, setupConnections, etc.)



void MainWindow::setupStatusBar() {
    // Etiqueta de coordenadas
    m_coordLabel = new QLabel("--°--'--\"N --°--'--\"W", this);
    m_coordLabel->setStyleSheet("color: #e0e0e0; padding: 0 15px;");

    // Etiqueta de ángulo (transportador)
    m_angleLabel = new QLabel("", this);
    m_angleLabel->setStyleSheet("color: #27ae60; padding: 0 15px;");
    m_angleLabel->setVisible(false);

    // Etiqueta de zoom
    m_zoomLabel = new QLabel("100%", this);
    m_zoomLabel->setStyleSheet(
        "color: #3498db; font-weight: bold; padding: 0 15px;");

    ui->statusbar->addWidget(m_coordLabel, 1);
    ui->statusbar->addWidget(m_angleLabel);
    ui->statusbar->addPermanentWidget(m_zoomLabel);
}

void MainWindow::setupConnections() {
    // === Login UI ===
    connect(ui->loginButton, &QPushButton::clicked, this, [this]() {
        QString nick = ui->nickEdit->text();
        QString password = ui->passwordEdit->text();
        m_loginController->login(nick, password);
    });
    connect(ui->goToRegisterButton, &QPushButton::clicked, this,
            &MainWindow::onShowRegister);

    // === Login Controller ===
    connect(m_loginController, &LoginController::loginSuccessful, this,
            &MainWindow::onLoginSuccessful);
    connect(m_loginController, &LoginController::loginFailed, this,
            &MainWindow::onLoginFailed);

    // === Register UI ===
    connect(ui->registerButton, &QPushButton::clicked, this, [this]() {
        QString nick = ui->registerNickEdit->text();
        QString email = ui->registerEmailEdit->text();
        QString password = ui->registerPasswordEdit->text();
        QString confirmPassword = ui->registerConfirmPasswordEdit->text();
        QDate birthdate = ui->registerBirthdateEdit->date();

        // Validación básica
        if (password != confirmPassword) {
            ui->registerErrorLabel->setText("Las contraseñas no coinciden");
            return;
        }

        // Recuperar avatar desde property o usar null
        QImage avatar;
        QLabel *preview = ui->registerPage->findChild<QLabel*>("avatarPreviewLabel");
        if (preview) {
             QVariant var = preview->property("avatarImage");
             if (var.isValid()) {
                 avatar = var.value<QImage>();
             }
        }
        
        m_registerController->registerUser(nick, email, password, birthdate, avatar);
    });
    connect(ui->backToLoginButton, &QPushButton::clicked, this,
            &MainWindow::onBackToLogin);

    // === Register Controller ===
    connect(m_registerController, &RegisterController::registrationSuccessful,
            this, &MainWindow::onRegistrationSuccessful);
    connect(m_registerController, &RegisterController::registrationFailed, this,
            &MainWindow::onRegistrationFailed);

    // === Chart Controller ===
    connect(m_chartWidget->controller(), &ChartController::coordinatesUpdated,
            this, &MainWindow::onCoordinatesUpdated);
    connect(m_chartWidget->controller(), &ChartController::zoomChanged, this,
            &MainWindow::onZoomChanged);
    connect(m_chartWidget->controller(), &ChartController::toolChanged, this,
            [this](ToolMode mode) { onToolChanged(static_cast<int>(mode)); });
    connect(m_chartWidget->controller(), &ChartController::angleChanged, this,
            &MainWindow::onAngleChanged);

    // === Tool Group ===
    connect(m_toolGroup, &QButtonGroup::idClicked, this, [this](int id) {
        m_chartWidget->controller()->setTool(static_cast<ToolMode>(id));
    });

    // === Toolbar Buttons (no toggle) ===
    connect(ui->toolZoomIn, &QPushButton::clicked, this, &MainWindow::onZoomIn);
    connect(ui->toolZoomOut, &QPushButton::clicked, this, &MainWindow::onZoomOut);
    connect(ui->toolUndo, &QPushButton::clicked, this, &MainWindow::onUndo);
    connect(ui->toolClear, &QPushButton::clicked, this, &MainWindow::onClearAll);
    connect(ui->toolQuiz, &QPushButton::clicked, this, &MainWindow::onStartQuiz);

    // === Menu Actions ===
    connect(ui->actionSalir, &QAction::triggered, this, &QMainWindow::close);
    connect(ui->actionCerrarSesion, &QAction::triggered, this,
            &MainWindow::onLogout);
    connect(ui->actionDeshacer, &QAction::triggered, this, &MainWindow::onUndo);
    connect(ui->actionLimpiar, &QAction::triggered, this,
            &MainWindow::onClearAll);
    connect(ui->actionZoomIn, &QAction::triggered, this, &MainWindow::onZoomIn);
    connect(ui->actionZoomOut, &QAction::triggered, this, &MainWindow::onZoomOut);
    connect(ui->actionZoomReset, &QAction::triggered, this,
            &MainWindow::onZoomReset);
    connect(ui->actionEstadisticas, &QAction::triggered, this,
            &MainWindow::onShowStats);
    connect(ui->actionPerfil, &QAction::triggered, this,
            &MainWindow::onShowProfile);
    connect(ui->actionManual, &QAction::triggered, this,
            &MainWindow::onShowManual);
    connect(ui->actionAcercaDe, &QAction::triggered, this, &MainWindow::onAbout);
    connect(ui->actionAcercaDe, &QAction::triggered, this, &MainWindow::onAbout);

    // === NEW VIEWS CONNECTIONS ===
    connect(m_profileView, &ProfileView::backRequested, this, &MainWindow::showDashboard);
    connect(m_resultsView, &ResultsView::backRequested, this, &MainWindow::showDashboard);
    
    // Register Avatar Button
    QPushButton *btn = ui->registerPage->findChild<QPushButton*>("selectAvatarButton");
    if (btn) {
       connect(btn, &QPushButton::clicked, this, [this]() {
           QString fileName = QFileDialog::getOpenFileName(this, "Seleccionar Avatar", "", "Images (*.png *.jpg *.jpeg)");
           if (!fileName.isEmpty()) {
               QImage img(fileName);
               if (!img.isNull()) {
                   // Scale and show
                    QLabel *preview = ui->registerPage->findChild<QLabel*>("avatarPreviewLabel");
                    if (preview) {
                        preview->setPixmap(QPixmap::fromImage(img).scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                        // Store it in a property or member to access it in register?
                        // Actually RegisterController needs it. 
                        // For now we set it as a property on the preview label to retrieve it later?
                        // Better: adding a member `m_tempRegisterAvatar` in MainWindow if needed, or 
                        // retrieve from pixmap (lossy).
                        // Let's use property.
                        preview->setProperty("avatarImage", img);
                    }
               }
           }
       });
    }

    // Register Password Toggle
    QAction *togglePass = ui->registerPasswordEdit->addAction(QIcon(":/resources/icons/eye.svg"), QLineEdit::TrailingPosition);
    connect(togglePass, &QAction::triggered, this, [this]() {
        if (ui->registerPasswordEdit->echoMode() == QLineEdit::Password) {
            ui->registerPasswordEdit->setEchoMode(QLineEdit::Normal);
        } else {
            ui->registerPasswordEdit->setEchoMode(QLineEdit::Password);
        }
    });

}

void MainWindow::setupRegisterValidation() {
    auto validateField = [](QLineEdit *edit, QLabel *errorLabel, std::function<ValidationResult(QString)> validator) {
        QString text = edit->text();
        ValidationResult result = validator(text);
        if (result.valid) {
            edit->setStyleSheet("border: 2px solid #27ae60; background-color: #2a2a2a; color: white; padding: 10px; border-radius: 8px;"); // Green
            errorLabel->setText("");
        } else {
            edit->setStyleSheet("border: 2px solid #e74c3c; background-color: #2a2a2a; color: white; padding: 10px; border-radius: 8px;"); // Red
            errorLabel->setText(result.errorMessage);
        }
    };

    connect(ui->registerNickEdit, &QLineEdit::textChanged, this, [=]() {
        validateField(ui->registerNickEdit, ui->registerErrorLabel, [](QString s){ return Validators::validateNick(s); });
    });

    connect(ui->registerEmailEdit, &QLineEdit::textChanged, this, [=]() {
        validateField(ui->registerEmailEdit, ui->registerErrorLabel, [](QString s){ return Validators::validateEmail(s); });
    });

    connect(ui->registerPasswordEdit, &QLineEdit::textChanged, this, [=]() {
        validateField(ui->registerPasswordEdit, ui->registerErrorLabel, [](QString s){ return Validators::validatePassword(s); });
    });
}

// === NAVIGATION ===

void MainWindow::showLoginPage() {
    ui->stackedWidget->setCurrentIndex(0);
    ui->menuUsuario->setEnabled(false);
}

void MainWindow::showRegisterPage() { ui->stackedWidget->setCurrentIndex(1); }

void MainWindow::showDashboard() {
    ui->stackedWidget->setCurrentIndex(2);
    ui->menuUsuario->setEnabled(true);
    loadChart();
}

void MainWindow::loadChart() {
    // Cargar la carta náutica desde recursos
    QString chartPath = ":/resources/carta_nautica.jpg";
    m_chartWidget->controller()->loadChart(chartPath);

    // Calibración aproximada del Estrecho de Gibraltar
    // TODO: Reemplazar con puntos de calibración reales
    // Esquina superior izquierda: Aprox. 36°20'N 6°00'W
    // Esquina inferior derecha: Aprox. 35°40'N 5°10'W
}

// === LOGIN SLOTS ===

void MainWindow::onLoginClicked() { showDashboard(); }

void MainWindow::onLoginSuccessful(User *user) {
    Q_UNUSED(user)
    showDashboard();
}

void MainWindow::onLoginFailed(const QString &reason) {
    ui->loginErrorLabel->setText(reason);
}

void MainWindow::onShowRegister() { showRegisterPage(); }

// === REGISTER SLOTS ===

void MainWindow::onRegisterClicked() {}

void MainWindow::onRegistrationSuccessful() {
    QMessageBox::information(
        this, "Registro Exitoso",
        "Tu cuenta ha sido creada. Ya puedes iniciar sesión.");
    // Limpiar campos del formulario de registro
    ui->registerNickEdit->clear();
    ui->registerEmailEdit->clear();
    ui->registerPasswordEdit->clear();
    ui->registerConfirmPasswordEdit->clear();
    ui->registerErrorLabel->clear();
    showLoginPage();
}

void MainWindow::onRegistrationFailed(const QString &reason) {
    ui->registerErrorLabel->setText(reason);
}

void MainWindow::onBackToLogin() { showLoginPage(); }

// === SESSION SLOTS ===

void MainWindow::onLogout() {
    m_loginController->logout();
    m_sessionController->endSession(); // Ensure session is saved
    showLoginPage();
}

// === TOOL SLOTS ===

void MainWindow::onToolPanClicked() {
    m_chartWidget->controller()->setTool(ToolMode::Pan);
}

void MainWindow::onZoomIn() { m_chartWidget->controller()->zoomIn(); }

void MainWindow::onZoomOut() { m_chartWidget->controller()->zoomOut(); }

void MainWindow::onZoomReset() { m_chartWidget->controller()->resetZoom(); }

void MainWindow::onToolPointClicked() {
    m_chartWidget->controller()->setTool(ToolMode::Point);
}

void MainWindow::onToolLineClicked() {
    m_chartWidget->controller()->setTool(ToolMode::Line);
}

void MainWindow::onToolArcClicked() {
    m_chartWidget->controller()->setTool(ToolMode::Arc);
}

void MainWindow::onToolTextClicked() {
    m_chartWidget->controller()->setTool(ToolMode::Text);
}

void MainWindow::onToolEraserClicked() {
    m_chartWidget->controller()->setTool(ToolMode::Eraser);
}

void MainWindow::onToolProtractorClicked() {
    m_chartWidget->controller()->setTool(ToolMode::Protractor);
}

void MainWindow::onToolRulerClicked() {
    m_chartWidget->controller()->setTool(ToolMode::Ruler);
}

void MainWindow::onUndo() { m_chartWidget->controller()->undoLastAction(); }

void MainWindow::onClearAll() {
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirmar",
        "¿Estás seguro de que quieres borrar todos los trazos?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_chartWidget->controller()->clearAllDrawings();
    }
}

void MainWindow::onStartQuiz() {
    if (!m_loginController->isLoggedIn()) {
         QMessageBox::warning(this, "Acceso denegado", "Debes iniciar sesión para realizar problemas.");
         return;
    }
    
    User *user = m_loginController->currentUser();
    m_quizView->startQuiz(user);
    m_quizView->exec(); // Show as modal dialog
}

// === CHART SLOTS ===

void MainWindow::onCoordinatesUpdated(double lat, double lon) {
    GeoCoord coord{lat, lon};
    m_coordLabel->setText(coord.toDMS());
}

void MainWindow::onZoomChanged(double factor) {
    m_zoomLabel->setText(QString("%1%").arg(static_cast<int>(factor * 100)));
}

void MainWindow::onToolChanged(int toolMode) {
    ToolMode mode = static_cast<ToolMode>(toolMode);

    QString toolName;
    QWidget *anchor = nullptr;
    bool isDrawingTool = false;

    switch (mode) {
    case ToolMode::Pan:
        toolName = "Moverse";
        break;
    case ToolMode::Point:
        toolName = "Punto";
        anchor = ui->toolPoint;
        isDrawingTool = true;
        break;
    case ToolMode::Line:
        toolName = "Línea";
        anchor = ui->toolLine;
        isDrawingTool = true;
        break;
    case ToolMode::Arc:
        toolName = "Arco";
        anchor = ui->toolArc;
        isDrawingTool = true;
        break;
    case ToolMode::Text:
        toolName = "Texto";
        anchor = ui->toolText;
        isDrawingTool = true;
        break;
    case ToolMode::Eraser:
        toolName = "Borrador";
        break;
    case ToolMode::Protractor:
        toolName = "Transportador";
        break;
    case ToolMode::Ruler:
        toolName = "Regla";
        break;
    default:
        toolName = "Selecciona";
        break;
    }

    // Mostrar/ocultar ángulo según herramienta
    m_angleLabel->setVisible(mode == ToolMode::Protractor);

    // Configurar cursor según herramienta
    QCursor cursor = Qt::ArrowCursor;
    bool showPopup = isDrawingTool;

    switch (mode) {
    case ToolMode::Pan:
        cursor = Qt::OpenHandCursor;
        break;
    case ToolMode::Point:
    case ToolMode::Line:
    case ToolMode::Arc:
        cursor = Qt::CrossCursor;
        break;
    case ToolMode::Text:
        cursor = Qt::IBeamCursor;
        break;
    case ToolMode::Eraser:
        cursor = Qt::PointingHandCursor; // O un icono de goma si tuviéramos
        break;
    default:
        cursor = Qt::ArrowCursor;
        break;
    }
    m_chartWidget->setCursor(cursor);

    // Actualizar indicador de herramienta en el ChartWidget (mostrar para todas)
    QColor color = m_chartWidget->controller()->strokeColor();
    int width = m_chartWidget->controller()->strokeWidth();
    m_chartWidget->updateToolIndicator(toolName, color, width, isDrawingTool);

    // Configurar popup de trazo (modo texto vs normal)
    if (m_strokePopup) {
        m_strokePopup->setTextMode(mode == ToolMode::Text);
        m_strokePopup->setPointMode(mode == ToolMode::Point);
        m_strokePopup->setArcMode(mode == ToolMode::Arc);

        if (showPopup && anchor) {
            showStrokeSettings(anchor);
        } else {
            m_strokePopup->hide();
        }
    }

    updateToolButtonStates();
}



// === MENU SLOTS ===

void MainWindow::onShowStats() {
    if (m_sessionController && m_loginController->currentUser()) {
        ui->stackedWidget->setCurrentWidget(m_resultsView);
        m_resultsView->loadData(m_loginController->currentUser());
    }
}

void MainWindow::onShowProfile() {
    if (m_loginController->currentUser()) {
        ui->stackedWidget->setCurrentWidget(m_profileView);
        m_profileView->loadUser(m_loginController->currentUser());
    }
}

void MainWindow::onShowManual() {
    // Abrir manual de ayuda
    QMessageBox::information(
        this, "Manual de Uso",
        "<h3>Carta Náutica Digital</h3>"
        "<p><b>Navegación:</b></p>"
        "<ul>"
        "<li>✋ <b>Mano:</b> Arrastra para mover la carta</li>"
        "<li>🔍 <b>Zoom:</b> Usa la rueda del ratón o los botones +/-</li>"
        "</ul>"
        "<p><b>Dibujo:</b></p>"
        "<ul>"
        "<li>📍 <b>Punto:</b> Click para marcar posiciones</li>"
        "<li>📐 <b>Línea:</b> Arrastra para trazar rumbos</li>"
        "<li>◠ <b>Arco:</b> Arrastra desde el centro para crear arcos</li>"
        "<li>T <b>Texto:</b> Click para añadir anotaciones</li>"
        "<li>🧽 <b>Borrador:</b> Click sobre un elemento para eliminarlo</li>"
        "</ul>"
        "<p><b>Medición:</b></p>"
        "<ul>"
        "<li>🧭 <b>Transportador:</b> Arrastra para mover, rueda para rotar</li>"
        "<li>📏 <b>Regla:</b> Arrastra para medir distancias</li>"
        "</ul>"
        "<p><b>Atajos:</b> Ctrl+Z (Deshacer), +/- (Zoom), Esc (Cancelar)</p>");
}

void MainWindow::onAbout() {
    QMessageBox::about(
        this, "Acerca de",
        "<h2>Patrón de Embarcación de Recreo</h2>"
        "<p>Aplicación de preparación para el examen de navegación.</p>"
        "<p><b>Características:</b></p>"
        "<ul>"
        "<li>Carta Náutica Digital interactiva</li>"
        "<li>Herramientas de dibujo y medición</li>"
        "<li>Sistema de coordenadas geográficas</li>"
        "<li>Quiz de preguntas tipo test</li>"
        "</ul>"
        "<p><b>Curso 2025-2026 - UPV</b></p>"
        "<p>Práctica de Interacción Humano-Máquina</p>");
}

void MainWindow::onToggleProjections() {
    m_chartWidget->controller()->toggleProjectionsForSelected();
}

// === STROKE SETTINGS ===

void MainWindow::showStrokeSettings(QWidget *anchor) {
    if (m_strokePopup) {
        m_strokePopup->showNear(anchor);
    }
}

void MainWindow::onStrokeColorChanged(const QColor &color) {
    m_chartWidget->controller()->setStrokeColor(color);
    // Actualizar indicador
    int width = m_chartWidget->controller()->strokeWidth();
    m_chartWidget->updateToolIndicator("", color, width, true);
}

void MainWindow::onStrokeWidthChanged(int width) {
    m_chartWidget->controller()->setStrokeWidth(width);
    // Actualizar indicador
    QColor color = m_chartWidget->controller()->strokeColor();
    m_chartWidget->updateToolIndicator("", color, width, true);
}

void MainWindow::onSelectionChanged() {
    // Actualizar UI del popup si estamos en modo selección o si simplemente hay una selección activa
    QList<QGraphicsItem*> selected = m_chartWidget->scene()->selectedItems();
    if (selected.isEmpty()) {
        m_strokePopup->hide(); 
        return;
    }

    // Si estamos en modo selección, mostrar popup automáticamente
    if (m_chartWidget->controller()->currentTool() == ToolMode::Select) {
        QGraphicsItem *item = selected.first();
        
        // Configurar popup según el item seleccionado
        bool isText = false;
        bool isPoint = false;
        
        QColor color = Qt::black;
        int width = 1;
        
        if (auto *line = qgraphicsitem_cast<QGraphicsLineItem*>(item)) {
            color = line->pen().color();
            width = line->pen().width();
        } else if (auto *path = qgraphicsitem_cast<QGraphicsPathItem*>(item)) {
            color = path->pen().color();
            width = path->pen().width();
        } else if (auto *ellipse = qgraphicsitem_cast<QGraphicsEllipseItem*>(item)) {
            color = ellipse->pen().color();
            width = ellipse->pen().width();
            isPoint = true;
        } else if (auto *text = qgraphicsitem_cast<QGraphicsTextItem*>(item)) {
            color = text->defaultTextColor();
            width = text->font().pixelSize(); // Usamos pixelSize como grosor/tamaño
            isText = true;
        }
        
        // Actualizar popup sin emitir señales
        m_strokePopup->blockSignals(true);
        m_strokePopup->setColor(color);
        m_strokePopup->setStrokeWidth(width);
        m_strokePopup->setTextMode(isText);
        m_strokePopup->setPointMode(isPoint);
        m_strokePopup->blockSignals(false);
        
        // Mostrar popup cerca del mouse
        QPoint globalPos = QCursor::pos();
        m_strokePopup->move(globalPos + QPoint(20, 20));
        m_strokePopup->show();
        m_strokePopup->raise();
    }
}




