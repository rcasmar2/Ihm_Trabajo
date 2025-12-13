#pragma once

#include <QGraphicsView>
#include <QPointF>

class ChartController;
class QGraphicsScene;

/**
 * ChartWidget - QGraphicsView personalizado para la carta náutica
 * 
 * Implementa un sistema de tres capas:
 * - Capa 1 (Fondo): Imagen de la carta náutica
 * - Capa 2 (Dibujo): Elementos dibujados (líneas, puntos, arcos)
 * - Capa 3 (Herramientas): Overlay de herramientas (transportador, regla)
 */
class ChartWidget : public QGraphicsView {
    Q_OBJECT

public:
    explicit ChartWidget(QWidget *parent = nullptr);
    ~ChartWidget();

    ChartController* controller() const { return m_controller; }
    QGraphicsScene* chartScene() const { return m_scene; }

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QGraphicsScene *m_scene;
    ChartController *m_controller;
    
    // Para panning
    bool m_isPanning = false;
    QPoint m_lastPanPoint;

    void applyZoom(double factor);
    void centerOnChart();

private slots:
    void onZoomChanged(double factor);
};
