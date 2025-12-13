#include "chartcontroller.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QGraphicsPathItem>
#include <QPen>
#include <QBrush>
#include <QPainterPath>
#include <QtMath>
#include <QInputDialog>
#include <QPainter>

// =============================================================================
// ChartController Implementation
// =============================================================================

ChartController::ChartController(QGraphicsScene *scene, QObject *parent)
    : QObject(parent)
    , m_scene(scene)
    , m_protractor(nullptr)
{
}

ChartController::~ChartController() {
}

void ChartController::loadChart(const QString &imagePath) {
    QPixmap pixmap(imagePath);
    if (pixmap.isNull()) {
        qWarning() << "No se pudo cargar la carta:" << imagePath;
        return;
    }

    if (m_chartImage) {
        m_scene->removeItem(m_chartImage);
        delete m_chartImage;
    }

    m_chartImage = m_scene->addPixmap(pixmap);
    m_chartImage->setZValue(-1000);
    m_scene->setSceneRect(pixmap.rect());
}

QSizeF ChartController::chartSize() const {
    if (m_chartImage) {
        return m_chartImage->pixmap().size();
    }
    return QSizeF();
}

// === NAVEGACIÓN ===

void ChartController::zoomIn() {
    zoomTo(m_zoomLevel * 1.25);
}

void ChartController::zoomOut() {
    zoomTo(m_zoomLevel * 0.8);
}

void ChartController::zoomTo(double factor) {
    factor = qBound(0.1, factor, 10.0);
    m_zoomLevel = factor;
    emit zoomChanged(m_zoomLevel);
}

void ChartController::resetZoom() {
    zoomTo(1.0);
}

// === HERRAMIENTAS ===

void ChartController::setTool(ToolMode mode) {
    if (m_currentTool != mode) {
        if (m_currentDrawingItem) {
            m_scene->removeItem(m_currentDrawingItem);
            delete m_currentDrawingItem;
            m_currentDrawingItem = nullptr;
        }
        m_isDragging = false;

        if (mode == ToolMode::Protractor) {
            showProtractor(true);
        }

        m_currentTool = mode;
        emit toolChanged(mode);
    }
}

// === MÁQUINA DE ESTADOS ===

void ChartController::handleMousePressAt(const QPointF &scenePos, Qt::MouseButton button) {
    if (button != Qt::LeftButton) return;

    m_dragStart = scenePos;
    m_isDragging = true;

    switch (m_currentTool) {
    case ToolMode::Pan:
        break;
    case ToolMode::Point:
        createPoint(scenePos);
        break;
    case ToolMode::Line:
    case ToolMode::Ruler:
        startLine(scenePos);
        break;
    case ToolMode::Arc:
        startArc(scenePos);
        break;
    case ToolMode::Text:
        createText(scenePos);
        break;
    case ToolMode::Eraser:
        eraseItemAt(scenePos);
        break;
    default:
        break;
    }
}

void ChartController::handleMouseMoveAt(const QPointF &scenePos) {
    GeoCoord coord = getCoordinatesAt(scenePos);
    emit coordinatesUpdated(coord.latitude, coord.longitude);

    if (!m_isDragging) return;

    switch (m_currentTool) {
    case ToolMode::Line:
    case ToolMode::Ruler:
        updateGhostLine(scenePos);
        break;
    case ToolMode::Arc:
        updateGhostArc(scenePos);
        break;
    default:
        break;
    }
}

void ChartController::handleMouseReleaseAt(const QPointF &scenePos, Qt::MouseButton button) {
    if (button != Qt::LeftButton) return;

    if (m_isDragging) {
        switch (m_currentTool) {
        case ToolMode::Line:
        case ToolMode::Ruler:
            finishLine(scenePos);
            break;
        case ToolMode::Arc:
            finishArc(scenePos);
            break;
        default:
            break;
        }
    }

    m_isDragging = false;
}

void ChartController::handleWheelDelta(int delta) {
    if (m_currentTool == ToolMode::Protractor && m_protractor) {
        // Rotar transportador con la rueda
        double angleDelta = delta / 12.0;
        rotateProtractor(angleDelta);
    } else {
        // Zoom normal
        if (delta > 0) {
            zoomIn();
        } else {
            zoomOut();
        }
    }
}

