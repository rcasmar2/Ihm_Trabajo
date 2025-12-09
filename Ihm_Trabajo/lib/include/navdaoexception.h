#pragma once
#include <stdexcept>
#include <QString>

class NavDAOException : public std::runtime_error {
public:
    explicit NavDAOException(const QString &msg)
        : std::runtime_error(msg.toStdString()) {}
};
