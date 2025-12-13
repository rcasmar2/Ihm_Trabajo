#include "charttypes.h"
#include "validators.h"

ValidationResult Validators::validateNick(const QString &nick) {
    if (nick.length() < 6) {
        return ValidationResult::failure("El nick debe tener al menos 6 caracteres");
    }
    if (nick.length() > 15) {
        return ValidationResult::failure("El nick no puede tener más de 15 caracteres");
    }
    // Solo caracteres alfanuméricos y guión bajo
    static QRegularExpression validChars("^[a-zA-Z0-9_]+$");
    if (!validChars.match(nick).hasMatch()) {
        return ValidationResult::failure("El nick solo puede contener letras, números y guión bajo");
    }
    return ValidationResult::success();
}

ValidationResult Validators::validatePassword(const QString &password) {
    if (password.length() < 8) {
        return ValidationResult::failure("La contraseña debe tener al menos 8 caracteres");
    }
    if (password.length() > 20) {
        return ValidationResult::failure("La contraseña no puede tener más de 20 caracteres");
    }

    static QRegularExpression hasUpper("[A-Z]");
    static QRegularExpression hasLower("[a-z]");
    static QRegularExpression hasDigit("[0-9]");
    static QRegularExpression hasSpecial("[!@#$%^&*()_+\\-=\\[\\]{};':\"\\\\|,.<>\\/?]");

    if (!hasUpper.match(password).hasMatch()) {
        return ValidationResult::failure("La contraseña debe contener al menos una mayúscula");
    }
    if (!hasLower.match(password).hasMatch()) {
        return ValidationResult::failure("La contraseña debe contener al menos una minúscula");
    }
    if (!hasDigit.match(password).hasMatch()) {
        return ValidationResult::failure("La contraseña debe contener al menos un dígito");
    }
    if (!hasSpecial.match(password).hasMatch()) {
        return ValidationResult::failure("La contraseña debe contener al menos un carácter especial");
    }

    return ValidationResult::success();
}

ValidationResult Validators::validateBirthdate(const QDate &birthdate) {
    if (!birthdate.isValid()) {
        return ValidationResult::failure("Fecha de nacimiento no válida");
    }

    QDate today = QDate::currentDate();
    int age = today.year() - birthdate.year();
    if (today.month() < birthdate.month() ||
        (today.month() == birthdate.month() && today.day() < birthdate.day())) {
        age--;
    }

    if (age < 16) {
        return ValidationResult::failure("Debes tener al menos 16 años");
    }

    return ValidationResult::success();
}

ValidationResult Validators::validateEmail(const QString &email) {
    if (email.isEmpty()) {
        return ValidationResult::failure("El email es obligatorio");
    }

    static QRegularExpression emailRegex(
        R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)"
    );

    if (!emailRegex.match(email).hasMatch()) {
        return ValidationResult::failure("Formato de email no válido");
    }

    return ValidationResult::success();
}
