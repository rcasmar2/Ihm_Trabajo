#include "chartwidget.h"
#include "chartcontroller.h"
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QScrollBar>
#include <QApplication>
#include <QFrame>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QIcon>

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
    
    // Estilo oscuro - el mapa ocupa todo, las barras flotan encima
    setStyleSheet(R"(
        QGraphicsView {
            background-color: #0a0a0a;
            border: none;
        }
        QScrollBar:vertical {
            background-color: rgba(0, 0, 0, 0.5);
            width: 10px;
            margin: 0;
            border-radius: 5px;
        }
        QScrollBar::handle:vertical {
            background-color: rgba(255, 255, 255, 0.3);
            border-radius: 5px;
            min-height: 30px;
        }
        QScrollBar::handle:vertical:hover {
            background-color: #e94560;
        }
        QScrollBar:horizontal {
            background-color: rgba(0, 0, 0, 0.5);
            height: 10px;
            margin: 0;
            border-radius: 5px;
        }
        QScrollBar::handle:horizontal {
            background-color: rgba(255, 255, 255, 0.3);
            border-radius: 5px;
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
    
    // Crear indicador de herramienta
    setupToolIndicator();
}

ChartWidget::~ChartWidget() {
}

void ChartWidget::mousePressEvent(QMouseEvent *event) {
    ToolMode tool = m_controller->currentTool();
    
    // Para herramientas overlay (Transportador/Regla/Línea/Compás)
    if (tool == ToolMode::Protractor || tool == ToolMode::Ruler || 
        tool == ToolMode::Line || tool == ToolMode::Arc) {
        
        // Verificar si el click está sobre la herramienta overlay
        QGraphicsItem *item = itemAt(event->pos());
        QGraphicsItem *topItem = item ? item->topLevelItem() : nullptr;
        
        // Si estamos sobre la herramienta (o uno de sus hijos, como el marcador), 
        // dejar que ella maneje el evento
        if (topItem && (topItem == m_controller->protractorItem() || 
                        topItem == m_controller->rulerItem() || 
                        topItem == m_controller->compassItem())) {
            QGraphicsView::mousePressEvent(event);
            return;
        }
        
        // (Eliminado dibujo con clic derecho externo por petición del usuario)

        
        // Si NO estamos sobre la herramienta, permitir pan/moverse con clic izquierdo
        if (event->button() == Qt::LeftButton) {
            m_isPanning = true;
            m_lastPanPoint = event->pos();
            setCursor(Qt::ClosedHandCursor);
            event->accept();
            return;
        }
    }
    
    if (tool == ToolMode::Pan) {
        // Iniciar panning
        m_isPanning = true;
        m_lastPanPoint = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
    } else if (tool == ToolMode::Select) {
        // Modo Selección: comportamiento estándar de QGraphicsView
        QGraphicsView::mousePressEvent(event);
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
    
    // Para herramientas overlay: si estamos panning, hacer pan; si no, delegar
    if (tool == ToolMode::Protractor || tool == ToolMode::Ruler || 
        tool == ToolMode::Line || tool == ToolMode::Arc) {
        if (m_isPanning) {
            // Hacer pan aunque haya herramienta overlay
            QPoint delta = event->pos() - m_lastPanPoint;
            m_lastPanPoint = event->pos();
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
            verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
            event->accept();
            return;
        }
        // Si no estamos panning, dejar que los items manejen el drag
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
    
    // Primero manejar pan (puede estar activo incluso con herramientas overlay)
    if (m_isPanning) {
        m_isPanning = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    
    // Para herramientas overlay, dejar que los items manejen el release
    if (tool == ToolMode::Protractor || tool == ToolMode::Ruler || 
        tool == ToolMode::Line || tool == ToolMode::Arc) {
        QGraphicsView::mouseReleaseEvent(event);
        return;
    }
    
    QPointF scenePos = mapToScene(event->pos());
    m_controller->handleMouseReleaseAt(scenePos, event->button());
    event->accept();
}

void ChartWidget::wheelEvent(QWheelEvent *event) {
    ToolMode tool = m_controller->currentTool();
    
    // Ctrl + Rueda = rotar herramienta (transportador/regla/línea)
    if (event->modifiers() & Qt::ControlModifier) {
        if (tool == ToolMode::Protractor || tool == ToolMode::Ruler || tool == ToolMode::Line) {
            m_controller->handleWheelDelta(event->angleDelta().y());
            event->accept();
            return;
        }
    }
    
    // Rueda normal = Zoom (siempre)
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
    // Para el compás, pasar eventos de teclado al ítem con foco
    if (event->key() == Qt::Key_Space) {
        QGraphicsView::keyPressEvent(event);
        return;
    }
    
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

void ChartWidget::keyReleaseEvent(QKeyEvent *event) {
    // Pasar eventos de teclado a los items (para el compás)
    if (event->key() == Qt::Key_Space) {
        QGraphicsView::keyReleaseEvent(event);
        return;
    }
    QGraphicsView::keyReleaseEvent(event);
}

void ChartWidget::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    
    // Reposicionar el indicador de herramienta en la esquina derecha
    if (m_toolIndicator) {
        m_toolIndicator->move(width() - m_toolIndicator->width() - 15, 15);
    }
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

void ChartWidget::setupToolIndicator() {
    // Crear el frame del indicador
    m_toolIndicator = new QFrame(this);
    m_toolIndicator->setStyleSheet(R"(
        QFrame {
            background-color: #0d0d0d;
            border: none;
            border-radius: 10px;
        }
    )");
    
    QHBoxLayout *layout = new QHBoxLayout(m_toolIndicator);
    layout->setContentsMargins(14, 8, 14, 8);
    layout->setSpacing(10);
    
    // Etiqueta de herramienta (sin emoji)
    m_toolLabel = new QLabel("Selecciona", m_toolIndicator);
    m_toolLabel->setStyleSheet("color: #ffffff; font-weight: bold; font-size: 13px; background: transparent;");
    m_toolLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_toolLabel);
    
    // --- CONTENEDOR DE PROPIEDADES DE DIBUJO ---
    QWidget *propsWidget = new QWidget(m_toolIndicator);
    propsWidget->setObjectName("propsWidget");
    propsWidget->setStyleSheet("background: transparent;");
    QHBoxLayout *propsLayout = new QHBoxLayout(propsWidget);
    propsLayout->setContentsMargins(0, 0, 0, 0);
    propsLayout->setSpacing(10);
    
    // Separador
    QFrame *sep = new QFrame(propsWidget);
    sep->setFrameShape(QFrame::VLine);
    sep->setStyleSheet("background-color: #333333; max-width: 1px; border: none;");
    propsLayout->addWidget(sep);
    
    // Preview de color (sin borde)
    m_toolColorPreview = new QLabel(propsWidget);
    m_toolColorPreview->setFixedSize(18, 18);
    m_toolColorPreview->setStyleSheet("background-color: #e94560; border-radius: 4px;");
    propsLayout->addWidget(m_toolColorPreview);
    
    // Label de grosor
    m_toolWidthLabel = new QLabel("3px", propsWidget);
    m_toolWidthLabel->setStyleSheet("color: #888888; font-size: 12px; background: transparent;");
    propsLayout->addWidget(m_toolWidthLabel);
    
    layout->addWidget(propsWidget);
    
    // --- ICONO DE INFORMACIÓN (i) ---
    m_helpButton = new QPushButton(m_toolIndicator);
    m_helpButton->setFixedSize(24, 24);
    m_helpButton->setCursor(Qt::PointingHandCursor);
    m_helpButton->setIcon(QIcon(":/resources/icons/info_icon.svg"));
    m_helpButton->setIconSize(QSize(20, 20));
    m_helpButton->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            border: none;
        }
        QPushButton:hover {
            background-color: rgba(52, 152, 219, 0.3);
            border-radius: 12px;
        }
    )");
    connect(m_helpButton, &QPushButton::clicked, this, &ChartWidget::showToolHelp);
    layout->addWidget(m_helpButton);
    
    // Posicionar el indicador en la esquina derecha (posición inicial)
    m_toolIndicator->adjustSize();
    // Inicialmente posicionarlo a la izquierda, se reposicionará en resizeEvent
    m_toolIndicator->move(15, 15);
    m_toolIndicator->raise();
}

void ChartWidget::updateToolIndicator(const QString &toolName, const QColor &color, int width, bool isDrawingTool) {
    if (!m_toolIndicator) return;
    
    // Actualizar nombre
    if (!toolName.isEmpty()) {
        m_toolLabel->setVisible(true); // Asegurar visibilidad
        m_toolLabel->setText(toolName);
        m_currentToolName = toolName;  // Guardar para el botón de ayuda
    }
    
    // Gestionar visibilidad del panel de propiedades
    QWidget *props = m_toolIndicator->findChild<QWidget*>("propsWidget");
    if (props) {
        props->setVisible(isDrawingTool);
    }
    
    // Actualizar valores si es herramienta de dibujo
    if (isDrawingTool && props) {
        m_toolColorPreview->setStyleSheet(QString(
            "background-color: %1; border-radius: 4px;"
        ).arg(color.name()));
        m_toolWidthLabel->setText(QString("%1px").arg(width));
    }
    
    // Ajustar tamaño del layout y reposicionar en esquina derecha
    m_toolIndicator->adjustSize();
    int viewWidth = this->width();
    m_toolIndicator->move(viewWidth - m_toolIndicator->width() - 15, 15);
    m_toolIndicator->show();
    m_toolIndicator->raise();
    m_toolIndicator->update();
}

void ChartWidget::showToolHelp() {
    QString helpText = getToolHelpText(m_currentToolName);
    
    QMessageBox helpBox(this);
    helpBox.setWindowTitle("Ayuda: " + m_currentToolName);
    helpBox.setText(helpText);
    helpBox.setIcon(QMessageBox::Information);
    helpBox.setStyleSheet(R"(
        QMessageBox {
            background-color: #1a1a1a;
        }
        QMessageBox QLabel {
            color: #ffffff;
            font-size: 13px;
        }
        QPushButton {
            background-color: #3498db;
            color: white;
            padding: 8px 20px;
            border-radius: 6px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #2980b9;
        }
    )");
    helpBox.exec();
}

QString ChartWidget::getToolHelpText(const QString &toolName) {
    if (toolName == "Moverse") {
        return "<b>Herramienta Mover (Mano)</b><br><br>"
               "• <b>Arrastrar:</b> Mueve la carta náutica<br>"
               "• <b>Rueda del ratón:</b> Zoom in/out<br>"
               "• <b>+/-:</b> Zoom con teclado<br>"
               "• <b>0:</b> Resetear zoom";
    }
    else if (toolName == "Selecciona") {
        return "<b>Herramienta Seleccionar</b><br><br>"
               "• <b>Clic:</b> Selecciona un elemento<br>"
               "• <b>Clic en vacío:</b> Deselecciona<br>"
               "• <b>Delete/Supr:</b> Borra el elemento seleccionado<br>"
               "• Al seleccionar aparece un popup para editar propiedades";
    }
    else if (toolName == "Punto") {
        return "<b>Herramienta Punto</b><br><br>"
               "• <b>Clic:</b> Coloca un punto en la carta<br>"
               "• Usa el popup central para cambiar color y tamaño<br>"
               "• <b>Mostrar Proyecciones:</b> Dibuja líneas hasta los ejes";
    }
    else if (toolName == "Línea") {
        return "<b>Herramienta Línea</b><br><br>"
               "1. <b>Posiciona:</b> Arrastra para mover<br>"
               "2. <b>Rota:</b> Arrastra el icono ↻, mueve extremos o <b>Ctrl + Rueda</b><br>"
               "3. <b>Dibuja:</b> Clic derecho + Arrastrar el marcador rojo";
    }
    else if (toolName == "Arco") {
        return "<b>Herramienta Compás/Arco</b><br><br>"
               "• <b>Shift + Arrastrar:</b> Mover el compás<br>"
               "• <b>Arrastrar:</b> Rotar el compás<br>"
               "• <b>Clic derecho + Arrastrar:</b> Dibujar arco<br>"
               "• Usa el slider de Radio en el popup para ajustar tamaño";
    }
    else if (toolName == "Texto") {
        return "<b>Herramienta Texto</b><br><br>"
               "• <b>Clic:</b> Coloca texto en ese punto<br>"
               "• Aparecerá un cuadro para escribir el texto<br>"
               "• Usa el popup para cambiar fuente, tamaño y estilo (negrita, cursiva, tachado)";
    }
    else if (toolName == "Borrador") {
        return "<b>Herramienta Borrador</b><br><br>"
               "• <b>Clic sobre un elemento:</b> Lo borra<br>"
               "• Funciona con puntos, líneas, arcos y textos";
    }
    else if (toolName == "Transportador") {
        return "<b>Herramienta Transportador</b><br><br>"
               "• <b>Arrastrar centro:</b> Mover<br>"
               "• <b>Rota:</b> Arrastra el icono ↻, arrastrar borde o <b>Ctrl + Rueda</b><br>"
               "• El ángulo actual se muestra en pantalla";
    }
    else if (toolName == "Regla") {
        return "<b>Herramienta Regla</b><br><br>"
               "• <b>Arrastrar centro:</b> Mover la regla<br>"
               "• <b>Rota:</b> Arrastra el icono ↻, extremos o <b>Ctrl + Rueda</b><br>"
               "• Usa los bordes para medir distancias";
    }
    else {
        return "<b>" + toolName + "</b><br><br>"
               "Selecciona una herramienta para ver instrucciones de uso.";
    }
}
