#pragma once

#include <QObject>
#include <QDate>
#include <QImage>

struct ValidationResult;

/**
 * Controlador de registro de nuevos usuarios
 */
class RegisterController : public QObject {
    Q_OBJECT

public:
    explicit RegisterController(QObject *parent = nullptr);

    /**
     * Registra un nuevo usuario
     * @return true si el registro es exitoso
     */
    bool registerUser(const QString &nick,
                      const QString &email,
                      const QString &password,
                      const QDate &birthdate,
                      const QImage &avatar = QImage());

    // Métodos de validación individual
    ValidationResult validateNick(const QString &nick) const;
    ValidationResult validatePassword(const QString &password) const;
    ValidationResult validateEmail(const QString &email) const;
    ValidationResult validateBirthdate(const QDate &birthdate) const;

signals:
    /**
     * Emitida cuando el registro es exitoso
     */
    void registrationSuccessful();

    /**
     * Emitida cuando el registro falla
     * @param reason mensaje de error
     */
    void registrationFailed(const QString &reason);

    /**
     * Emitida cuando hay un error de validación en un campo específico
     */
    void validationError(const QString &field, const QString &message);
};
