#include "rotationhandle.h"
#include <QSvgRenderer>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

RotationHandle::RotationHandle(QGraphicsItem *parent)
    : QGraphicsSvgItem(parent)
{
    setSharedRenderer(new QSvgRenderer(QString(":/resources/icons/rotate_icon.svg"), this));
    setFlags(ItemIsSelectable); 
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    
    // Escalar para hacerlo MUY GRANDE (2.5x)
    setScale(2.5);
}

void RotationHandle::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isDragging = true;
        setCursor(Qt::ClosedHandCursor);
        emit dragStarted(event->scenePos());
        event->accept();
    }
}

void RotationHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (m_isDragging) {
        emit dragged(event->scenePos());
        event->accept();
    } else {
        QGraphicsSvgItem::mouseMoveEvent(event);
    }
}

void RotationHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton && m_isDragging) {
        m_isDragging = false;
        setCursor(Qt::PointingHandCursor);
        emit dragFinished();
        event->accept();
    } else {
        QGraphicsSvgItem::mouseReleaseEvent(event);
    }
}

void RotationHandle::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    setCursor(Qt::PointingHandCursor);
    QGraphicsSvgItem::hoverEnterEvent(event);
}

void RotationHandle::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    setCursor(Qt::ArrowCursor);
    QGraphicsSvgItem::hoverLeaveEvent(event);
}

void RotationHandle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    // Dibujar fondo circular blanco para máxima visibilidad
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(255, 255, 255, 220)); 
    
    QRectF b = boundingRect();
    double r = qMin(b.width(), b.height()) / 2.0;
    // Dibujar un poco más grande que el icono
    painter->drawEllipse(b.center(), r * 1.3, r * 1.3);
    painter->restore();

    // Dibujar el SVG del icono
    QGraphicsSvgItem::paint(painter, option, widget);
}
