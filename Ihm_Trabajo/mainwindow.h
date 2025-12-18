#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class LoginController;
class RegisterController;
class SessionController;
class ChartWidget;
class User;
class QLabel;
class QButtonGroup;
class StrokeSettingsPopup;
class QFrame;
class ProfileController;
class ProfileView;
class ResultsView;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * MainWindow - Ventana principal de la aplicación
 *
 * Gestiona:
 * - Navegación entre vistas (login, registro, dashboard)
 * - Toolbar de herramientas con ToggleGroup
 * - Menús y acciones
 * - Barra de estado con coordenadas y zoom
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Login
    void onLoginClicked();
    void onLoginSuccessful(User *user);
    void onLoginFailed(const QString &reason);
    void onShowRegister();

    // Register
    void onRegisterClicked();
    void onRegistrationSuccessful();
    void onRegistrationFailed(const QString &reason);
    void onBackToLogin();

    // Sesión
    void onLogout();

    // Herramientas de navegación
    void onToolPanClicked();
    void onZoomIn();
    void onZoomOut();
    void onZoomReset();

    // Herramientas de dibujo
    void onToolPointClicked();
    void onToolLineClicked();
    void onToolArcClicked();
    void onToolTextClicked();
    void onToolEraserClicked();

    // Herramientas de medición
    void onToolProtractorClicked();
    void onToolRulerClicked();

    // Acciones
    void onUndo();
    void onClearAll();
    void onStartQuiz();
    void onToggleProjections(); // Req 3.10

    // Carta
    void onCoordinatesUpdated(double lat, double lon);
    void onZoomChanged(double factor);
    void onToolChanged(int toolMode);
    void onAngleChanged(double angle);

    // Menú
    void onShowStats();
    void onShowProfile();
    void onShowManual();
    void onAbout();

    // Stroke Settings
    void onStrokeColorChanged(const QColor &color);
    void onStrokeWidthChanged(int width);
    void showStrokeSettings(QWidget *anchor);
    void onSelectionChanged(); // NUEVO

private:
    Ui::MainWindow *ui;

    // Controladores
    LoginController *m_loginController;
    RegisterController *m_registerController;
    SessionController *m_sessionController;
    ProfileController *m_profileController;

    // Widgets
    ChartWidget *m_chartWidget;

    // UI elements
    QLabel *m_coordLabel;
    QLabel *m_zoomLabel;
    QLabel *m_angleLabel;
    QButtonGroup *m_toolGroup;
    StrokeSettingsPopup *m_strokePopup;
    ProfileView *m_profileView;
    ResultsView *m_resultsView;

    void setupControllers();
    void setupChartWidget();
    void setupConnections();
    void setupToolbar();
    void setupStatusBar();
    void setupRegisterValidation(); // New method
    void setupViews(); // New method
    void showLoginPage();
    void showRegisterPage();
    void showDashboard();
    void loadChart();
    void updateToolButtonStates();
    void updateToolIndicator();
};

#endif // MAINWINDOW_H
