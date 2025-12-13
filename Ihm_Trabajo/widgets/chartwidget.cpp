#include "chartwidget.h"
#include "chartcontroller.h"
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QScrollBar>
#include <QApplication>

ChartWidget::ChartWidget(QWidget *parent)
    : QGraphicsView(parent)
    , m_scene(new QGraphicsScene(this))
    , m_controller(new ChartController(m_scene, this))
{
    setScene(m_scene);

    // Configuración visual premium
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);
    setRenderHint(QPainter::TextAntialiasing);
    
    // Configuración de interacción
    setDragMode(QGraphicsView::NoDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    
    // Scrollbars
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    // Estilo oscuro para las scrollbars
    setStyleSheet(R"(
        QGraphicsView {
            background-color: #0a0a14;
            border: none;
        }
        QScrollBar:vertical {
            background-color: #16213e;
            width: 12px;
            margin: 0;
        }
        QScrollBar::handle:vertical {
            background-color: #0f3460;
            border-radius: 6px;
            min-height: 30px;
        }
        QScrollBar::handle:vertical:hover {
            background-color: #e94560;
        }
        QScrollBar:horizontal {
            background-color: #16213e;
            height: 12px;
            margin: 0;
        }
        QScrollBar::handle:horizontal {
            background-color: #0f3460;
            border-radius: 6px;
            min-width: 30px;
        }
        QScrollBar::handle:horizontal:hover {
            background-color: #e94560;
        }
        QScrollBar::add-line, QScrollBar::sub-line {
            height: 0; width: 0;
        }
    )");

    // Permitir tracking del mouse para actualizar coordenadas
    setMouseTracking(true);
    
    // Focus para eventos de teclado
    setFocusPolicy(Qt::StrongFocus);

    // Conectar señal de zoom
    connect(m_controller, &ChartController::zoomChanged,
            this, &ChartWidget::onZoomChanged);
}

ChartWidget::~ChartWidget() {
}

void ChartWidget::mousePressEvent(QMouseEvent *event) {
    ToolMode tool = m_controller->currentTool();
    
    // Para herramientas overlay (Transportador/Regla), dejar que los items manejen el evento
    if (tool == ToolMode::Protractor || tool == ToolMode::Ruler) {
        // Verificar si el click está sobre un item de la escena
        QGraphicsItem *item = itemAt(event->pos());
        if (item) {
            // Dejar que QGraphicsView pase el evento al item
            QGraphicsView::mousePressEvent(event);
            return;
        }
    }
    
    if (tool == ToolMode::Pan) {
        // Iniciar panning
        m_isPanning = true;
        m_lastPanPoint = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
    } else {
        // Delegar al controller para dibujar
        QPointF scenePos = mapToScene(event->pos());
        m_controller->handleMousePressAt(scenePos, event->button());
        event->accept();
    }
}

void ChartWidget::mouseMoveEvent(QMouseEvent *event) {
    ToolMode tool = m_controller->currentTool();
    QPointF scenePos = mapToScene(event->pos());
    
    // Guardar posición del ratón para zoom centrado
    m_lastMousePos = event->pos();
    
    // Siempre informar coordenadas al controller
    m_controller->handleMouseMoveAt(scenePos);
    
    // Para herramientas overlay, dejar que los items manejen el drag
    if (tool == ToolMode::Protractor || tool == ToolMode::Ruler) {
        QGraphicsView::mouseMoveEvent(event);
        return;
    }

    if (m_isPanning) {
        // Realizar panning
        QPoint delta = event->pos() - m_lastPanPoint;
        m_lastPanPoint = event->pos();
        
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        
        event->accept();
    }
}

void ChartWidget::mouseReleaseEvent(QMouseEvent *event) {
    ToolMode tool = m_controller->currentTool();
    
    // Para herramientas overlay, dejar que los items manejen el release
    if (tool == ToolMode::Protractor || tool == ToolMode::Ruler) {
        QGraphicsView::mouseReleaseEvent(event);
        return;
    }
    
    if (m_isPanning) {
        m_isPanning = false;
        setCursor(Qt::OpenHandCursor);
        event->accept();
    } else {
        QPointF scenePos = mapToScene(event->pos());
        m_controller->handleMouseReleaseAt(scenePos, event->button());
        event->accept();
    }
}

