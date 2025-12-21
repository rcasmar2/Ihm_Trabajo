#pragma once

#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QPainterPath>
#include <QColor>
#include <QSvgRenderer>

/**
 * DraggableCompass - Compás interactivo para dibujar arcos
 * 
 * Overlay tool que muestra un compás con leg SVG y permite:
 * - Arrastrar para posicionar el centro
 * - Rotar arrastrando el extremo del compás
 * - Pulsar ESPACIO mientras rota para dibujar el arco
 * - Visualiza un círculo de preview punteado
 */
class DraggableCompass : public QGraphicsObject {
    Q_OBJECT

public:
    explicit DraggableCompass(QGraphicsItem *parent = nullptr);
    ~DraggableCompass() override = default;

    // QGraphicsItem interface
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // Propiedades
    double radius() const { return m_radius; }
    void setRadius(double r);
    double currentAngle() const { return m_angle; }
    void setAngle(double angle);
    
    // Estilos
    void setColor(const QColor &color);
    QColor color() const { return m_strokeColor; }
    void setStrokeWidth(int width);
    int strokeWidth() const { return m_strokeWidth; }
    
    // Control de dibujo de arco
    bool isDrawing() const { return m_isDrawingArc; }
    double startAngle() const { return m_arcStartAngle; }
    double endAngle() const { return m_arcEndAngle; }
    QPainterPath getArcPath() const;

signals:
    void angleChanged(double angle);
    void radiusChanged(double radius);
    void arcCompleted(const QPainterPath &path, const QColor &color, int width);
    void positionChanged(const QPointF &newPos);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    // Estado
    double m_radius = 200.0;  // Radio inicial más grande
    double m_angle = 0.0;
    QColor m_strokeColor = QColor("#e94560");
    int m_strokeWidth = 3;
    
    // Indicador inicial (desaparece al usar)
    bool m_showIndicator = true;
    
    // Interacción
    bool m_isDragging = false;
    bool m_isRotating = false;
    QPointF m_dragStart;
    
    // Dibujo de arco
    bool m_isDrawingArc = false;
    double m_arcStartAngle = 0.0;
    double m_arcEndAngle = 0.0;
    
    // SVG del compass leg
    QSvgRenderer *m_svgRenderer = nullptr;
    
    // Helpers
    bool isNearLegTip(const QPointF &pos) const;
    double angleToPoint(const QPointF &pos) const;
    void finishArc();
};
