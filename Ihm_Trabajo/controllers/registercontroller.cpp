#include "registercontroller.h"
#include "navigation.h"
#include "charttypes.h"
#include "validators.h"

RegisterController::RegisterController(QObject *parent)
    : QObject(parent)
{
}

bool RegisterController::registerUser(const QString &nick,
                                       const QString &email,
                                       const QString &password,
                                       const QDate &birthdate,
                                       const QImage &avatar) {
    // Validar todos los campos
    auto nickResult = validateNick(nick);
    if (!nickResult.valid) {
        emit validationError("nick", nickResult.errorMessage);
        emit registrationFailed(nickResult.errorMessage);
        return false;
    }

    auto emailResult = validateEmail(email);
    if (!emailResult.valid) {
        emit validationError("email", emailResult.errorMessage);
        emit registrationFailed(emailResult.errorMessage);
        return false;
    }

    auto passResult = validatePassword(password);
    if (!passResult.valid) {
        emit validationError("password", passResult.errorMessage);
        emit registrationFailed(passResult.errorMessage);
        return false;
    }

    auto birthResult = validateBirthdate(birthdate);
    if (!birthResult.valid) {
        emit validationError("birthdate", birthResult.errorMessage);
        emit registrationFailed(birthResult.errorMessage);
        return false;
    }

    // Verificar que el nick no exista
    if (Navigation::instance().findUser(nick) != nullptr) {
        emit validationError("nick", "Este nick ya está en uso");
        emit registrationFailed("Este nick ya está en uso");
        return false;
    }

    // Crear el usuario
    QImage userAvatar = avatar;
    if (userAvatar.isNull()) {
        // Avatar por defecto
        userAvatar = QImage(64, 64, QImage::Format_ARGB32);
        userAvatar.fill(Qt::gray);
    }

    User newUser(nick, email, password, userAvatar, birthdate);

    try {
        Navigation::instance().addUser(newUser);
        emit registrationSuccessful();
        return true;
    } catch (const std::exception &e) {
        emit registrationFailed(QString("Error al guardar: %1").arg(e.what()));
        return false;
    }
}

ValidationResult RegisterController::validateNick(const QString &nick) const {
    return Validators::validateNick(nick);
}

ValidationResult RegisterController::validatePassword(const QString &password) const {
    return Validators::validatePassword(password);
}

ValidationResult RegisterController::validateEmail(const QString &email) const {
    return Validators::validateEmail(email);
}

ValidationResult RegisterController::validateBirthdate(const QDate &birthdate) const {
    return Validators::validateBirthdate(birthdate);
}
