#pragma once

#include <QGraphicsSvgItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>

/**
 * DraggableProtractor - Transportador de ángulos interactivo
 * 
 * Overlay tool que carga un SVG del transportador y permite:
 * - Arrastrar (drag) para posicionar sobre la carta
 * - Rotar con la rueda del ratón o manejador visual
 * - Emite señales cuando cambia el ángulo
 */
class DraggableProtractor : public QGraphicsSvgItem {
    Q_OBJECT

public:
    explicit DraggableProtractor(QGraphicsItem *parent = nullptr);
    ~DraggableProtractor() override = default;

    double angle() const { return m_angle; }
    void setAngle(double angle);
    void rotateBy(double delta);

signals:
    void angleChanged(double newAngle);
    void positionChanged(const QPointF &newPos);

protected:
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

    void updateTransformOrigin();
};
