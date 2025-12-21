#pragma once

#include <QGraphicsSvgItem>
#include <QObject>

class QTimer;

class RotationHandle : public QGraphicsSvgItem {
    Q_OBJECT

public:
    explicit RotationHandle(QGraphicsItem *parent = nullptr);
    
signals:
    void dragStarted(const QPointF &pos);
    void dragged(const QPointF &pos);
    void dragFinished();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    bool m_isDragging = false;
};
