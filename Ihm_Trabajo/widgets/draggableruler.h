#pragma once

#include <QGraphicsSvgItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>

class QGraphicsEllipseItem;
class RotationHandle; // Forward declaration

class DraggableRuler;

/**
 * RulerMarkerItem - Clase helper para el marcador interactivo
 * Gestiona sus propios eventos para evitar conflictos con la regla padre.
 */
class RulerMarkerItem : public QGraphicsEllipseItem {
public:
    RulerMarkerItem(DraggableRuler *parent);
    void setColor(const QColor &color); // Nuevo método
    
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    
private:
    DraggableRuler *m_ruler;
    bool m_isDrawing = false;
    double m_drawStartRatio = 0.0;
};

/**
 * DraggableRuler - Regla interactiva para medir y dibujar líneas
 */
class DraggableRuler : public QGraphicsSvgItem {
    Q_OBJECT

    friend class RulerMarkerItem; // Permitir que el marcador acceda a sets privados si necesario

public:
    explicit DraggableRuler(QGraphicsItem *parent = nullptr);
    ~DraggableRuler() override = default;

    double angle() const { return m_angle; }
    void setAngle(double angle);
    void rotateBy(double delta);
    
    // Posición del marcador en la regla (punto de dibujo)
    QPointF markerScenePos() const;
    void setMarkerColor(const QColor &color);
    
    // Métodos públicos para que el marcador actualice el estado
    void setMarkerRatio(double ratio);
    double markerRatio() const { return m_markerRatio; }
    void notifyDrawSegment(const QPointF &start, const QPointF &end);
    void notifyPreviewSegment(const QPointF &start, const QPointF &end);
    
    // Configurar si permite dibujo (marcador visible/activo)
    void setDrawingEnabled(bool enabled);

    QRectF getRulerBoundingRect() const { return boundingRect(); }

signals:
    void angleChanged(double newAngle);
    void positionChanged(const QPointF &newPos);
    void drawSegmentRequested(const QPointF &startPos, const QPointF &endPos);
    void segmentPreviewRequested(const QPointF &startPos, const QPointF &endPos);

protected:
    // Eventos de la regla (padre)
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    double m_angle = 0.0;
    QPointF m_dragStart;
    bool m_isDragging = false;
    bool m_isRotating = false;
    
    // Marcador interactivo (hijo)
    RulerMarkerItem *m_markerItem = nullptr;
    void updateMarkerPosition();
    
    // Icono de rotación
    RotationHandle *m_rotateHandle = nullptr; 
    void updateRotateHandlePosition();
    
    // Posición relativa del marcador (0.0 a 1.0)
    double m_markerRatio = 0.5;

    // Determina si el click está cerca de los extremos (para rotar)
    bool isNearEdge(const QPointF &localPos) const;
    void updateTransformOrigin();
    
    // Filter YA NO ES NECESARIO
    // bool sceneEventFilter(QGraphicsItem *watched, QEvent *event) override;
};
