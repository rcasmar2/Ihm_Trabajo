#pragma once

#include <QString>
#include <QDate>
#include <QRegularExpression>

struct ValidationResult;

/**
 * Clase de utilidad para validaciones de registro de usuario
 */
class Validators {
public:
    /**
     * Valida el nick de usuario
     * Requisitos: 6-15 caracteres, único
     */
    static ValidationResult validateNick(const QString &nick);

    /**
     * Valida la contraseña
     * Requisitos: 8-20 caracteres, mayúscula, minúscula, dígito, carácter especial
     */
    static ValidationResult validatePassword(const QString &password);

    /**
     * Valida la edad (>= 16 años)
     */
    static ValidationResult validateBirthdate(const QDate &birthdate);

    /**
     * Valida formato de email
     */
    static ValidationResult validateEmail(const QString &email);
};
