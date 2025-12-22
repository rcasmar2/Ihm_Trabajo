#include "draggableruler.h"
#include "rotationhandle.h"
#include <QSvgRenderer>
#include <QCursor>
#include <QtMath>
#include <QPainter>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QEvent>

// ============ RulerMarkerItem ============

RulerMarkerItem::RulerMarkerItem(DraggableRuler *parent)
    : QGraphicsEllipseItem(-25, -25, 50, 50, parent), m_ruler(parent)
{
    setFlags(ItemIsSelectable); // Visual feedback only
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setCursor(Qt::CrossCursor);
    
    setBrush(QColor("#e94560"));
    setZValue(1001);
}

void RulerMarkerItem::setColor(const QColor &color) {
    setBrush(color);
}

void RulerMarkerItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        event->accept();
    } else if (event->button() == Qt::RightButton) {
        m_isDrawing = true;
        m_drawStartRatio = m_ruler->markerRatio();
        event->accept();
    } else {
        QGraphicsEllipseItem::mousePressEvent(event);
    }
}

void RulerMarkerItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    // Calculamos posición relativa a la regla padre
    QPointF posInRuler = m_ruler->mapFromScene(event->scenePos());
    QRectF bounds = m_ruler->getRulerBoundingRect();
    
    // Actualizar posición del marcador
    double width = bounds.width();
    if (width > 0) {
        double newRatio = posInRuler.x() / width;
        newRatio = qBound(0.0, newRatio, 1.0); // Movimiento completo
        m_ruler->setMarkerRatio(newRatio);
    }
    
    // Si estamos dibujando (Right Click Drag), notificar preview
    if (m_isDrawing) {
         double startX = bounds.left() + width * m_drawStartRatio;
         double currentX = bounds.left() + width * m_ruler->markerRatio();
         double y = bounds.top(); // Dibujar en borde superior
         
         QPointF startPt = m_ruler->mapToScene(QPointF(startX, y));
         QPointF endPt = m_ruler->mapToScene(QPointF(currentX, y));
         
         m_ruler->notifyPreviewSegment(startPt, endPt);
    }
    
    event->accept();
}

void RulerMarkerItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::RightButton && m_isDrawing) {
        m_isDrawing = false;
        
        QRectF bounds = m_ruler->getRulerBoundingRect();
        double width = bounds.width();
        
        double startX = bounds.left() + width * m_drawStartRatio;
        double currentX = bounds.left() + width * m_ruler->markerRatio();
        double y = bounds.top();
        
        if (qAbs(startX - currentX) > 1.0) {
             QPointF startPt = m_ruler->mapToScene(QPointF(startX, y));
             QPointF endPt = m_ruler->mapToScene(QPointF(currentX, y));
             m_ruler->notifyDrawSegment(startPt, endPt);
        }
        event->accept();
    } else if (event->button() == Qt::LeftButton) {
        event->accept();
    } else {
        QGraphicsEllipseItem::mouseReleaseEvent(event);
    }
}


// ============ DraggableRuler ============

DraggableRuler::DraggableRuler(QGraphicsItem *parent)
    : QGraphicsSvgItem(parent)
{
    setSharedRenderer(new QSvgRenderer(QString(":/resources/icons/ruler.svg"), this));
    
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton); 
    
    setZValue(1000);
    setOpacity(0.85);
    
    updateTransformOrigin();
    
    // Crear el marcador usando la clase helper
    m_markerItem = new RulerMarkerItem(this);
    
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
    
    // Posición inicial
    m_markerRatio = 0.5;
    updateMarkerPosition();
    updateRotateHandlePosition();
}

void DraggableRuler::updateTransformOrigin() {
    QRectF bounds = boundingRect();
    if (bounds.isEmpty()) return;
    setTransformOriginPoint(bounds.center());
    // Actualizar posición del handle si cambia el bounds (p.ej. carga SVG)
    if (m_rotateHandle) updateRotateHandlePosition();
    if (m_markerItem) updateMarkerPosition();
}

void DraggableRuler::updateRotateHandlePosition() {
    QRectF bounds = boundingRect();
    if (bounds.isEmpty()) return;
    
    // A la izquierda, MÁS SEPARADO
    double x = bounds.left() - 60; // 60px a la izquierda
    double y = bounds.center().y() - 12; 
    
    if (m_rotateHandle)
        m_rotateHandle->setPos(x, y);
}

void DraggableRuler::updateMarkerPosition() {
    QRectF bounds = boundingRect();
    if (bounds.isEmpty()) return;
    
    double x = bounds.left() + bounds.width() * m_markerRatio;
    double y = bounds.top(); 
    
    m_markerItem->setPos(x, y);
}

void DraggableRuler::setMarkerRatio(double ratio) {
    if (!qFuzzyCompare(m_markerRatio, ratio)) {
        m_markerRatio = ratio;
        updateMarkerPosition();
        // Podríamos emitir signal si fuera necesario
    }
}

void DraggableRuler::notifyDrawSegment(const QPointF &start, const QPointF &end) {
    emit drawSegmentRequested(start, end);
}

void DraggableRuler::notifyPreviewSegment(const QPointF &start, const QPointF &end) {
    emit segmentPreviewRequested(start, end);
}

void DraggableRuler::setAngle(double angle) {
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

QPointF DraggableRuler::markerScenePos() const {
    return m_markerItem->scenePos();
}

void DraggableRuler::setMarkerColor(const QColor &color) {
    if (m_markerItem) {
        m_markerItem->setColor(color);
    }
}

void DraggableRuler::setDrawingEnabled(bool enabled) {
    if (m_markerItem) {
        m_markerItem->setVisible(enabled);
        m_markerItem->setEnabled(enabled);
    }
}

bool DraggableRuler::isNearEdge(const QPointF &localPos) const {
    QRectF bounds = boundingRect();
    double edgeThreshold = bounds.width() * 0.15;
    return localPos.x() < edgeThreshold || localPos.x() > (bounds.width() - edgeThreshold);
}

void DraggableRuler::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    // Si el evento llega aquí, es porque NO fue en el marcador (el marcador lo hubiera consumido)
    if (event->button() == Qt::LeftButton) {
        if (isNearEdge(event->pos())) {
            m_isRotating = true;
            m_isDragging = false;
        } else {
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
        QPointF delta = event->scenePos() - m_dragStart;
        moveBy(delta.x(), delta.y());
        m_dragStart = event->scenePos();
        emit positionChanged(pos());
        event->accept();
    } else if (m_isRotating) {
        QPointF center = mapToScene(transformOriginPoint());
        QPointF currentPos = event->scenePos();
        double angleToMouse = qAtan2(currentPos.y() - center.y(), currentPos.x() - center.x());
        double angleToStart = qAtan2(m_dragStart.y() - center.y(), m_dragStart.x() - center.x());
        double angleDelta = qRadiansToDegrees(angleToMouse - angleToStart);
        rotateBy(angleDelta);
        m_dragStart = currentPos;
        event->accept();
    } else {
        QGraphicsSvgItem::mouseMoveEvent(event);
    }
}

void DraggableRuler::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
        m_isRotating = false;
        setCursor(Qt::OpenHandCursor);
        event->accept();
    } else {
        QGraphicsSvgItem::mouseReleaseEvent(event);
    }
}

void DraggableRuler::wheelEvent(QGraphicsSceneWheelEvent *event) {
    double angleDelta = event->delta() / 12.0;
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
