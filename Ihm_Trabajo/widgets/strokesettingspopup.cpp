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
#include <QPainterPath>
#include <QEvent>
#include <QFontComboBox>
#include <QComboBox> // NECESARIO

StrokeSettingsPopup::StrokeSettingsPopup(QWidget *parent)
    : QWidget(parent, Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint)
{
    // Hacer el fondo de la ventana transparente para mostrar esquinas redondeadas
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground);
    
    setupUI();
}

void StrokeSettingsPopup::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 16, 20, 20);
    mainLayout->setSpacing(14);

    // === HEADER CON TÍTULO Y BOTÓN X ===
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setContentsMargins(0, 0, 0, 0);
    
    QLabel *titleLabel = new QLabel("Estilo de Trazo", this);
    titleLabel->setStyleSheet("color: #ffffff; font-size: 14px; font-weight: bold; background: transparent;");
    headerLayout->addWidget(titleLabel);
    
    headerLayout->addStretch();
    
    // Botón X para cerrar - elegante y bien centrado
    QPushButton *closeButton = new QPushButton(this);
    closeButton->setFixedSize(20, 20);
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setStyleSheet(R"(
        QPushButton {
            background-color: #1a1a1a;
            color: #666666;
            font-size: 14px;
            font-weight: bold;
            border: none;
            border-radius: 10px;
            padding-bottom: 2px;
        }
        QPushButton:hover {
            background-color: #e94560;
            color: #ffffff;
        }
    )");
    closeButton->setText(QString::fromUtf8("×"));
    connect(closeButton, &QPushButton::clicked, this, &QWidget::hide);
    headerLayout->addWidget(closeButton);
    
    mainLayout->addLayout(headerLayout);
    
    // === SEPARADOR ===
    QFrame *separator1 = new QFrame(this);
    separator1->setFrameShape(QFrame::HLine);
    separator1->setStyleSheet("background-color: #2a2a2a; max-height: 1px;");
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
                background: #1a1a1a;
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
            background-color: #1a1a1a;
            border: 1px solid #333333;
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

    // === SECCION FUENTE (solo modo texto) ===
    m_textOptionsWidget = new QWidget(this);
    QVBoxLayout *textLayout = new QVBoxLayout(m_textOptionsWidget);
    textLayout->setContentsMargins(0,0,0,0);
    textLayout->setSpacing(8);
    
    QLabel *fontLabel = new QLabel("FUENTE", m_textOptionsWidget);
    fontLabel->setStyleSheet("color: #7f8c8d; font-size: 11px; font-weight: bold; background: transparent; text-transform: uppercase;");
    textLayout->addWidget(fontLabel);

    m_fontCombo = new QFontComboBox(m_textOptionsWidget);
    m_fontCombo->setFixedHeight(30);
    // Estilo básico para que se vea bien en dark mode
    m_fontCombo->setStyleSheet("QComboBox { background: #1a1a1a; color: white; border: 1px solid #333; border-radius: 4px; padding: 4px; } QComboBox::drop-down { border: none; }");
    connect(m_fontCombo, &QFontComboBox::currentFontChanged, this, [this](const QFont &f){ 
        m_fontFamily = f.family(); 
        updatePreview();
    });
    textLayout->addWidget(m_fontCombo);
    
    // Botones estilo
    QHBoxLayout *styleLayout = new QHBoxLayout();
    styleLayout->setSpacing(8);
    
    auto createStyleBtn = [this](const QString &text) -> QPushButton* {
        QPushButton *btn = new QPushButton(text, this);
        btn->setCheckable(true);
        btn->setFixedSize(40, 30);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet(
            "QPushButton { background: #2a2a2a; color: #aaa; border: none; border-radius: 4px; font-weight: bold; font-family: sans-serif; }"
            "QPushButton:checked { background: #e94560; color: white; }"
            "QPushButton:hover:!checked { background: #333; }"
        );
        connect(btn, &QPushButton::toggled, this, &StrokeSettingsPopup::onFontStyleChanged);
        return btn;
    };
    
    m_boldBtn = createStyleBtn("B");
    m_italicBtn = createStyleBtn("I");
    m_strikeBtn = createStyleBtn("S");
    
    // Aplicar estilos visuales a los botones
    QFont f = m_boldBtn->font(); f.setBold(true); m_boldBtn->setFont(f);
    f = m_italicBtn->font(); f.setItalic(true); m_italicBtn->setFont(f);
    f = m_strikeBtn->font(); f.setStrikeOut(true); m_strikeBtn->setFont(f);

    styleLayout->addWidget(m_boldBtn);
    styleLayout->addWidget(m_italicBtn);
    styleLayout->addWidget(m_strikeBtn);
    styleLayout->addStretch();
    textLayout->addLayout(styleLayout);
    
    // Espacio extra
    textLayout->addSpacing(6);

    mainLayout->addWidget(m_textOptionsWidget);
    m_textOptionsWidget->setVisible(false); // Default hidden

    // === SECCIÓN GROSOR ===
    m_widthTitleLabel = new QLabel("Grosor", this);
    m_widthTitleLabel->setStyleSheet("color: #7f8c8d; font-size: 11px; font-weight: bold; background: transparent; text-transform: uppercase;");
    mainLayout->addWidget(m_widthTitleLabel);
    
    // Slider con valor
    QHBoxLayout *sliderLayout = new QHBoxLayout();
    sliderLayout->setSpacing(12);
    
    m_widthSlider = new QSlider(Qt::Horizontal, this);
    m_widthSlider->setRange(1, 15);
    m_widthSlider->setValue(m_strokeWidth);
    m_widthSlider->setStyleSheet(R"(
        QSlider::groove:horizontal {
            height: 6px;
            background: #1a1a1a;
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
    m_previewLabel->setStyleSheet("background-color: #1a1a1a; border-radius: 8px;");
    mainLayout->addWidget(m_previewLabel);

    updatePreview();
    
    // Botón Aceptar (NUEVO)
    QPushButton *applyButton = new QPushButton("Aceptar", this);
    applyButton->setCursor(Qt::PointingHandCursor);
    applyButton->setStyleSheet(R"(
        QPushButton {
            background-color: #27ae60;
            color: white;
            padding: 8px;
            border-radius: 6px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #2ecc71;
        }
    )");
    connect(applyButton, &QPushButton::clicked, this, &StrokeSettingsPopup::onApplyClicked);
    mainLayout->addWidget(applyButton);
    
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
    }
}

