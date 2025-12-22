#include "chartcontroller.h"
#include "draggableprotractor.h"
#include "draggableruler.h"
#include "draggablecompass.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QGraphicsPathItem>
#include <QPen>
#include <QBrush>
#include <QPainterPath>
#include <QtMath>
#include <QInputDialog>

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
    // Modificación para soportar cambio entre Ruler y RulerDraw sin destruir la regla
    // Si cambiamos de Ruler -> RulerDraw o viceversa, no entra en el bloque principal if (m_currentTool != mode)
    // porque ambos son modos diferentes, PERO sí queremos reusar la regla y actualizar el estado de dibujo.
    
    // Si ya tenemos regla y cambiamos entre modos de regla (Ruler, Line, RulerDraw):
    if (m_ruler && (mode == ToolMode::Ruler || mode == ToolMode::RulerDraw || mode == ToolMode::Line) &&
                   (m_currentTool == ToolMode::Ruler || m_currentTool == ToolMode::RulerDraw || m_currentTool == ToolMode::Line)) {
        m_ruler->setDrawingEnabled(mode == ToolMode::RulerDraw || mode == ToolMode::Line);
        m_currentTool = mode;
        emit toolChanged(mode);
        return;
    }

    if (m_currentTool != mode) {
        if (m_currentDrawingItem) {
            m_scene->removeItem(m_currentDrawingItem);
            delete m_currentDrawingItem;
            m_currentDrawingItem = nullptr;
        }
        m_isDragging = false;
        
        // ... switch block rest

        // Gestionar visibilidad de herramientas overlay
        if (mode == ToolMode::Protractor) {
            showProtractor(true);
            showRuler(false);
            showCompass(false);
        } else if (mode == ToolMode::Ruler || mode == ToolMode::Line || mode == ToolMode::RulerDraw) {
            // Línea y Regla usan la misma herramienta visual (regla SVG)
            showRuler(true);
            
            // Configurar si se permite dibujar con la regla (el marcador rojo)
            if (m_ruler) {
                // Permitir dibujar si es RulerDraw o Line
                m_ruler->setDrawingEnabled(mode == ToolMode::RulerDraw || mode == ToolMode::Line);
            }

            showProtractor(false);
            showCompass(false);
        } else if (mode == ToolMode::Arc) {
            showCompass(true);
            showProtractor(false);
            showRuler(false);
        } else {
            // Ocultar todas las herramientas overlay
            showProtractor(false);
            showRuler(false);
            showCompass(false);
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
        // No hacer nada - el DraggableCompass maneja el dibujo de arcos
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
        // No hacer nada - el DraggableCompass maneja el dibujo de arcos
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
            // No hacer nada - el DraggableCompass maneja el dibujo de arcos
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
    } else if ((m_currentTool == ToolMode::Ruler || m_currentTool == ToolMode::Line || m_currentTool == ToolMode::RulerDraw) && m_ruler) {
        // Rotar regla con la rueda (también en modo Línea)
        double angleDelta = delta / 12.0;
        rotateRuler(angleDelta);
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
        // Excluir la carta de fondo y las herramientas overlay
        if (item != m_chartImage && item != m_protractor && item != m_ruler && item != m_compass) {
            m_scene->removeItem(item);
            delete item;
        }
    }
    m_undoStack.clear();
}

