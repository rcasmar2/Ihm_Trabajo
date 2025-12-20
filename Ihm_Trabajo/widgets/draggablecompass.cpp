#include "draggablecompass.h"
#include <QPainter>
#include <QCursor>
#include <QtMath>
#include <QGraphicsScene>

DraggableCompass::DraggableCompass(QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
    // Cargar el SVG del compass leg
    m_svgRenderer = new QSvgRenderer(QString(":/resources/icons/compass_leg.svg"), this);
    
    // Configurar flags de interacción
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges | ItemIsFocusable);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    
    // Z-order alto para estar encima
    setZValue(1000);
}

QRectF DraggableCompass::boundingRect() const {
    // El bounding rect debe incluir el círculo de preview y el compass leg
    double margin = m_strokeWidth + 30;
    return QRectF(-m_radius - margin, -m_radius - margin,
                  2 * m_radius + 2 * margin, 2 * m_radius + 2 * margin);
}

QPainterPath DraggableCompass::shape() const {
    // El shape define el área clickeable - incluir todo el círculo
    QPainterPath path;
    path.addEllipse(QPointF(0, 0), m_radius + 20, m_radius + 20);
    return path;
}

void DraggableCompass::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)
    
    painter->setRenderHint(QPainter::Antialiasing);
    
    // 1. Dibujar el círculo de preview (gris, punteado, translúcido)
    QPen previewPen(QColor(150, 150, 150, 150), 2, Qt::DashLine);
    painter->setPen(previewPen);
    painter->setBrush(Qt::NoBrush);
    painter->drawEllipse(QPointF(0, 0), m_radius, m_radius);
    
    // 2. Dibujar el arco en progreso (si está dibujando)
    if (m_isDrawingArc) {
        QPen arcPen(m_strokeColor, m_strokeWidth, Qt::SolidLine, Qt::RoundCap);
        painter->setPen(arcPen);
        
        // Calcular el rectángulo del arco
        QRectF arcRect(-m_radius, -m_radius, 2 * m_radius, 2 * m_radius);
        
        // Calcular ángulos para drawArc (Qt usa 1/16 de grado)
        double start = -m_arcStartAngle; // Qt mide counterclockwise desde las 3h
        double span = -(m_angle - m_arcStartAngle);
        
        painter->drawArc(arcRect, static_cast<int>(start * 16), static_cast<int>(span * 16));
    }
    
    // 3. Dibujar el compass leg SVG (el SVG es horizontal: 200x20)
    painter->save();
    
    // Rotar alrededor del centro según el ángulo actual
    painter->rotate(m_angle);
    
    if (m_svgRenderer && m_svgRenderer->isValid()) {
        // El SVG es 200x20 horizontal, necesitamos escalarlo al radio
        QSizeF svgSize = m_svgRenderer->defaultSize();
        double scale = m_radius / svgSize.width();
        
        // Dibujar el SVG desde el centro hacia afuera (positivo X)
        // Centrado verticalmente
        double svgHeight = svgSize.height() * scale;
        QRectF targetRect(0, -svgHeight / 2, m_radius, svgHeight);
        m_svgRenderer->render(painter, targetRect);
    } else {
        // Fallback: dibujar una línea simple con punta
        QPen legPen(QColor(200, 200, 200), 6, Qt::SolidLine, Qt::RoundCap);
        painter->setPen(legPen);
        painter->drawLine(QPointF(0, 0), QPointF(m_radius - 15, 0));
        
        // Punta de flecha
        QPolygonF arrow;
        arrow << QPointF(m_radius - 15, -6)
              << QPointF(m_radius, 0)
              << QPointF(m_radius - 15, 6);
        painter->setBrush(QColor(180, 180, 180));
        painter->setPen(QPen(QColor(80, 80, 80), 1));
        painter->drawPolygon(arrow);
    }
    
    painter->restore();
    
    // 4. Punto central (pivote del compás)
    painter->setPen(QPen(QColor(60, 60, 60), 2));
    painter->setBrush(m_strokeColor);
    painter->drawEllipse(QPointF(0, 0), 8, 8);
    
    // 5. Punto en el extremo (para indicar dónde arrastrar)
    double tipX = m_radius * qCos(qDegreesToRadians(m_angle));
    double tipY = m_radius * qSin(qDegreesToRadians(m_angle));
    painter->setPen(QPen(QColor(255, 255, 255), 2));
    painter->setBrush(QColor(100, 200, 100, 200));
    painter->drawEllipse(QPointF(tipX, tipY), 12, 12);
    
    // 6. Instrucciones (texto pequeño)
    painter->setPen(QColor(255, 255, 255));
    QFont font = painter->font();
    font.setPointSize(10);
    font.setBold(true);
    painter->setFont(font);
    
    if (m_isDrawingArc) {
        painter->drawText(QRectF(-100, m_radius + 15, 200, 25), 
                          Qt::AlignCenter, "Suelta para terminar arco");
    } else {
        painter->drawText(QRectF(-100, m_radius + 15, 200, 25), 
                          Qt::AlignCenter, "Clic derecho = dibujar arco");
    }
}

void DraggableCompass::setRadius(double r) {
    if (r > 10 && !qFuzzyCompare(m_radius, r)) {
        prepareGeometryChange();
        m_radius = r;
        emit radiusChanged(m_radius);
        update();
    }
}

void DraggableCompass::setAngle(double angle) {
    // Normalizar a [0, 360)
    while (angle < 0) angle += 360.0;
    while (angle >= 360) angle -= 360.0;
    
    if (!qFuzzyCompare(m_angle, angle)) {
        m_angle = angle;
        emit angleChanged(m_angle);
        update();
    }
}

void DraggableCompass::setColor(const QColor &color) {
    m_strokeColor = color;
    update();
}