void ChartWidget::wheelEvent(QWheelEvent *event) {
    ToolMode tool = m_controller->currentTool();
    
    // Si es transportador o regla, delegar rotación al controller
    if (tool == ToolMode::Protractor || tool == ToolMode::Ruler) {
        m_controller->handleWheelDelta(event->angleDelta().y());
        event->accept();
        return;
    }
    
    // Zoom centrado en la posición del ratón
    int delta = event->angleDelta().y();
    double scaleFactor = (delta > 0) ? 1.25 : 0.8;
    double newZoom = m_currentZoom * scaleFactor;
    newZoom = qBound(0.1, newZoom, 10.0);
    
    if (!qFuzzyCompare(newZoom, m_currentZoom)) {
        applyZoomAtPoint(newZoom, event->position().toPoint());
        m_controller->zoomTo(newZoom);
    }
    
    event->accept();
}

void ChartWidget::keyPressEvent(QKeyEvent *event) {
    // Atajos de teclado
    switch (event->key()) {
    case Qt::Key_Plus:
    case Qt::Key_Equal: {
        // Zoom in centrado en el ratón
        double newZoom = qBound(0.1, m_currentZoom * 1.25, 10.0);
        applyZoomAtPoint(newZoom, m_lastMousePos);
        m_controller->zoomTo(newZoom);
        break;
    }
    case Qt::Key_Minus: {
        // Zoom out centrado en el ratón
        double newZoom = qBound(0.1, m_currentZoom * 0.8, 10.0);
        applyZoomAtPoint(newZoom, m_lastMousePos);
        m_controller->zoomTo(newZoom);
        break;
    }
    case Qt::Key_0:
        applyZoomAtPoint(1.0, m_lastMousePos);
        m_controller->resetZoom();
        break;
    case Qt::Key_Z:
        if (event->modifiers() & Qt::ControlModifier) {
            m_controller->undoLastAction();
        }
        break;
    case Qt::Key_Delete:
    case Qt::Key_Backspace:
        m_controller->deleteSelectedItems();
        break;
    case Qt::Key_Escape:
        // Cancelar operación actual
        m_controller->setTool(ToolMode::Pan);
        break;
    default:
        QGraphicsView::keyPressEvent(event);
        return;
    }
    event->accept();
}

void ChartWidget::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    // Opcionalmente centrar la carta al redimensionar
}

void ChartWidget::onZoomChanged(double factor) {
    // Solo actualizar si el zoom cambió externamente (botones UI)
    if (!qFuzzyCompare(factor, m_currentZoom)) {
        applyZoomAtPoint(factor, m_lastMousePos);
    }
}

void ChartWidget::applyZoomAtPoint(double factor, const QPoint &viewPos) {
    // Obtener punto de la escena bajo el ratón antes del zoom
    QPointF scenePosBefore = mapToScene(viewPos);
    
    // Aplicar el nuevo zoom
    QTransform transform;
    transform.scale(factor, factor);
    setTransform(transform);
    m_currentZoom = factor;
    
    // Obtener punto de la escena bajo el ratón después del zoom
    QPointF scenePosAfter = mapToScene(viewPos);
    
    // Ajustar scroll para mantener el punto bajo el ratón
    QPointF delta = scenePosBefore - scenePosAfter;
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() + delta.x() * factor);
    verticalScrollBar()->setValue(verticalScrollBar()->value() + delta.y() * factor);
    
    // Actualizar cursor según herramienta
    ToolMode tool = m_controller->currentTool();
    if (tool == ToolMode::Pan) {
        setCursor(Qt::OpenHandCursor);
    }
}

void ChartWidget::centerOnChart() {
    if (scene() && !scene()->sceneRect().isEmpty()) {
        centerOn(scene()->sceneRect().center());
    }
}