// === GESTIÓN DE ELEMENTOS ===

void ChartController::clearAllDrawings() {
    QList<QGraphicsItem*> items = m_scene->items();
    for (QGraphicsItem *item : items) {
        if (item != m_chartImage && item != m_protractor) {
            m_scene->removeItem(item);
            delete item;
        }
    }
    m_undoStack.clear();
}

void ChartController::deleteSelectedItems() {
    QList<QGraphicsItem*> selected = m_scene->selectedItems();
    for (QGraphicsItem *item : selected) {
        if (item != m_chartImage && item != m_protractor) {
            m_scene->removeItem(item);
            delete item;
        }
    }
}

void ChartController::undoLastAction() {
    if (!m_undoStack.isEmpty()) {
        QGraphicsItem *item = m_undoStack.pop();
        m_scene->removeItem(item);
        delete item;
    }
}

// === COORDENADAS ===
// TODO: Insertar fórmulas de conversión Píxel -> Latitud/Longitud

GeoCoord ChartController::getCoordinatesAt(const QPointF &scenePos) const {
    if (!m_isCalibrated) {
        double chartWidth = m_chartImage ? m_chartImage->pixmap().width() : 3000;
        double chartHeight = m_chartImage ? m_chartImage->pixmap().height() : 2000;
        
        // Estrecho de Gibraltar (aproximado)
        double latTop = 36.33;
        double latBottom = 35.67;
        double lonLeft = -6.0;
        double lonRight = -5.17;
        
        double lat = latTop - (scenePos.y() / chartHeight) * (latTop - latBottom);
        double lon = lonLeft + (scenePos.x() / chartWidth) * (lonRight - lonLeft);
        
        return GeoCoord{lat, lon};
    }

    double dx = m_calPixel2.x() - m_calPixel1.x();
    double dy = m_calPixel2.y() - m_calPixel1.y();

    if (qFuzzyIsNull(dx) || qFuzzyIsNull(dy)) {
        return GeoCoord{0, 0};
    }

    double ratioX = (scenePos.x() - m_calPixel1.x()) / dx;
    double ratioY = (scenePos.y() - m_calPixel1.y()) / dy;

    double lon = m_calCoord1.longitude + ratioX * (m_calCoord2.longitude - m_calCoord1.longitude);
    double lat = m_calCoord1.latitude + ratioY * (m_calCoord2.latitude - m_calCoord1.latitude);

    return GeoCoord{lat, lon};
}

QPointF ChartController::getScenePosFromCoord(const GeoCoord &coord) const {
    if (!m_isCalibrated) {
        double chartWidth = m_chartImage ? m_chartImage->pixmap().width() : 3000;
        double chartHeight = m_chartImage ? m_chartImage->pixmap().height() : 2000;
        
        double latTop = 36.33;
        double latBottom = 35.67;
        double lonLeft = -6.0;
        double lonRight = -5.17;
        
        double x = ((coord.longitude - lonLeft) / (lonRight - lonLeft)) * chartWidth;
        double y = ((latTop - coord.latitude) / (latTop - latBottom)) * chartHeight;
        
        return QPointF(x, y);
    }

    double dLon = m_calCoord2.longitude - m_calCoord1.longitude;
    double dLat = m_calCoord2.latitude - m_calCoord1.latitude;

    if (qFuzzyIsNull(dLon) || qFuzzyIsNull(dLat)) {
        return QPointF(0, 0);
    }

    double ratioX = (coord.longitude - m_calCoord1.longitude) / dLon;
    double ratioY = (coord.latitude - m_calCoord1.latitude) / dLat;

    double x = m_calPixel1.x() + ratioX * (m_calPixel2.x() - m_calPixel1.x());
    double y = m_calPixel1.y() + ratioY * (m_calPixel2.y() - m_calPixel1.y());

    return QPointF(x, y);
}