void ChartController::deleteSelectedItems() {
    QList<QGraphicsItem*> selected = m_scene->selectedItems();
    for (QGraphicsItem *item : selected) {
        if (item != m_chartImage && item != m_protractor && item != m_ruler && item != m_compass) {
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
        // Calcular los ángulos en grados para drawArc (16ths of degree)
        // Qt drawArc(x, y, w, h, startAngle, spanAngle)
        // startAngle es relativo a las 3 en punto (eje X positivo), en sentido antihorario (+).
        // Nuestros ángulos m_arcStartAngle y m_angle crecen clockwise en pantalla (Y hacia abajo), 
        // pero qAtan2 devuelve radians normales (antihorario matemático invertido por sistema coordenadas pantalla).
        // DraggableCompass::angleToPoint usa qAtan2(y, x).
        // En Qt Graphics View por defecto +Y es abajo. 
        // qAtan2(y,x) devuelve ángulo positivo horario si Y es abajo.
        // PERO drawArc usa grados matemáticos estándar (antihorario).
        
        // Para simplificar: usaremos path para visualización exacta.
        // Ocultar lógica anterior que fallaba
        // double start = -m_arcStartAngle; 
        // double span = -(m_angle - m_arcStartAngle);
        // painter->drawArc(arcRect, static_cast<int>(start * 16), static_cast<int>(span * 16));(latTop - latBottom);
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
    
    // Actualizar compás si está visible
    if (m_compass) {
        m_compass->setColor(color);
    }
    
    // Actualizar regla si está visible
    if (m_ruler) {
        m_ruler->setMarkerColor(color);
    }

    // Actualizar elementos seleccionados (Req 3.5)
    for (QGraphicsItem *item : m_scene->selectedItems()) {
        if (auto *line = qgraphicsitem_cast<QGraphicsLineItem*>(item)) {
            QPen p = line->pen();
            p.setColor(color);
            line->setPen(p);
        } else if (auto *path = qgraphicsitem_cast<QGraphicsPathItem*>(item)) {
            QPen p = path->pen();
            p.setColor(color);
            path->setPen(p);
        } else if (auto *ellipse = qgraphicsitem_cast<QGraphicsEllipseItem*>(item)) {
            QPen p = ellipse->pen();
            p.setColor(color);
            ellipse->setPen(p);
            ellipse->setBrush(QBrush(color));
        } else if (auto *text = qgraphicsitem_cast<QGraphicsTextItem*>(item)) {
            text->setDefaultTextColor(color);
        }
    }
}

void ChartController::setStrokeWidth(int width) {
    m_strokeWidth = qBound(1, width, 100);
    
    // Actualizar compás si está visible
    if (m_compass) {
        m_compass->setStrokeWidth(width);
    }

    // Actualizar elementos seleccionados (Req 3.5)
    for (QGraphicsItem *item : m_scene->selectedItems()) {
        if (auto *line = qgraphicsitem_cast<QGraphicsLineItem*>(item)) {
            QPen p = line->pen();
            p.setWidth(width);
            line->setPen(p);
        } else if (auto *path = qgraphicsitem_cast<QGraphicsPathItem*>(item)) {
            QPen p = path->pen();
            p.setWidth(width);
            path->setPen(p);
        } else if (auto *ellipse = qgraphicsitem_cast<QGraphicsEllipseItem*>(item)) {
            // Recalcular tamaño del punto
            QPen p = ellipse->pen();
            p.setWidth(width);
            ellipse->setPen(p);
            
            QRectF rect = ellipse->rect();
            QPointF center = rect.center();
            double radius = width * 1.5;
            ellipse->setRect(center.x() - radius, center.y() - radius, radius * 2, radius * 2);

        } else if (auto *text = qgraphicsitem_cast<QGraphicsTextItem*>(item)) {
            QFont f = text->font();
            f.setPixelSize(qMax(8, width)); // Usar width como tamaño de fuente aprox
            text->setFont(f);
        }
    }
}

void ChartController::setFont(const QFont &font) {
    m_font = font;

    // Actualizar elementos seleccionados (Req 3.5)
    for (QGraphicsItem *item : m_scene->selectedItems()) {
        if (auto *text = qgraphicsitem_cast<QGraphicsTextItem*>(item)) {
            QFont f = font;
            f.setPixelSize(text->font().pixelSize()); // Mantener tamaño actual
            text->setFont(f);
        }
    }
}

void ChartController::toggleProjectionsForSelected() {
    QList<QGraphicsItem*> selected = m_scene->selectedItems();
    if (selected.size() != 1) return;

    QGraphicsItem *item = selected.first();
    // Solo permitir proyecciones para Puntos (Elipses)
    auto *pointItem = qgraphicsitem_cast<QGraphicsEllipseItem*>(item);
    if (!pointItem) return;

    // Verificar si ya tiene proyecciones (buscamos hijos que sean líneas dashed)
    QList<QGraphicsItem*> children = pointItem->childItems();
    bool hasProjections = false;
    for (auto *child : children) {
        if (child->data(0).toString() == "projection") {
            hasProjections = true;
            // Borrar
            m_scene->removeItem(child);
            delete child;
        }
    }
    
    // Si tenía, ya las borramos y salimos (Toggle OFF)
    if (hasProjections) return;

    // Si no, las creamos (Toggle ON)
    QPointF center = pointItem->rect().center();
    QRectF sceneRect = m_scene->sceneRect();

    QPen pen(Qt::black, 2, Qt::DashLine);
    
    // Línea vertical (hacia abajo o arriba según convenga, aquí a los ejes bordes)
    // Asumiremos ejes a la izquierda y abajo o arriba.
    // Dibujamos línea horizontal hacia la izquierda (eje latitud típico)
    QGraphicsLineItem *hLine = new QGraphicsLineItem(sceneRect.left(), center.y(), center.x(), center.y(), pointItem);
    hLine->setPen(pen);
    hLine->setData(0, "projection");

    // Línea vertical hacia abajo (eje longitud típico)
    QGraphicsLineItem *vLine = new QGraphicsLineItem(center.x(), center.y(), center.x(), sceneRect.bottom(), pointItem);
    vLine->setPen(pen);
    vLine->setData(0, "projection");
}

// === HERRAMIENTAS OVERLAY (SVG) ===

void ChartController::showProtractor(bool visible) {
    if (visible && !m_protractor) {
        // Crear transportador SVG
        m_protractor = new DraggableProtractor();
        m_scene->addItem(m_protractor);
        
        // Posicionar en el centro de la carta
        QRectF sceneRect = m_scene->sceneRect();
        QRectF bounds = m_protractor->boundingRect();
        m_protractor->setPos(
            sceneRect.center().x() - bounds.width() / 2,
            sceneRect.center().y() - bounds.height() / 2
        );
        
        // Conectar señal de ángulo
        connect(m_protractor, &DraggableProtractor::angleChanged,
                this, &ChartController::angleChanged);
                
    } else if (!visible && m_protractor) {
        m_scene->removeItem(m_protractor);
        delete m_protractor;
        m_protractor = nullptr;
    }
}

void ChartController::showRuler(bool visible) {
    if (visible && !m_ruler) {
        // Crear regla SVG
        m_ruler = new DraggableRuler();
        m_scene->addItem(m_ruler);
        
        // Posicionar en el centro de la carta
        QRectF sceneRect = m_scene->sceneRect();
        QRectF bounds = m_ruler->boundingRect();
        m_ruler->setPos(
            sceneRect.center().x() - bounds.width() / 2,
            sceneRect.center().y() - bounds.height() / 2
        );
        
        m_ruler->setMarkerColor(m_strokeColor); // <--- COLOR INICIAL
        
        // Conectar señal de ángulo
        connect(m_ruler, &DraggableRuler::angleChanged,
                this, &ChartController::angleChanged);
                
        // Conectar señales de dibujo de segmento
        connect(m_ruler, &DraggableRuler::drawSegmentRequested,
                this, &ChartController::drawSegmentFromRuler);
                
        connect(m_ruler, &DraggableRuler::segmentPreviewRequested,
                this, &ChartController::drawSegmentPreview);
                
    } else if (!visible && m_ruler) {
        m_scene->removeItem(m_ruler);
        delete m_ruler;
        m_ruler = nullptr;
    }
}

// ... [omitido]

void ChartController::drawSegmentFromRuler(const QPointF &startPos, const QPointF &endPos) {
    // Si hay un preview, eliminarlo
    if (m_currentDrawingItem) {
        m_scene->removeItem(m_currentDrawingItem);
        delete m_currentDrawingItem;
        m_currentDrawingItem = nullptr;
    }
    
    // Crear la línea con el estilo actual
    QPen pen(m_strokeColor, m_strokeWidth);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    
    QGraphicsLineItem *line = m_scene->addLine(
        startPos.x(), startPos.y(),
        endPos.x(), endPos.y(),
        pen
    );
    
    line->setFlag(QGraphicsItem::ItemIsSelectable);
    m_undoStack.push(line);
    emit itemAdded(line);
}

void ChartController::drawSegmentPreview(const QPointF &startPos, const QPointF &endPos) {
    if (m_currentDrawingItem) {
        m_scene->removeItem(m_currentDrawingItem);
        delete m_currentDrawingItem;
    }
    
    // Línea de preview (puede ser un poco más fina o semi-transparente)
    QPen pen(m_strokeColor, m_strokeWidth);
    pen.setStyle(Qt::DashLine); // Estilo temporal
    
    QGraphicsLineItem *line = m_scene->addLine(
        startPos.x(), startPos.y(),
        endPos.x(), endPos.y(),
        pen
    );
    m_currentDrawingItem = line;
}

void ChartController::removeSegmentPreview() {
    if (m_currentDrawingItem) {
        m_scene->removeItem(m_currentDrawingItem);
        delete m_currentDrawingItem;
        m_currentDrawingItem = nullptr;
    }
}

void ChartController::rotateProtractor(double angleDelta) {
    if (m_protractor) {
        m_protractor->rotateBy(angleDelta);
    }
}

void ChartController::rotateRuler(double angleDelta) {
    if (m_ruler) {
        m_ruler->rotateBy(angleDelta);
    }
}

bool ChartController::isProtractorVisible() const {
    return m_protractor != nullptr;
}

bool ChartController::isRulerVisible() const {
    return m_ruler != nullptr;
}

double ChartController::protractorAngle() const {
    return m_protractor ? m_protractor->angle() : 0.0;
}

double ChartController::rulerAngle() const {
    return m_ruler ? m_ruler->angle() : 0.0;
}

void ChartController::showCompass(bool visible) {
    if (visible && !m_compass) {
        // Crear compás interactivo
        m_compass = new DraggableCompass();
        m_scene->addItem(m_compass);
        
        // Posicionar en el centro de la vista
        QRectF sceneRect = m_scene->sceneRect();
        m_compass->setPos(sceneRect.center());
        
        // Configurar estilos actuales
        m_compass->setColor(m_strokeColor);
        m_compass->setStrokeWidth(m_strokeWidth);
        
        // Conectar señales
        connect(m_compass, &DraggableCompass::arcCompleted,
                this, &ChartController::onArcCompleted);
        connect(m_compass, &DraggableCompass::angleChanged,
                this, &ChartController::angleChanged);
                
    } else if (!visible && m_compass) {
        m_scene->removeItem(m_compass);
        delete m_compass;
        m_compass = nullptr;
    }
}

bool ChartController::isCompassVisible() const {
    return m_compass != nullptr;
}

void ChartController::setCompassRadius(double radius) {
    if (m_compass) {
        m_compass->setRadius(radius);
    }
}

double ChartController::compassRadius() const {
    return m_compass ? m_compass->radius() : 100.0;
}

QGraphicsItem* ChartController::protractorItem() const {
    return m_protractor;
}

QGraphicsItem* ChartController::rulerItem() const {
    return m_ruler;
}

QGraphicsItem* ChartController::compassItem() const {
    return m_compass;
}

// (drawLineFromRuler eliminado)

void ChartController::onArcCompleted(const QPainterPath &path, const QColor &color, int width) {
    if (path.isEmpty()) return;
    
    QPen pen(color, width);
    pen.setCapStyle(Qt::RoundCap);
    
    QGraphicsPathItem *arcItem = m_scene->addPath(path, pen);
    arcItem->setFlag(QGraphicsItem::ItemIsSelectable);
    m_undoStack.push(arcItem);
    emit itemAdded(arcItem);
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
        
        // Configurar fuente con el tamaño seleccionado
        QFont f = m_font;
        f.setPixelSize(qMax(8, m_strokeWidth));
        textItem->setFont(f);
        
        textItem->setFlag(QGraphicsItem::ItemIsSelectable);
        textItem->setFlag(QGraphicsItem::ItemIsMovable);
        
        m_undoStack.push(textItem);
        emit itemAdded(textItem);
    }
}

void ChartController::eraseItemAt(const QPointF &pos) {
    QList<QGraphicsItem*> items = m_scene->items(pos);
    for (QGraphicsItem *item : items) {
        // Excluir la carta de fondo y las herramientas overlay
        if (item != m_chartImage && item != m_protractor && item != m_ruler && item != m_compass) {
            m_scene->removeItem(item);
            delete item;
            break;
        }
    }
}

QPointF ChartController::adjustForZoom(const QPointF &viewPos) const {
    return viewPos / m_zoomLevel;
}
