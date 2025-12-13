#pragma once

#include <QString>
#include <QPointF>
#include <QDate>
#include <QRegularExpression>

/**
 * Modos de herramienta para la carta náutica
 */
enum class ToolMode {
    Select,         // Seleccionar/mover elementos
    Pan,            // Mover la carta (mano)
    Point,          // Marcar punto
    Line,           // Trazar línea/rumbo
    Arc,            // Arco (compás)
    Text,           // Añadir texto
    Eraser,         // Borrar elemento individual
    Protractor,     // Transportador de ángulos
    Ruler           // Regla para medir distancias
};

/**
 * Coordenadas geográficas
 */
struct GeoCoord {
    double latitude = 0.0;   // Grados decimales (+ Norte, - Sur)
    double longitude = 0.0;  // Grados decimales (+ Este, - Oeste)

    QString toDMS() const {
        auto formatDMS = [](double decimal, char pos, char neg) -> QString {
            char dir = decimal >= 0 ? pos : neg;
            decimal = qAbs(decimal);
            int deg = static_cast<int>(decimal);
            double minFloat = (decimal - deg) * 60.0;
            int min = static_cast<int>(minFloat);
            double sec = (minFloat - min) * 60.0;
            return QString("%1°%2'%3\"%4")
                .arg(deg, 2, 10, QChar('0'))
                .arg(min, 2, 10, QChar('0'))
                .arg(sec, 5, 'f', 2, QChar('0'))
                .arg(dir);
        };
        return formatDMS(latitude, 'N', 'S') + " " + formatDMS(longitude, 'E', 'W');
    }
};

/**
 * Resultado de validación
 */
struct ValidationResult {
    bool valid = false;
    QString errorMessage;

    static ValidationResult success() {
        return {true, QString()};
    }

    static ValidationResult failure(const QString &msg) {
        return {false, msg};
    }
};
