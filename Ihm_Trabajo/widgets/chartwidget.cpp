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
    
    if (tool == ToolMode::Pan) {
        // Iniciar panning
        m_isPanning = true;
        m_lastPanPoint = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
    } else {
        // Delegar al controller
        QPointF scenePos = mapToScene(event->pos());
        m_controller->handleMousePressAt(scenePos, event->button());
        event->accept();
    }
}

void ChartWidget::mouseMoveEvent(QMouseEvent *event) {
    QPointF scenePos = mapToScene(event->pos());
    
    // Siempre informar coordenadas al controller
    m_controller->handleMouseMoveAt(scenePos);

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
    // Zoom con la rueda del ratón
    m_controller->handleWheelDelta(event->angleDelta().y());
    event->accept();
}

void ChartWidget::keyPressEvent(QKeyEvent *event) {
    // Atajos de teclado
    switch (event->key()) {
    case Qt::Key_Plus:
    case Qt::Key_Equal:
        m_controller->zoomIn();
        break;
    case Qt::Key_Minus:
        m_controller->zoomOut();
        break;
    case Qt::Key_0:
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
    applyZoom(factor);
}

void ChartWidget::applyZoom(double factor) {
    // Resetear transformación y aplicar nuevo zoom
    QTransform transform;
    transform.scale(factor, factor);
    setTransform(transform);
    
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