void DraggableCompass::setStrokeWidth(int width) {
    m_strokeWidth = qBound(1, width, 20);
    update();
}

bool DraggableCompass::isNearLegTip(const QPointF &pos) const {
    // Calcular posición del extremo del compás
    double tipX = m_radius * qCos(qDegreesToRadians(m_angle));
    double tipY = m_radius * qSin(qDegreesToRadians(m_angle));
    QPointF tip(tipX, tipY);
    
    // Verificar si el punto está cerca (dentro de 20px)
    return QLineF(pos, tip).length() < 25;
}

double DraggableCompass::angleToPoint(const QPointF &pos) const {
    return qRadiansToDegrees(qAtan2(pos.y(), pos.x()));
}

void DraggableCompass::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF localPos = event->pos();
    
    if (event->button() == Qt::LeftButton) {
        // Shift + Click = mover, Click normal = rotar
        if (event->modifiers() & Qt::ShiftModifier) {
            // Iniciar arrastre
            m_isDragging = true;
            m_dragStart = event->scenePos();
            setCursor(Qt::ClosedHandCursor);
        } else {
            // Iniciar rotación
            m_isRotating = true;
            setCursor(Qt::ClosedHandCursor);
            
            // Actualizar ángulo inmediatamente
            double newAngle = angleToPoint(localPos);
            setAngle(newAngle);
        }
        event->accept();
    } else if (event->button() == Qt::RightButton) {
        // Clic derecho = iniciar dibujo de arco
        if (!m_isDrawingArc) {
            m_isDrawingArc = true;
            m_isRotating = true;
            m_arcStartAngle = angleToPoint(localPos);
            m_arcEndAngle = m_arcStartAngle;
            setAngle(m_arcStartAngle);
            setCursor(Qt::CrossCursor);
            update();
        }
        event->accept();
    } else {
        QGraphicsObject::mousePressEvent(event);
    }
}

void DraggableCompass::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (m_isRotating) {
        // Calcular nuevo ángulo basado en la posición del ratón
        QPointF localPos = event->pos();
        double newAngle = angleToPoint(localPos);
        setAngle(newAngle);
        
        // Si estamos dibujando, actualizar el ángulo final
        if (m_isDrawingArc) {
            m_arcEndAngle = m_angle;
        }
        
        event->accept();
    } else if (m_isDragging) {
        // Mover el compás
        QPointF delta = event->scenePos() - m_dragStart;
        moveBy(delta.x(), delta.y());
        m_dragStart = event->scenePos();
        emit positionChanged(pos());
        event->accept();
    } else {
        QGraphicsObject::mouseMoveEvent(event);
    }
}

void DraggableCompass::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (m_isRotating && !m_isDrawingArc) {
            m_isRotating = false;
            setCursor(Qt::OpenHandCursor);
        }
        if (m_isDragging) {
            m_isDragging = false;
            setCursor(Qt::OpenHandCursor);
        }
        event->accept();
    } else if (event->button() == Qt::RightButton) {
        // Clic derecho soltado = finalizar arco
        if (m_isDrawingArc) {
            finishArc();
            m_isRotating = false;
            setCursor(Qt::OpenHandCursor);
        }
        event->accept();
    } else {
        QGraphicsObject::mouseReleaseEvent(event);
    }
}

void DraggableCompass::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space && !event->isAutoRepeat()) {
        if (m_isRotating && !m_isDrawingArc) {
            // Iniciar dibujo del arco
            m_isDrawingArc = true;
            m_arcStartAngle = m_angle;
            m_arcEndAngle = m_angle;
            update();
        }
        event->accept();
    } else {
        QGraphicsObject::keyPressEvent(event);
    }
}

void DraggableCompass::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space && !event->isAutoRepeat()) {
        if (m_isDrawingArc) {
            // Finalizar el arco
            finishArc();
        }
        event->accept();
    } else {
        QGraphicsObject::keyReleaseEvent(event);
    }
}

void DraggableCompass::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    setCursor(Qt::OpenHandCursor);
    QGraphicsObject::hoverEnterEvent(event);
}

void DraggableCompass::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    setCursor(Qt::ArrowCursor);
    QGraphicsObject::hoverLeaveEvent(event);
}

QPainterPath DraggableCompass::getArcPath() const {
    QPainterPath path;
    
    // Calcular el span del arco
    double span = m_arcEndAngle - m_arcStartAngle;
    
    // Normalizar
    while (span < -180) span += 360;
    while (span > 180) span -= 360;
    
    if (qAbs(span) < 1) {
        return path; // Arco muy pequeño, ignorar
    }
    
    // Crear el arco
    QRectF arcRect(-m_radius, -m_radius, 2 * m_radius, 2 * m_radius);
    
    // Mover al punto de inicio
    double startRad = qDegreesToRadians(m_arcStartAngle);
    QPointF startPoint(m_radius * qCos(startRad), m_radius * qSin(startRad));
    path.moveTo(startPoint);
    
    // Añadir el arco
    path.arcTo(arcRect, -m_arcStartAngle, -span);
    
    return path;
}

void DraggableCompass::finishArc() {
    if (!m_isDrawingArc) return;
    
    // Obtener el path del arco
    QPainterPath arcPath = getArcPath();
    
    if (!arcPath.isEmpty()) {
        // Trasladar el path a la posición del compás en la escena
        QTransform transform;
        transform.translate(pos().x(), pos().y());
        arcPath = transform.map(arcPath);
        
        // Emitir señal con el arco completado
        emit arcCompleted(arcPath, m_strokeColor, m_strokeWidth);
    }
    
    // Resetear estado
    m_isDrawingArc = false;
    m_arcStartAngle = 0;
    m_arcEndAngle = 0;
    update();
}
