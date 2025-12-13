#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "controllers/logincontroller.h"
#include "controllers/registercontroller.h"
#include "controllers/sessioncontroller.h"
#include "controllers/chartcontroller.h"
#include "widgets/chartwidget.h"
#include "widgets/strokesettingspopup.h"
#include "utils/charttypes.h"
#include "navigation.h"

#include <QLabel>
#include <QButtonGroup>
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_loginController(nullptr)
    , m_registerController(nullptr)
    , m_sessionController(nullptr)
    , m_chartWidget(nullptr)
    , m_coordLabel(nullptr)
    , m_zoomLabel(nullptr)
    , m_angleLabel(nullptr)
    , m_toolLabel(nullptr)
    , m_toolGroup(nullptr)
    , m_strokePopup(nullptr)
{
    ui->setupUi(this);

    setupControllers();
    setupChartWidget();
    setupToolbar();
    setupStatusBar();
    setupConnections();

    // Iniciar mostrando el dashboard directamente para debug
    // En producción: showLoginPage()
    showDashboard();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// === SETUP ===

void MainWindow::setupControllers() {
    m_loginController = new LoginController(this);
    m_registerController = new RegisterController(this);
    m_sessionController = new SessionController(this);
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
    m_toolGroup->addButton(ui->toolProtractor, static_cast<int>(ToolMode::Protractor));
    m_toolGroup->addButton(ui->toolRuler, static_cast<int>(ToolMode::Ruler));

    // Seleccionar Pan por defecto
    ui->toolPan->setChecked(true);
    
    // Crear popup de configuración de trazo
    m_strokePopup = new StrokeSettingsPopup(this);
    m_strokePopup->setColor(m_chartWidget->controller()->strokeColor());
    m_strokePopup->setStrokeWidth(m_chartWidget->controller()->strokeWidth());
    
    // Conectar cambios del popup al controller
    connect(m_strokePopup, &StrokeSettingsPopup::colorChanged,
            this, &MainWindow::onStrokeColorChanged);
    connect(m_strokePopup, &StrokeSettingsPopup::strokeWidthChanged,
            this, &MainWindow::onStrokeWidthChanged);
}

void MainWindow::setupStatusBar() {
    // Etiqueta de herramienta actual
    m_toolLabel = new QLabel("🔧 Mano", this);
    m_toolLabel->setStyleSheet("color: #e94560; font-weight: bold; padding: 0 15px;");
    
    // Etiqueta de coordenadas
    m_coordLabel = new QLabel("📍 --°--'--\"N --°--'--\"W", this);
    m_coordLabel->setStyleSheet("color: #e0e0e0; padding: 0 15px;");
    
    // Etiqueta de ángulo (transportador)
    m_angleLabel = new QLabel("", this);
    m_angleLabel->setStyleSheet("color: #27ae60; padding: 0 15px;");
    m_angleLabel->setVisible(false);
    
    // Etiqueta de zoom
    m_zoomLabel = new QLabel("🔍 100%", this);
    m_zoomLabel->setStyleSheet("color: #3498db; font-weight: bold; padding: 0 15px;");

    ui->statusbar->addWidget(m_toolLabel);
    ui->statusbar->addWidget(m_coordLabel, 1);
    ui->statusbar->addWidget(m_angleLabel);
    ui->statusbar->addPermanentWidget(m_zoomLabel);
}

void MainWindow::setupConnections() {
    // === Login Controller ===
    connect(m_loginController, &LoginController::loginSuccessful,
            this, &MainWindow::onLoginSuccessful);
    connect(m_loginController, &LoginController::loginFailed,
            this, &MainWindow::onLoginFailed);

    // === Register Controller ===
    connect(m_registerController, &RegisterController::registrationSuccessful,
            this, &MainWindow::onRegistrationSuccessful);
    connect(m_registerController, &RegisterController::registrationFailed,
            this, &MainWindow::onRegistrationFailed);

    // === Chart Controller ===
    connect(m_chartWidget->controller(), &ChartController::coordinatesUpdated,
            this, &MainWindow::onCoordinatesUpdated);
    connect(m_chartWidget->controller(), &ChartController::zoomChanged,
            this, &MainWindow::onZoomChanged);
    connect(m_chartWidget->controller(), &ChartController::toolChanged,
            this, [this](ToolMode mode) { onToolChanged(static_cast<int>(mode)); });
    connect(m_chartWidget->controller(), &ChartController::angleChanged,
            this, &MainWindow::onAngleChanged);

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
    connect(ui->actionCerrarSesion, &QAction::triggered, this, &MainWindow::onLogout);
    connect(ui->actionDeshacer, &QAction::triggered, this, &MainWindow::onUndo);
    connect(ui->actionLimpiar, &QAction::triggered, this, &MainWindow::onClearAll);
    connect(ui->actionZoomIn, &QAction::triggered, this, &MainWindow::onZoomIn);
    connect(ui->actionZoomOut, &QAction::triggered, this, &MainWindow::onZoomOut);
    connect(ui->actionZoomReset, &QAction::triggered, this, &MainWindow::onZoomReset);
    connect(ui->actionEstadisticas, &QAction::triggered, this, &MainWindow::onShowStats);
    connect(ui->actionPerfil, &QAction::triggered, this, &MainWindow::onShowProfile);
    connect(ui->actionManual, &QAction::triggered, this, &MainWindow::onShowManual);
    connect(ui->actionAcercaDe, &QAction::triggered, this, &MainWindow::onAbout);
}

// === NAVIGATION ===

void MainWindow::showLoginPage() {
    ui->stackedWidget->setCurrentIndex(0);
    ui->menuUsuario->setEnabled(false);
}

void MainWindow::showRegisterPage() {
    ui->stackedWidget->setCurrentIndex(1);
}

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

void MainWindow::onLoginClicked() {
    showDashboard();
}

void MainWindow::onLoginSuccessful(User *user) {
    Q_UNUSED(user)
    showDashboard();
}

void MainWindow::onLoginFailed(const QString &reason) {
    QMessageBox::warning(this, "Error de Login", reason);
}

void MainWindow::onShowRegister() {
    showRegisterPage();
}

// === REGISTER SLOTS ===

void MainWindow::onRegisterClicked() {
}

void MainWindow::onRegistrationSuccessful() {
    QMessageBox::information(this, "Registro Exitoso",
                             "Tu cuenta ha sido creada. Ya puedes iniciar sesión.");
    showLoginPage();
}

void MainWindow::onRegistrationFailed(const QString &reason) {
    QMessageBox::warning(this, "Error de Registro", reason);
}

void MainWindow::onBackToLogin() {
    showLoginPage();
}

// === SESSION SLOTS ===

void MainWindow::onLogout() {
    m_loginController->logout();
    showLoginPage();
}

// === TOOL SLOTS ===

void MainWindow::onToolPanClicked() {
    m_chartWidget->controller()->setTool(ToolMode::Pan);
}

void MainWindow::onZoomIn() {
    m_chartWidget->controller()->zoomIn();
}

void MainWindow::onZoomOut() {
    m_chartWidget->controller()->zoomOut();
}

void MainWindow::onZoomReset() {
    m_chartWidget->controller()->resetZoom();
}

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

void MainWindow::onUndo() {
    m_chartWidget->controller()->undoLastAction();
}

void MainWindow::onClearAll() {
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirmar",
        "¿Estás seguro de que quieres borrar todos los trazos?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        m_chartWidget->controller()->clearAllDrawings();
    }
}

