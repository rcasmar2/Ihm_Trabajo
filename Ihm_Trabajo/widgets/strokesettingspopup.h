#pragma once

#include <QWidget>
#include <QColor>
#include <QFontComboBox> // Include completo en vez de fwd decl

class QSlider;
class QPushButton;
class QLabel;
class QLineEdit;

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

// ...
signals:
    void colorChanged(const QColor &color);
    void strokeWidthChanged(int width);
    void fontChanged(const QFont &font);
    void toggleProjectionsRequested(); // NUEVO


protected slots:
    void onFontStyleChanged();

private slots:
    void onColorButtonClicked();
    void onWidthSliderChanged(int value);
    void onRgbaSliderChanged();
    void onHexEditChanged();
    void onApplyClicked();

private:
    QColor m_color = QColor("#e94560");
    int m_strokeWidth = 3;
    
    // Font State
    bool m_isBold = false;
    bool m_isItalic = false;
    bool m_isStrikeOut = false;
    QString m_fontFamily = "Arial";
    
    QPushButton *m_colorButton;
    QSlider *m_widthSlider;
    QLabel *m_widthLabel;
    QLabel *m_previewLabel;
    
    // Text Controls
    QWidget *m_textOptionsWidget = nullptr;
    QFontComboBox *m_fontCombo = nullptr;
    QPushButton *m_boldBtn = nullptr;
    QPushButton *m_italicBtn = nullptr;
    QPushButton *m_strikeBtn = nullptr;
    
    // RGBA Sliders
    QSlider *m_redSlider;
    QSlider *m_greenSlider;
    QSlider *m_blueSlider;
    QSlider *m_alphaSlider;
    QLabel *m_redLabel;
    QLabel *m_greenLabel;
    QLabel *m_blueLabel;
    QLabel *m_alphaLabel;
    
    // Hex input
    QLineEdit *m_hexEdit;
    
    // Text Mode
    bool m_isTextMode = false;
    QLabel *m_widthTitleLabel; // "GROSOR" o "TAMAÑO FUENTE"
    
public:
    void setTextMode(bool enabled);
    void setPointMode(bool enabled);
    void setArcMode(bool enabled);  // NUEVO
    void setRadius(double radius);  // NUEVO
    double radius() const { return m_radius; }  // NUEVO

signals:
    void radiusChanged(double radius);  // NUEVO

private slots:
    void onRadiusSliderChanged(int value);  // NUEVO

private:    

    bool m_updatingFromSliders = false;
    bool m_updatingFromHex = false;
    
    // Point Mode
    QWidget *m_pointOptionsWidget = nullptr;
    QPushButton *m_projectionsBtn = nullptr;
    
    // Arc Mode
    QWidget *m_arcOptionsWidget = nullptr;  // NUEVO
    QSlider *m_radiusSlider = nullptr;      // NUEVO
    QLabel *m_radiusLabel = nullptr;        // NUEVO
    double m_radius = 100.0;                // NUEVO
    
    void updatePreview();
    void updateColorButtonStyle();
    void updateRgbaSliders();
    void updateHexEdit();
    void setupUI();
    
protected:
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *event) override;
};
