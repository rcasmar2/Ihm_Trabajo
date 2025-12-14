#pragma once

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QStack>
#include <QStack>
#include <QColor>
#include <QFont> // NECESARIO
#include "charttypes.h"

// Forward declarations for SVG overlay tools
class DraggableProtractor;
class DraggableRuler;

/**
 * ChartController - Controlador principal de la carta náutica
 * 
 * Implementa una máquina de estados basada en la herramienta seleccionada
 * para gestionar zoom, pan, dibujo y herramientas de medición.
 */
class ChartController : public QObject {
    Q_OBJECT

public:
    explicit ChartController(QGraphicsScene *scene, QObject *parent = nullptr);
    ~ChartController();

    // === CARGA DE CARTA ===
    void loadChart(const QString &imagePath);
    QSizeF chartSize() const;

    // === NAVEGACIÓN ===
    void zoomIn();
    void zoomOut();
    void zoomTo(double factor);
    void resetZoom();
    double zoomLevel() const { return m_zoomLevel; }

    // === HERRAMIENTAS ===
    void setTool(ToolMode mode);
    ToolMode currentTool() const { return m_currentTool; }

    // === EVENTOS DE RATÓN (Máquina de Estados) ===
    void handleMousePressAt(const QPointF &scenePos, Qt::MouseButton button);
    void handleMouseMoveAt(const QPointF &scenePos);
    void handleMouseReleaseAt(const QPointF &scenePos, Qt::MouseButton button);
    void handleWheelDelta(int delta);

    // === GESTIÓN DE ELEMENTOS ===
    void clearAllDrawings();
    void deleteSelectedItems();
    void undoLastAction();

    // === COORDENADAS ===
    // TODO: Insertar aquí fórmulas de conversión Píxel -> Latitud/Longitud
    GeoCoord getCoordinatesAt(const QPointF &scenePos) const;
    QPointF getScenePosFromCoord(const GeoCoord &coord) const;

    // === ESTILOS ===
    void setStrokeColor(const QColor &color);
    void setStrokeWidth(int width);
    void setFont(const QFont &font); // NUEVO
    QColor strokeColor() const { return m_strokeColor; }
    int strokeWidth() const { return m_strokeWidth; }
    QFont font() const { return m_font; } // NUEVO

    // === CALIBRACIÓN ===
    void setCalibrationPoints(const QPointF &pixel1, const GeoCoord &coord1,
                               const QPointF &pixel2, const GeoCoord &coord2);

    // === HERRAMIENTAS OVERLAY (SVG) ===
    void showProtractor(bool visible);
    void showRuler(bool visible);
    void rotateProtractor(double angleDelta);
    void rotateRuler(double angleDelta);
    bool isProtractorVisible() const;
    bool isRulerVisible() const;
    double protractorAngle() const;
    double rulerAngle() const;

signals:
    void toolChanged(ToolMode newTool);
    void zoomChanged(double zoomFactor);
    void coordinatesUpdated(double lat, double lon);
    void itemAdded(QGraphicsItem *item);
    void angleChanged(double angle);

private:
   QGraphicsScene *m_scene;
    QGraphicsPixmapItem *m_chartImage = nullptr;
    ToolMode m_currentTool = ToolMode::Pan;
    double m_zoomLevel = 1.0;
    QColor m_strokeColor = QColor("#e94560");
    int m_strokeWidth = 3;
    QFont m_font = QFont("Arial", 12); // NUEVO
    QStack<QGraphicsItem*> m_undoStack;

    // Estado de dibujo
    QPointF m_dragStart;
    QGraphicsItem *m_currentDrawingItem = nullptr;
    bool m_isDragging = false;

    // Calibración
    QPointF m_calPixel1, m_calPixel2;
    GeoCoord m_calCoord1, m_calCoord2;
    bool m_isCalibrated = false;

    // Herramientas Overlay (SVG-based)
    DraggableProtractor *m_protractor = nullptr;
    DraggableRuler *m_ruler = nullptr;

    // Métodos privados
    void createPoint(const QPointF &pos);
    void startLine(const QPointF &pos);
    void updateGhostLine(const QPointF &pos);
    void finishLine(const QPointF &pos);
    void startArc(const QPointF &centerPos);
    void updateGhostArc(const QPointF &pos);
    void finishArc(const QPointF &pos);
    void createText(const QPointF &pos);
    void eraseItemAt(const QPointF &pos);
    QPointF adjustForZoom(const QPointF &viewPos) const;
};
