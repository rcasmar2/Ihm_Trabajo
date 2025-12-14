#include "strokesettingspopup.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QColorDialog>
#include <QPainter>
#include <QPalette>
#include <QGraphicsDropShadowEffect>
#include <QRegularExpressionValidator>

StrokeSettingsPopup::StrokeSettingsPopup(QWidget *parent)
    : QWidget(parent, Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setAutoFillBackground(true);
    
    // Sombra para el popup
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 100));
    shadow->setOffset(0, 4);
    setGraphicsEffect(shadow);
    
    setupUI();
}

void StrokeSettingsPopup::setupUI() {
    // Estilo del popup - azul marino premium
    setStyleSheet(R"(
        StrokeSettingsPopup {
            background-color: #16213e;
            border: 1px solid #0f3460;
            border-radius: 16px;
        }
    )");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(16);

    // === TÍTULO ===
    QLabel *titleLabel = new QLabel("✏️ Estilo de Trazo", this);
    titleLabel->setStyleSheet("color: #ffffff; font-size: 14px; font-weight: bold; background: transparent;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // === SEPARADOR ===
    QFrame *separator1 = new QFrame(this);
    separator1->setFrameShape(QFrame::HLine);
    separator1->setStyleSheet("background-color: #0f3460; max-height: 1px;");
    mainLayout->addWidget(separator1);

    // === SECCIÓN COLOR ===
    QLabel *colorLabel = new QLabel("Color", this);
    colorLabel->setStyleSheet("color: #7f8c8d; font-size: 11px; font-weight: bold; background: transparent; text-transform: uppercase;");
    mainLayout->addWidget(colorLabel);

    // === PERSONALIZADOR DE COLOR ===
    QHBoxLayout *colorCustomizerLayout = new QHBoxLayout();
    colorCustomizerLayout->setSpacing(16);
    
    // Botón de color seleccionado (izquierda)
    m_colorButton = new QPushButton(this);
    m_colorButton->setFixedSize(64, 64);
    m_colorButton->setCursor(Qt::PointingHandCursor);
    m_colorButton->setToolTip("Click para seleccionar color");
    updateColorButtonStyle();
    connect(m_colorButton, &QPushButton::clicked, this, &StrokeSettingsPopup::onColorButtonClicked);
    colorCustomizerLayout->addWidget(m_colorButton);
    
    // Panel de sliders RGBA y Hex (derecha)
    QVBoxLayout *rgbaLayout = new QVBoxLayout();
    rgbaLayout->setSpacing(4);
    
    // Estilo común para sliders RGBA
    auto createRgbaSlider = [this](const QString &labelText, const QString &color, QSlider *&slider, QLabel *&valueLabel) {
        QHBoxLayout *row = new QHBoxLayout();
        row->setSpacing(6);
        
        QLabel *label = new QLabel(labelText, this);
        label->setFixedWidth(12);
        label->setStyleSheet(QString("color: %1; font-size: 10px; font-weight: bold; background: transparent;").arg(color));
        row->addWidget(label);
        
        slider = new QSlider(Qt::Horizontal, this);
        slider->setRange(0, 255);
        slider->setFixedHeight(16);
        slider->setStyleSheet(QString(R"(
            QSlider::groove:horizontal {
                height: 4px;
                background: #0f3460;
                border-radius: 2px;
            }
            QSlider::handle:horizontal {
                background: %1;
                width: 12px;
                height: 12px;
                margin: -4px 0;
                border-radius: 6px;
            }
            QSlider::handle:horizontal:hover {
                background: %1;
                border: 1px solid #ffffff;
            }
            QSlider::sub-page:horizontal {
                background: %1;
                border-radius: 2px;
            }
        )").arg(color));
        connect(slider, &QSlider::valueChanged, this, &StrokeSettingsPopup::onRgbaSliderChanged);
        row->addWidget(slider);
        
        valueLabel = new QLabel("0", this);
        valueLabel->setFixedWidth(28);
        valueLabel->setStyleSheet(QString("color: %1; font-size: 10px; background: transparent;").arg(color));
        valueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        row->addWidget(valueLabel);
        
        return row;
    };
    
    rgbaLayout->addLayout(createRgbaSlider("R", "#ff6b6b", m_redSlider, m_redLabel));
    rgbaLayout->addLayout(createRgbaSlider("G", "#51cf66", m_greenSlider, m_greenLabel));
    rgbaLayout->addLayout(createRgbaSlider("B", "#339af0", m_blueSlider, m_blueLabel));
    rgbaLayout->addLayout(createRgbaSlider("A", "#868e96", m_alphaSlider, m_alphaLabel));
    
    // Campo Hex
    QHBoxLayout *hexRow = new QHBoxLayout();
    hexRow->setSpacing(6);
    
    QLabel *hexLabel = new QLabel("#", this);
    hexLabel->setFixedWidth(12);
    hexLabel->setStyleSheet("color: #ffffff; font-size: 11px; font-weight: bold; background: transparent;");
    hexRow->addWidget(hexLabel);
    
    m_hexEdit = new QLineEdit(this);
    m_hexEdit->setMaxLength(8);  // RRGGBBAA
    m_hexEdit->setPlaceholderText("RRGGBBAA");
    m_hexEdit->setStyleSheet(R"(
        QLineEdit {
            background-color: #0f3460;
            border: 1px solid #1a3a5c;
            border-radius: 4px;
            color: #ffffff;
            font-size: 11px;
            font-family: 'Consolas', monospace;
            padding: 2px 6px;
        }
        QLineEdit:focus {
            border-color: #e94560;
        }
    )");
    QRegularExpression hexRegex("[0-9A-Fa-f]{0,8}");
    m_hexEdit->setValidator(new QRegularExpressionValidator(hexRegex, this));
    connect(m_hexEdit, &QLineEdit::textEdited, this, &StrokeSettingsPopup::onHexEditChanged);
    hexRow->addWidget(m_hexEdit);
    
    rgbaLayout->addLayout(hexRow);
    
    colorCustomizerLayout->addLayout(rgbaLayout);
    mainLayout->addLayout(colorCustomizerLayout);
    
    // Inicializar sliders con el color actual
    updateRgbaSliders();
    updateHexEdit();
    
    // Espaciado extra
    mainLayout->addSpacing(4);
    
    // Label para colores rápidos
    QLabel *quickColorsLabel = new QLabel("Colores rápidos", this);
    quickColorsLabel->setStyleSheet("color: #5a6a7a; font-size: 10px; background: transparent;");
    mainLayout->addWidget(quickColorsLabel);

    // Grid de colores predefinidos
    QGridLayout *colorGrid = new QGridLayout();
    colorGrid->setSpacing(6);
    
    QStringList presetColors = {"#e94560", "#27ae60", "#3498db", "#f39c12", 
                                 "#9b59b6", "#1abc9c", "#ecf0f1", "#2c3e50"};
    int row = 0, col = 0;
    for (const QString &colorStr : presetColors) {
        QPushButton *preset = new QPushButton(this);
        preset->setFixedSize(28, 28);
        preset->setCursor(Qt::PointingHandCursor);
        preset->setStyleSheet(QString(
            "QPushButton {"
            "  background-color: %1;"
            "  border-radius: 6px;"
            "  border: 2px solid transparent;"
            "}"
            "QPushButton:hover {"
            "  border-color: #ffffff;"
            "}"
        ).arg(colorStr));
        connect(preset, &QPushButton::clicked, this, [this, colorStr]() {
            setColor(QColor(colorStr));
        });
        colorGrid->addWidget(preset, row, col);
        col++;
        if (col > 3) {
            col = 0;
            row++;
        }
    }
    mainLayout->addLayout(colorGrid);

    // === SECCIÓN GROSOR ===
    QLabel *widthLabel = new QLabel("Grosor", this);
    widthLabel->setStyleSheet("color: #7f8c8d; font-size: 11px; font-weight: bold; background: transparent; text-transform: uppercase;");
    mainLayout->addWidget(widthLabel);
    
    // Slider con valor
    QHBoxLayout *sliderLayout = new QHBoxLayout();
    sliderLayout->setSpacing(12);
    
    m_widthSlider = new QSlider(Qt::Horizontal, this);
    m_widthSlider->setRange(1, 15);
    m_widthSlider->setValue(m_strokeWidth);
    m_widthSlider->setStyleSheet(R"(
        QSlider::groove:horizontal {
            height: 6px;
            background: #0f3460;
            border-radius: 3px;
        }
        QSlider::handle:horizontal {
            background: #e94560;
            width: 20px;
            height: 20px;
            margin: -7px 0;
            border-radius: 10px;
        }
        QSlider::handle:horizontal:hover {
            background: #ff6b6b;
        }
        QSlider::sub-page:horizontal {
            background: #e94560;
            border-radius: 3px;
        }
    )");
    connect(m_widthSlider, &QSlider::valueChanged, this, &StrokeSettingsPopup::onWidthSliderChanged);
    sliderLayout->addWidget(m_widthSlider);
    
    m_widthLabel = new QLabel(QString::number(m_strokeWidth) + " px", this);
    m_widthLabel->setFixedWidth(45);
    m_widthLabel->setStyleSheet("color: #e94560; font-size: 13px; font-weight: bold; background: transparent;");
    m_widthLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    sliderLayout->addWidget(m_widthLabel);
    
    mainLayout->addLayout(sliderLayout);

    // === PREVIEW ===
    QLabel *previewLabel = new QLabel("Vista previa", this);
    previewLabel->setStyleSheet("color: #7f8c8d; font-size: 11px; font-weight: bold; background: transparent; text-transform: uppercase;");
    mainLayout->addWidget(previewLabel);
    
    m_previewLabel = new QLabel(this);
    m_previewLabel->setFixedHeight(40);
    m_previewLabel->setAlignment(Qt::AlignCenter);
    m_previewLabel->setStyleSheet("background-color: #0f3460; border-radius: 8px;");
    mainLayout->addWidget(m_previewLabel);

    updatePreview();
    
    setFixedWidth(340);
    adjustSize();
}

void StrokeSettingsPopup::updateColorButtonStyle() {
    m_colorButton->setStyleSheet(QString(
        "QPushButton {"
        "  background-color: %1;"
        "  border-radius: 12px;"
        "  border: 3px solid #ffffff;"
        "}"
        "QPushButton:hover {"
        "  border-color: #e94560;"
        "}"
    ).arg(m_color.name()));
}

void StrokeSettingsPopup::setColor(const QColor &color) {
    if (m_color != color) {
        m_color = color;
        updateColorButtonStyle();
        updateRgbaSliders();
        updateHexEdit();
        updatePreview();
        emit colorChanged(m_color);
    }
}

void StrokeSettingsPopup::setStrokeWidth(int width) {
    width = qBound(1, width, 15);
    if (m_strokeWidth != width) {
        m_strokeWidth = width;
        m_widthSlider->setValue(width);
        m_widthLabel->setText(QString::number(width) + " px");
        updatePreview();
        emit strokeWidthChanged(m_strokeWidth);
    }
}

void StrokeSettingsPopup::showNear(QWidget *anchor) {
    if (anchor) {
        QPoint globalPos = anchor->mapToGlobal(QPoint(anchor->width() + 10, 0));
        move(globalPos);
    }
    show();
    raise();
}

void StrokeSettingsPopup::onColorButtonClicked() {
    QColor newColor = QColorDialog::getColor(m_color, this, "Seleccionar Color");
    if (newColor.isValid()) {
        setColor(newColor);
    }
}

void StrokeSettingsPopup::onWidthSliderChanged(int value) {
    m_strokeWidth = value;
    m_widthLabel->setText(QString::number(value) + " px");
    updatePreview();
    emit strokeWidthChanged(value);
}

void StrokeSettingsPopup::updatePreview() {
    // Crear imagen de preview con fondo transparente
    QPixmap preview(200, 30);
    preview.fill(Qt::transparent);
    
    QPainter painter(&preview);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QPen pen(m_color, m_strokeWidth);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);
    painter.drawLine(20, 15, 180, 15);
    
    m_previewLabel->setPixmap(preview);
}

void StrokeSettingsPopup::onRgbaSliderChanged() {
    if (m_updatingFromHex) return;
    
    m_updatingFromSliders = true;
    
    int r = m_redSlider->value();
    int g = m_greenSlider->value();
    int b = m_blueSlider->value();
    int a = m_alphaSlider->value();
    
    // Actualizar labels
    m_redLabel->setText(QString::number(r));
    m_greenLabel->setText(QString::number(g));
    m_blueLabel->setText(QString::number(b));
    m_alphaLabel->setText(QString::number(a));
    
    // Actualizar color
    QColor newColor(r, g, b, a);
    if (m_color != newColor) {
        m_color = newColor;
        updateColorButtonStyle();
        updateHexEdit();
        updatePreview();
        emit colorChanged(m_color);
    }
    
    m_updatingFromSliders = false;
}

void StrokeSettingsPopup::onHexEditChanged() {
    if (m_updatingFromSliders) return;
    
    QString hex = m_hexEdit->text().toUpper();
    if (hex.length() < 6) return;  // Mínimo RRGGBB
    
    m_updatingFromHex = true;
    
    bool ok;
    int r = hex.mid(0, 2).toInt(&ok, 16);
    if (!ok) { m_updatingFromHex = false; return; }
    
    int g = hex.mid(2, 2).toInt(&ok, 16);
    if (!ok) { m_updatingFromHex = false; return; }
    
    int b = hex.mid(4, 2).toInt(&ok, 16);
    if (!ok) { m_updatingFromHex = false; return; }
    
    int a = 255;
    if (hex.length() >= 8) {
        a = hex.mid(6, 2).toInt(&ok, 16);
        if (!ok) a = 255;
    }
    
    QColor newColor(r, g, b, a);
    if (m_color != newColor) {
        m_color = newColor;
        updateColorButtonStyle();
        updateRgbaSliders();
        updatePreview();
        emit colorChanged(m_color);
    }
    
    m_updatingFromHex = false;
}

void StrokeSettingsPopup::updateRgbaSliders() {
    // Bloquear señales para evitar recursión
    m_redSlider->blockSignals(true);
    m_greenSlider->blockSignals(true);
    m_blueSlider->blockSignals(true);
    m_alphaSlider->blockSignals(true);
    
    m_redSlider->setValue(m_color.red());
    m_greenSlider->setValue(m_color.green());
    m_blueSlider->setValue(m_color.blue());
    m_alphaSlider->setValue(m_color.alpha());
    
    m_redLabel->setText(QString::number(m_color.red()));
    m_greenLabel->setText(QString::number(m_color.green()));
    m_blueLabel->setText(QString::number(m_color.blue()));
    m_alphaLabel->setText(QString::number(m_color.alpha()));
    
    m_redSlider->blockSignals(false);
    m_greenSlider->blockSignals(false);
    m_blueSlider->blockSignals(false);
    m_alphaSlider->blockSignals(false);
}

void StrokeSettingsPopup::updateHexEdit() {
    if (m_updatingFromHex) return;
    
    QString hex = QString("%1%2%3%4")
        .arg(m_color.red(), 2, 16, QChar('0'))
        .arg(m_color.green(), 2, 16, QChar('0'))
        .arg(m_color.blue(), 2, 16, QChar('0'))
        .arg(m_color.alpha(), 2, 16, QChar('0'));
    
    m_hexEdit->setText(hex.toUpper());
}