void StrokeSettingsPopup::setStrokeWidth(int width) {
    width = qBound(1, width, 100);
    if (m_strokeWidth != width) {
        m_strokeWidth = width;
        m_widthSlider->setValue(width);
        m_widthLabel->setText(QString::number(width) + " px");
        updatePreview();
    }
}

void StrokeSettingsPopup::showNear(QWidget *anchor) {
    if (anchor) {
        // Más separación de la barra lateral (30px en lugar de 10px)
        QPoint globalPos = anchor->mapToGlobal(QPoint(anchor->width() + 30, 0));
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
}

void StrokeSettingsPopup::onApplyClicked() {
    emit colorChanged(m_color);
    emit strokeWidthChanged(m_strokeWidth);
    
    if (m_isTextMode) {
        QFont f(m_fontFamily);
        f.setBold(m_isBold);
        f.setItalic(m_isItalic);
        f.setStrikeOut(m_isStrikeOut);
        f.setPixelSize(m_strokeWidth);
        emit fontChanged(f);
    }
    
    hide();
}

void StrokeSettingsPopup::updatePreview() {
    // Crear imagen de preview con fondo transparente
    int height = m_isTextMode ? qMax(40, m_strokeWidth + 10) : 40;
    // Asegurar que cabe
    if (height > 80) height = 80;
    
    QPixmap preview(240, height);
    preview.fill(Qt::transparent);
    m_previewLabel->setFixedHeight(height);
    
    QPainter painter(&preview);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    
    if (m_isTextMode) {
        QFont f(m_fontFamily);
        f.setPixelSize(m_strokeWidth); // Usar PixelSize para control exacto
        f.setBold(m_isBold);
        f.setItalic(m_isItalic);
        f.setStrikeOut(m_isStrikeOut);
        painter.setFont(f);
        painter.setPen(m_color);
        painter.drawText(preview.rect(), Qt::AlignCenter, "Abc");
    } else {
        QPen pen(m_color, m_strokeWidth);
        pen.setCapStyle(Qt::RoundCap);
        painter.setPen(pen);
        painter.drawLine(20, height/2, 220, height/2);
    }
    
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
        updatePreview();
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
        updatePreview();
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

void StrokeSettingsPopup::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Dibujar fondo negro con esquinas redondeadas
    QPainterPath path;
    path.addRoundedRect(rect(), 16, 16);
    
    // Fondo negro
    painter.fillPath(path, QColor("#0d0d0d"));
    
    // Borde gris oscuro
    painter.setPen(QPen(QColor("#2a2a2a"), 1));
    painter.drawPath(path);
}


bool StrokeSettingsPopup::event(QEvent *event) {
    return QWidget::event(event);
}

void StrokeSettingsPopup::setTextMode(bool enabled) {
    m_isTextMode = enabled;
    
    if (m_textOptionsWidget) {
        m_textOptionsWidget->setVisible(enabled);
    }

    if (enabled) {
        m_widthTitleLabel->setText("Tamaño Fuente");
        // Ajustamos el rango del slider para tamaño de fuente (ej. 8 a 72)
        m_widthSlider->setRange(8, 72);
        // Si el valor actual es muy bajo (grosor de línea), poner un valor por defecto legible
        if (m_strokeWidth < 8) {
            setStrokeWidth(20);
        }
    } else {
        m_widthTitleLabel->setText("Grosor");
        m_widthSlider->setRange(1, 20);
        // Si teníamos un tamaño de fuente grande, volver a un grosor razonable
        if (m_strokeWidth > 20) {
            setStrokeWidth(3);
        }
    }
    
    adjustSize();
    updatePreview();
}

void StrokeSettingsPopup::onFontStyleChanged() {
    m_isBold = m_boldBtn->isChecked();
    m_isItalic = m_italicBtn->isChecked();
    m_isStrikeOut = m_strikeBtn->isChecked();
    updatePreview();
}

