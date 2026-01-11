#include "EditToolbar.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QLinearGradient>

EditToolbar::EditToolbar(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setFixedWidth(80);
    
    setStyleSheet(R"(
        QToolButton {
            background: rgba(40, 40, 50, 150);
            border: 1px solid rgba(255, 255, 255, 20);
            border-radius: 8px;
            color: white;
            font-size: 24px;
            padding: 8px;
            margin: 5px;
        }
        QToolButton:hover {
            background: rgba(60, 60, 70, 180);
            border: 1px solid rgba(255, 255, 255, 40);
        }
        QToolButton:checked {
            background: rgba(42, 130, 218, 200);
            border: 1px solid rgba(42, 130, 218, 255);
        }
    )");
}

void EditToolbar::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 20, 10, 10);
    layout->setSpacing(10);
    
    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->setExclusive(true);
    
    // Add tool buttons
    m_toolButtons.append(createToolButton("🖱", "Select Tool", Select));
    m_toolButtons.append(createToolButton("📝", "Text Tool", Text));
    m_toolButtons.append(createToolButton("🖍", "Highlight Tool", Highlight));
    m_toolButtons.append(createToolButton("✏️", "Drawing Tool", Drawing));
    m_toolButtons.append(createToolButton("⬜", "Shape Tool", Shape));
    m_toolButtons.append(createToolButton("🖼", "Image Tool", Image));
    m_toolButtons.append(createToolButton("💬", "Annotation Tool", Annotation));
    m_toolButtons.append(createToolButton("🧹", "Eraser Tool", Eraser));
    
    for (auto btn : m_toolButtons) {
        layout->addWidget(btn);
        m_buttonGroup->addButton(btn);
    }
    
    layout->addStretch();
    
    // Select first tool by default
    m_toolButtons.first()->setChecked(true);
    
    connect(m_buttonGroup, QOverload<int>::of(&QButtonGroup::idClicked),
            this, &EditToolbar::toolChanged);
}

QToolButton* EditToolbar::createToolButton(const QString &icon, const QString &tooltip, Tool tool) {
    QToolButton *button = new QToolButton(this);
    button->setText(icon);
    button->setToolTip(tooltip);
    button->setCheckable(true);
    button->setFixedSize(60, 60);
    button->setCursor(Qt::PointingHandCursor);
    m_buttonGroup->setId(button, static_cast<int>(tool));
    
    return button;
}

void EditToolbar::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw glass effect background
    QLinearGradient gradient(0, 0, width(), 0);
    gradient.setColorAt(0, QColor(30, 30, 40, 220));
    gradient.setColorAt(1, QColor(35, 35, 45, 220));
    
    painter.setBrush(gradient);
    painter.setPen(QPen(QColor(255, 255, 255, 20), 1));
    painter.drawRoundedRect(rect(), 10, 10);
    
    // Highlight effect
    QLinearGradient highlight(0, 0, width() / 2, 0);
    highlight.setColorAt(0, QColor(255, 255, 255, 20));
    highlight.setColorAt(1, QColor(255, 255, 255, 0));
    painter.setBrush(highlight);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect().adjusted(0, 0, -width()/2, 0), 10, 10);
    
    QWidget::paintEvent(event);
}
