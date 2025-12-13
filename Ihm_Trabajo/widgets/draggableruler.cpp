#include "draggableruler.h"
#include <QSvgRenderer>
#include <QCursor>
#include <QtMath>

DraggableRuler::DraggableRuler(QGraphicsItem *parent)
    : QGraphicsSvgItem(parent)
{
    // Cargar el SVG de la regla desde recursos
    setSharedRenderer(new QSvgRenderer(QString(":/resources/icons/ruler.svg"), this));
    
    // Configurar flags de interacción
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    
    // Configurar z-order alto para estar encima de todo
    setZValue(1000);
    
    // Establecer opacidad semi-transparente para ver la carta debajo
    setOpacity(0.85);
    
    // Configurar el punto de origen de transformación (centro de la regla)
    updateTransformOrigin();
}

void DraggableRuler::updateTransformOrigin() {
    // El centro de rotación está en el centro del bounding rect
    QRectF bounds = boundingRect();
    setTransformOriginPoint(bounds.center());
}

void DraggableRuler::setAngle(double angle) {
    // Normalizar ángulo a [0, 360)
    while (angle < 0) angle += 360.0;
    while (angle >= 360) angle -= 360.0;
    
    if (!qFuzzyCompare(m_angle, angle)) {
        m_angle = angle;
        setRotation(m_angle);
        emit angleChanged(m_angle);
    }
}

void DraggableRuler::rotateBy(double delta) {
    setAngle(m_angle + delta);
}

bool DraggableRuler::isNearEdge(const QPointF &localPos) const {
    // Considera "cerca del borde" si está en el 15% de los extremos
    QRectF bounds = boundingRect();
    double edgeThreshold = bounds.width() * 0.15;
    
    return localPos.x() < edgeThreshold || 
           localPos.x() > (bounds.width() - edgeThreshold);
}

void DraggableRuler::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        QPointF localPos = event->pos();
        
        if (isNearEdge(localPos)) {
            // Iniciar rotación desde los extremos
            m_isRotating = true;
            m_isDragging = false;
        } else {
            // Iniciar arrastre desde el centro
            m_isDragging = true;
            m_isRotating = false;
        }
        
        m_dragStart = event->scenePos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
    } else {
        QGraphicsSvgItem::mousePressEvent(event);
    }
}

void DraggableRuler::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (m_isDragging) {
        // Mover la regla
        QPointF delta = event->scenePos() - m_dragStart;
        moveBy(delta.x(), delta.y());
        m_dragStart = event->scenePos();
        emit positionChanged(pos());
        event->accept();
    } else if (m_isRotating) {
        // Rotar la regla basándose en el movimiento del ratón
        QPointF center = mapToScene(transformOriginPoint());
        QPointF currentPos = event->scenePos();
        
        // Calcular ángulo desde el centro al punto actual
        double angleToMouse = qAtan2(currentPos.y() - center.y(), 
                                      currentPos.x() - center.x());
        double angleToStart = qAtan2(m_dragStart.y() - center.y(), 
                                      m_dragStart.x() - center.x());
        
        double angleDelta = qRadiansToDegrees(angleToMouse - angleToStart);
        rotateBy(angleDelta);
        
        m_dragStart = currentPos;
        event->accept();
    } else {
        QGraphicsSvgItem::mouseMoveEvent(event);
    }
}

void DraggableRuler::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton && (m_isDragging || m_isRotating)) {
        m_isDragging = false;
        m_isRotating = false;
        setCursor(Qt::OpenHandCursor);
        event->accept();
    } else {
        QGraphicsSvgItem::mouseReleaseEvent(event);
    }
}

void DraggableRuler::wheelEvent(QGraphicsSceneWheelEvent *event) {
    // Rotar con la rueda del ratón
    double angleDelta = event->delta() / 12.0; // ~30° por "notch" completo
    rotateBy(angleDelta);
    event->accept();
}

void DraggableRuler::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    setCursor(Qt::OpenHandCursor);
    QGraphicsSvgItem::hoverEnterEvent(event);
}

void DraggableRuler::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    setCursor(Qt::ArrowCursor);
    QGraphicsSvgItem::hoverLeaveEvent(event);
}