void MainWindow::onStartQuiz() {
    // TODO: Abrir diálogo de quiz
    QMessageBox::information(this, "Quiz de Navegación",
                             "Próximamente: Sesión de preguntas tipo test\n"
                             "para preparar el examen de Patrón de Embarcación.");
}

// === CHART SLOTS ===

void MainWindow::onCoordinatesUpdated(double lat, double lon) {
    GeoCoord coord{lat, lon};
    m_coordLabel->setText(QString("📍 %1").arg(coord.toDMS()));
}

void MainWindow::onZoomChanged(double factor) {
    m_zoomLabel->setText(QString("🔍 %1%").arg(static_cast<int>(factor * 100)));
}

void MainWindow::onToolChanged(int toolMode) {
    ToolMode mode = static_cast<ToolMode>(toolMode);
    
    QString toolName;
    QWidget *anchor = nullptr;
    bool isDrawingTool = false;
    
    switch (mode) {
    case ToolMode::Pan: 
        toolName = "🔧 Mano"; 
        break;
    case ToolMode::Point: 
        toolName = "🔧 Punto"; 
        anchor = ui->toolPoint;
        isDrawingTool = true;
        break;
    case ToolMode::Line: 
        toolName = "🔧 Línea"; 
        anchor = ui->toolLine;
        isDrawingTool = true;
        break;
    case ToolMode::Arc: 
        toolName = "🔧 Arco"; 
        anchor = ui->toolArc;
        isDrawingTool = true;
        break;
    case ToolMode::Text: 
        toolName = "🔧 Texto"; 
        anchor = ui->toolText;
        isDrawingTool = true;
        break;
    case ToolMode::Eraser: 
        toolName = "🔧 Borrador"; 
        break;
    case ToolMode::Protractor: 
        toolName = "🔧 Transportador"; 
        break;
    case ToolMode::Ruler: 
        toolName = "🔧 Regla"; 
        break;
    default: 
        toolName = "🔧 Seleccionar"; 
        break;
    }
    
    m_toolLabel->setText(toolName);
    
    // Mostrar/ocultar ángulo según herramienta
    m_angleLabel->setVisible(mode == ToolMode::Protractor);
    
    // Mostrar popup de color/grosor para herramientas de dibujo
    if (isDrawingTool && anchor) {
        showStrokeSettings(anchor);
    } else if (m_strokePopup) {
        m_strokePopup->hide();
    }
    
    updateToolButtonStates();
}

