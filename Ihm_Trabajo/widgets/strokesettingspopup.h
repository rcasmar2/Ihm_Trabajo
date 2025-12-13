#pragma once

#include <QWidget>
#include <QColor>

class QSlider;
class QPushButton;
class QLabel;

/**
 * StrokeSettingsPopup - Panel flotante para configurar color y grosor de trazo
 * 
 * Aparece al seleccionar herramientas de dibujo (Punto, Línea, Arco, Texto)
 */
class StrokeSettingsPopup : public QWidget {
    Q_OBJECT

public:
    explicit StrokeSettingsPopup(QWidget *parent = nullptr);
    
    void setColor(const QColor &color);
    void setStrokeWidth(int width);
    
    QColor color() const { return m_color; }
    int strokeWidth() const { return m_strokeWidth; }
    
    void showNear(QWidget *anchor);

signals:
    void colorChanged(const QColor &color);
    void strokeWidthChanged(int width);

private slots:
    void onColorButtonClicked();
    void onWidthSliderChanged(int value);

private:
    QColor m_color = QColor("#e94560");
    int m_strokeWidth = 3;
    
    QPushButton *m_colorButton;
    QSlider *m_widthSlider;
    QLabel *m_widthLabel;
    QLabel *m_previewLabel;
    
    void updatePreview();
    void updateColorButtonStyle();
    void setupUI();
};
