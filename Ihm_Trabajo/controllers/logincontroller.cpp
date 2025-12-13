#include "logincontroller.h"
#include "navigation.h"

LoginController::LoginController(QObject *parent)
    : QObject(parent)
    , m_currentUser(nullptr)
{
}

bool LoginController::login(const QString &nick, const QString &password) {
    if (nick.isEmpty() || password.isEmpty()) {
        emit loginFailed("Nick y contraseña son obligatorios");
        return false;
    }

    User *user = Navigation::instance().authenticate(nick, password);

    if (user) {
        m_currentUser = user;
        emit loginSuccessful(user);
        return true;
    } else {
        emit loginFailed("Nick o contraseña incorrectos");
        return false;
    }
}

void LoginController::logout() {
    m_currentUser = nullptr;
    emit loggedOut();
}