void MainWindow::onAngleChanged(double angle) {
    m_angleLabel->setText(QString("📐 %1°").arg(angle, 0, 'f', 1));
    m_angleLabel->setVisible(true);
}

void MainWindow::updateToolButtonStates() {
    // Actualizar visualmente qué botón está seleccionado
    ToolMode current = m_chartWidget->controller()->currentTool();
    int currentId = static_cast<int>(current);
    
    QAbstractButton *button = m_toolGroup->button(currentId);
    if (button) {
        button->setChecked(true);
    }
}

// === MENU SLOTS ===

void MainWindow::onShowStats() {
    // TODO: Abrir diálogo de estadísticas
    QMessageBox::information(this, "Estadísticas",
                             "Próximamente: Historial de sesiones y estadísticas.");
}

void MainWindow::onShowProfile() {
    // TODO: Abrir diálogo de perfil
    QMessageBox::information(this, "Mi Perfil",
                             "Próximamente: Editar perfil de usuario.");
}

void MainWindow::onShowManual() {
    // Abrir manual de ayuda
    QMessageBox::information(this, "Manual de Uso",
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
        "<p><b>Atajos:</b> Ctrl+Z (Deshacer), +/- (Zoom), Esc (Cancelar)</p>"
    );
}

void MainWindow::onAbout() {
    QMessageBox::about(this, "Acerca de",
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
        "<p>Práctica de Interacción Humano-Máquina</p>"
    );
}

// === STROKE SETTINGS ===

void MainWindow::onStrokeColorChanged(const QColor &color) {
    m_chartWidget->controller()->setStrokeColor(color);
}

void MainWindow::onStrokeWidthChanged(int width) {
    m_chartWidget->controller()->setStrokeWidth(width);
}

void MainWindow::showStrokeSettings(QWidget *anchor) {
    if (m_strokePopup) {
        m_strokePopup->showNear(anchor);
    }
}

