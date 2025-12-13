#pragma once

#include <QGraphicsSvgItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>

/**
 * DraggableRuler - Regla interactiva para medir distancias
 * 
 * Overlay tool que carga un SVG de la regla y permite:
 * - Arrastrar (drag) para posicionar sobre la carta
 * - Rotar con la rueda del ratón o arrastrando desde los extremos
 * - NO muestra popup de distancia (simulación realista: 
 *   el usuario debe arrastrarla a la escala de latitudes)
 */
class DraggableRuler : public QGraphicsSvgItem {
    Q_OBJECT

public:
    explicit DraggableRuler(QGraphicsItem *parent = nullptr);
    ~DraggableRuler() override = default;

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
    bool m_isRotating = false;
    
    // Determina si el click está cerca de los extremos (para rotar)
    bool isNearEdge(const QPointF &localPos) const;
    void updateTransformOrigin();
};
