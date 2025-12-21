#include "draggableprotractor.h"
#include "rotationhandle.h"
#include <QSvgRenderer>
#include <QCursor>
#include <QtMath>

DraggableProtractor::DraggableProtractor(QGraphicsItem *parent)
    : QGraphicsSvgItem(parent)
{
    // Cargar el SVG del transportador desde recursos
    setSharedRenderer(new QSvgRenderer(QString(":/resources/icons/transportador.svg"), this));
    
    // Configurar flags de interacción
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    
    // Configurar z-order alto para estar encima de todo
    setZValue(1000);
    
    // Establecer opacidad semi-transparente
    setOpacity(0.85);
    
    // Crear handle de rotación
    m_rotateHandle = new RotationHandle(this);
    m_rotateHandle->setZValue(1001);
    
    // Conectar señales de rotación por arrastre
    connect(m_rotateHandle, &RotationHandle::dragStarted, [this](const QPointF &pos) {
        m_dragStart = pos; 
    });
    connect(m_rotateHandle, &RotationHandle::dragged, [this](const QPointF &pos) {
        QPointF center = mapToScene(transformOriginPoint());
        double angleToMouse = qAtan2(pos.y() - center.y(), pos.x() - center.x());
        double angleToStart = qAtan2(m_dragStart.y() - center.y(), m_dragStart.x() - center.x());
        double angleDelta = qRadiansToDegrees(angleToMouse - angleToStart);
        
        rotateBy(angleDelta);
        m_dragStart = pos;
    });
    
    // Configurar el punto de origen de transformación (centro del transportador)
    updateTransformOrigin();
    updateRotateHandlePosition();
}

void DraggableProtractor::updateTransformOrigin() {
    // El centro de rotación es el centro del bounding rect
    QRectF bounds = boundingRect();
    setTransformOriginPoint(bounds.center());
    if (m_rotateHandle) updateRotateHandlePosition();
}

void DraggableProtractor::updateRotateHandlePosition() {
    QRectF bounds = boundingRect();
    if (bounds.isEmpty()) return;
    
    // A la izquierda del transportador, MÁS SEPARADO
    double x = bounds.left() - 60; 
    double y = bounds.center().y() - 12; // Centrar verticalmente (icono 24px)
    
    if (m_rotateHandle)
        m_rotateHandle->setPos(x, y);
}

void DraggableProtractor::setAngle(double angle) {
    // Normalizar ángulo a [0, 360)
    while (angle < 0) angle += 360.0;
    while (angle >= 360) angle -= 360.0;
    
    if (!qFuzzyCompare(m_angle, angle)) {
        m_angle = angle;
        setRotation(m_angle);
        emit angleChanged(m_angle);
    }
}

void DraggableProtractor::rotateBy(double delta) {
    setAngle(m_angle + delta);
}

void DraggableProtractor::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isDragging = true;
        m_dragStart = event->scenePos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
    } else {
        QGraphicsSvgItem::mousePressEvent(event);
    }
}

void DraggableProtractor::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (m_isDragging) {
        // Mover el transportador
        QPointF delta = event->scenePos() - m_dragStart;
        moveBy(delta.x(), delta.y());
        m_dragStart = event->scenePos();
        emit positionChanged(pos());
        event->accept();
    } else {
        QGraphicsSvgItem::mouseMoveEvent(event);
    }
}

void DraggableProtractor::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton && m_isDragging) {
        m_isDragging = false;
        setCursor(Qt::OpenHandCursor);
        event->accept();
    } else {
        QGraphicsSvgItem::mouseReleaseEvent(event);
    }
}

void DraggableProtractor::wheelEvent(QGraphicsSceneWheelEvent *event) {
    // Rotar con la rueda del ratón
    double angleDelta = event->delta() / 12.0; // ~30° por "notch" completo
    rotateBy(angleDelta);
    event->accept();
}

void DraggableProtractor::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    setCursor(Qt::OpenHandCursor);
    QGraphicsSvgItem::hoverEnterEvent(event);
}

void DraggableProtractor::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    setCursor(Qt::ArrowCursor);
    QGraphicsSvgItem::hoverLeaveEvent(event);
}
