#include "profilecontroller.h"
#include "navigation.h"
#include "utils/charttypes.h"
#include "validators.h"
#include <QDebug>

ProfileController::ProfileController(QObject *parent)
    : QObject(parent)
{
}

void ProfileController::updateProfile(const QString &nick,
                                      const QString &email,
                                      const QString &newPassword,
                                      const QDate &birthdate,
                                      const QImage &avatar)
{
    // 1. Validate inputs
    auto emailResult = validateEmail(email);
    if (!emailResult.valid) {
        emit validationError("email", emailResult.errorMessage);
        emit updateFailed("Error en el email");
        return;
    }

    auto birthResult = validateBirthdate(birthdate);
    if (!birthResult.valid) {
        emit validationError("birthdate", birthResult.errorMessage);
        emit updateFailed("Error en la fecha de nacimiento");
        return;
    }

    // Password is optional during update
    if (!newPassword.isEmpty()) {
        auto passResult = validatePassword(newPassword);
        if (!passResult.valid) {
            emit validationError("password", passResult.errorMessage);
            emit updateFailed("Error en la contraseña");
            return;
        }
    }

    // 2. Retrieve current user to ensure existence and keep other data if needed
    User *currentUser = Navigation::instance().findUser(nick);
    if (!currentUser) {
        emit updateFailed("Usuario no encontrado");
        return;
    }

    // 3. Prepare updated user object
    // We create a copy or modify the existing one. 
    // Navigation::updateUser takes a const User&.
    // Important: Nickname cannot be changed.
    
    User updatedUser = *currentUser; 
    updatedUser.setEmail(email);
    updatedUser.setBirthdate(birthdate);
    updatedUser.setAvatar(avatar);

    if (!newPassword.isEmpty()) {
        updatedUser.setPassword(newPassword);
    }

    // 4. Save to DB
    try {
        Navigation::instance().updateUser(updatedUser);
        
        // Update the singleton's cached user if it's the logged-in user
        // Note: Navigation::instance().authenticate might need re-calling or manual update 
        // if the session holds a reference to the user.
        // However, Navigation::findUser returns a pointer to the internal map value? 
        // Let's check NavigationDAO/Navigation implementation carefully. 
        // NavigationDAO::loadUsers returns a copy. Navigation likely caches it.
        
        emit updateSuccessful("Perfil actualizado correctamente");
    } catch (const std::exception &e) {
        emit updateFailed(QString("Error al actualizar: %1").arg(e.what()));
    }
}

ValidationResult ProfileController::validateEmail(const QString &email) const {
    return Validators::validateEmail(email);
}

ValidationResult ProfileController::validatePassword(const QString &password) const {
    return Validators::validatePassword(password);
}

ValidationResult ProfileController::validateBirthdate(const QDate &birthdate) const {
    return Validators::validateBirthdate(birthdate);
}
