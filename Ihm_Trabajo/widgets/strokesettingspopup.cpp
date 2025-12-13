#include "strokesettingspopup.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QColorDialog>
#include <QPainter>
#include <QPalette>
#include <QGraphicsDropShadowEffect>

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

    // Color seleccionado (separado)
    QHBoxLayout *selectedColorLayout = new QHBoxLayout();
    selectedColorLayout->setAlignment(Qt::AlignLeft);
    
    m_colorButton = new QPushButton(this);
    m_colorButton->setFixedSize(52, 52);
    m_colorButton->setCursor(Qt::PointingHandCursor);
    updateColorButtonStyle();
    connect(m_colorButton, &QPushButton::clicked, this, &StrokeSettingsPopup::onColorButtonClicked);
    selectedColorLayout->addWidget(m_colorButton);
    selectedColorLayout->addStretch();
    mainLayout->addLayout(selectedColorLayout);
    
    // Espaciado extra
    mainLayout->addSpacing(8);
    
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
    
    setFixedWidth(240);
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
