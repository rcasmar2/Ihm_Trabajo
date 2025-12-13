#pragma once

#include <QObject>

class User;

/**
 * Controlador de autenticación de usuarios
 */
class LoginController : public QObject {
    Q_OBJECT

public:
    explicit LoginController(QObject *parent = nullptr);

    /**
     * Intenta autenticar al usuario con nick y password
     * @return true si la autenticación es exitosa
     */
    bool login(const QString &nick, const QString &password);

    /**
     * Cierra la sesión del usuario actual
     */
    void logout();

    /**
     * @return puntero al usuario autenticado, nullptr si no hay sesión
     */
    User* currentUser() const { return m_currentUser; }

    /**
     * @return true si hay un usuario autenticado
     */
    bool isLoggedIn() const { return m_currentUser != nullptr; }

signals:
    /**
     * Emitida cuando el login es exitoso
     * @param user puntero al usuario autenticado
     */
    void loginSuccessful(User *user);

    /**
     * Emitida cuando el login falla
     * @param reason mensaje de error
     */
    void loginFailed(const QString &reason);

    /**
     * Emitida cuando el usuario cierra sesión
     */
    void loggedOut();

private:
    User *m_currentUser = nullptr;
};