void ChartController::setCalibrationPoints(const QPointF &pixel1, const GeoCoord &coord1,
                                            const QPointF &pixel2, const GeoCoord &coord2) {
    m_calPixel1 = pixel1;
    m_calPixel2 = pixel2;
    m_calCoord1 = coord1;
    m_calCoord2 = coord2;
    m_isCalibrated = true;
}

// === ESTILOS ===

void ChartController::setStrokeColor(const QColor &color) {
    m_strokeColor = color;
}

void ChartController::setStrokeWidth(int width) {
    m_strokeWidth = qBound(1, width, 20);
}

// === TRANSPORTADOR ===

void ChartController::showProtractor(bool visible) {
    if (visible && !m_protractor) {
        // Crear imagen del transportador
        QImage img(300, 160, QImage::Format_ARGB32);
        img.fill(Qt::transparent);
        QPainter painter(&img);
        painter.setRenderHint(QPainter::Antialiasing);
        
        // Semicírculo
        QPen pen(QColor("#27ae60"), 3);
        painter.setPen(pen);
        painter.drawArc(10, 10, 280, 280, 0, 180 * 16);
        
        // Marcas de grados
        pen.setWidth(1);
        painter.setPen(pen);
        QFont font("Arial", 8);
        painter.setFont(font);
        
        for (int angle = 0; angle <= 180; angle += 10) {
            double rad = qDegreesToRadians((double)angle);
            double x1 = 150 + 130 * qCos(rad);
            double y1 = 150 - 130 * qSin(rad);
            double x2 = 150 + 140 * qCos(rad);
            double y2 = 150 - 140 * qSin(rad);
            painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
            
            if (angle % 30 == 0) {
                double xt = 150 + 115 * qCos(rad) - 10;
                double yt = 150 - 115 * qSin(rad) + 5;
                painter.drawText(QPointF(xt, yt), QString::number(angle));
            }
        }
        
        // Línea base
        pen.setWidth(2);
        painter.setPen(pen);
        painter.drawLine(10, 150, 290, 150);
        
        // Centro
        painter.setBrush(QColor("#e94560"));
        painter.drawEllipse(QPointF(150, 150), 5, 5);
        
        m_protractor = m_scene->addPixmap(QPixmap::fromImage(img));
        m_protractor->setTransformOriginPoint(150, 150);
        m_protractor->setZValue(1000);
        m_protractor->setOpacity(0.9);
        m_protractor->setFlag(QGraphicsItem::ItemIsMovable);
        m_protractor->setFlag(QGraphicsItem::ItemIsSelectable);
        
        QRectF sceneRect = m_scene->sceneRect();
        m_protractor->setPos(
            sceneRect.center().x() - 150,
            sceneRect.center().y() - 80
        );
        
    } else if (!visible && m_protractor) {
        m_scene->removeItem(m_protractor);
        delete m_protractor;
        m_protractor = nullptr;
    }
}

void ChartController::rotateProtractor(double angleDelta) {
    if (m_protractor) {
        m_protractorAngle += angleDelta;
        while (m_protractorAngle < 0) m_protractorAngle += 360;
        while (m_protractorAngle >= 360) m_protractorAngle -= 360;
        m_protractor->setRotation(m_protractorAngle);
        emit angleChanged(m_protractorAngle);
    }
}

bool ChartController::isProtractorVisible() const {
    return m_protractor != nullptr;
}

double ChartController::protractorAngle() const {
    return m_protractorAngle;
}

// === MÉTODOS PRIVADOS ===

void ChartController::createPoint(const QPointF &pos) {
    QPen pen(m_strokeColor, m_strokeWidth);
    QBrush brush(m_strokeColor);

    double radius = m_strokeWidth * 1.5;
    QGraphicsEllipseItem *point = m_scene->addEllipse(
        pos.x() - radius, pos.y() - radius,
        radius * 2, radius * 2,
        pen, brush
    );

    point->setFlag(QGraphicsItem::ItemIsSelectable);
    point->setFlag(QGraphicsItem::ItemIsMovable);
    m_undoStack.push(point);
    emit itemAdded(point);
}

