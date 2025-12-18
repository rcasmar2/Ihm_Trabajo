#pragma once

#include <QObject>
#include <QImage>
#include <QDate>
#include "navtypes.h"
#include "validators.h"

class ProfileController : public QObject
{
    Q_OBJECT
public:
    explicit ProfileController(QObject *parent = nullptr);

    /**
     * @brief processUpdate
     * Validates and updates user profile information.
     * Nickname is READ-ONLY and used to identify the user.
     */
    void updateProfile(const QString &nick,
                       const QString &email,
                       const QString &newPassword, // Empty if not changing
                       const QDate &birthdate,
                       const QImage &avatar);

signals:
    void updateSuccessful(const QString &message);
    void updateFailed(const QString &message);
    void validationError(const QString &field, const QString &message);

private:
    ValidationResult validateEmail(const QString &email) const;
    ValidationResult validatePassword(const QString &password) const;
    ValidationResult validateBirthdate(const QDate &birthdate) const;
};