void ChartController::startLine(const QPointF &pos) {
    QPen pen(m_strokeColor, m_strokeWidth);
    pen.setStyle(Qt::DashLine);

    QGraphicsLineItem *ghostLine = m_scene->addLine(
        pos.x(), pos.y(), pos.x(), pos.y(), pen
    );
    
    ghostLine->setZValue(500);
    m_currentDrawingItem = ghostLine;
}

void ChartController::updateGhostLine(const QPointF &pos) {
    if (auto *line = qgraphicsitem_cast<QGraphicsLineItem*>(m_currentDrawingItem)) {
        QLineF newLine(m_dragStart, pos);
        line->setLine(newLine);
    }
}

void ChartController::finishLine(const QPointF &pos) {
    if (m_currentDrawingItem) {
        m_scene->removeItem(m_currentDrawingItem);
        delete m_currentDrawingItem;
        m_currentDrawingItem = nullptr;

        double distance = QLineF(m_dragStart, pos).length();
        if (distance < 5) return;

        QPen pen(m_strokeColor, m_strokeWidth);
        pen.setStyle(Qt::SolidLine);
        pen.setCapStyle(Qt::RoundCap);

        QGraphicsLineItem *line = m_scene->addLine(
            m_dragStart.x(), m_dragStart.y(),
            pos.x(), pos.y(),
            pen
        );

        line->setFlag(QGraphicsItem::ItemIsSelectable);
        m_undoStack.push(line);
        emit itemAdded(line);
    }
}

void ChartController::startArc(const QPointF &centerPos) {
    m_dragStart = centerPos;

    QPen pen(m_strokeColor, m_strokeWidth);
    pen.setStyle(Qt::DashLine);

    QPainterPath path;
    path.addEllipse(centerPos, 1, 1);

    QGraphicsPathItem *ghostArc = m_scene->addPath(path, pen);
    ghostArc->setZValue(500);
    m_currentDrawingItem = ghostArc;
}

void ChartController::updateGhostArc(const QPointF &pos) {
    if (auto *arcItem = qgraphicsitem_cast<QGraphicsPathItem*>(m_currentDrawingItem)) {
        double radius = QLineF(m_dragStart, pos).length();
        
        QPainterPath path;
        path.addEllipse(m_dragStart, radius, radius);
        
        arcItem->setPath(path);
    }
}

void ChartController::finishArc(const QPointF &pos) {
    if (m_currentDrawingItem) {
        m_scene->removeItem(m_currentDrawingItem);
        delete m_currentDrawingItem;
        m_currentDrawingItem = nullptr;

        double radius = QLineF(m_dragStart, pos).length();
        if (radius < 5) return;

        QPen pen(m_strokeColor, m_strokeWidth);
        pen.setStyle(Qt::SolidLine);

        QPainterPath path;
        path.addEllipse(m_dragStart, radius, radius);

        QGraphicsPathItem *arc = m_scene->addPath(path, pen);
        arc->setFlag(QGraphicsItem::ItemIsSelectable);
        m_undoStack.push(arc);
        emit itemAdded(arc);
    }
}

void ChartController::createText(const QPointF &pos) {
    bool ok;
    QString text = QInputDialog::getText(
        nullptr, "Añadir Texto", "Texto:", QLineEdit::Normal, "", &ok
    );

    if (ok && !text.isEmpty()) {
        QGraphicsTextItem *textItem = m_scene->addText(text);
        textItem->setPos(pos);
        textItem->setDefaultTextColor(m_strokeColor);
        textItem->setFont(QFont("Arial", m_strokeWidth * 4));
        textItem->setFlag(QGraphicsItem::ItemIsSelectable);
        textItem->setFlag(QGraphicsItem::ItemIsMovable);
        
        m_undoStack.push(textItem);
        emit itemAdded(textItem);
    }
}

void ChartController::eraseItemAt(const QPointF &pos) {
    QList<QGraphicsItem*> items = m_scene->items(pos);
    for (QGraphicsItem *item : items) {
        if (item != m_chartImage && item != m_protractor) {
            m_scene->removeItem(item);
            delete item;
            break;
        }
    }
}

QPointF ChartController::adjustForZoom(const QPointF &viewPos) const {
    return viewPos / m_zoomLevel;
}
