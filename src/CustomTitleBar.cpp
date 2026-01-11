#include "CustomTitleBar.h"
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QLinearGradient>
#include <QApplication>

CustomTitleBar::CustomTitleBar(QWidget *parent)
    : QWidget(parent)
    , m_isDragging(false)
    , m_hoverAnimation(nullptr)
{
    setFixedHeight(40);
    setupUI();
    
    setStyleSheet(R"(
        QLabel {
            color: white;
            font-size: 14px;
            font-weight: 500;
        }
        QPushButton {
            background: transparent;
            border: none;
            color: white;
            font-size: 16px;
            font-weight: bold;
            border-radius: 4px;
        }
        QPushButton:hover {
            background: rgba(255, 255, 255, 30);
        }
        QPushButton#closeBtn:hover {
            background: rgba(255, 50, 50, 200);
        }
    )");
}

void CustomTitleBar::setupUI() {
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(15, 0, 10, 0);
    layout->setSpacing(5);
    
    // App icon placeholder
    QLabel *iconLabel = new QLabel("📄", this);
    iconLabel->setFixedSize(24, 24);
    layout->addWidget(iconLabel);
    
    // Title
    m_titleLabel = new QLabel("PDF Editor", this);
    layout->addWidget(m_titleLabel);
    
    layout->addStretch();
    
    // Window control buttons
    m_minimizeBtn = new QPushButton("−", this);
    m_minimizeBtn->setFixedSize(40, 30);
    m_minimizeBtn->setCursor(Qt::PointingHandCursor);
    connect(m_minimizeBtn, &QPushButton::clicked, this, &CustomTitleBar::minimizeClicked);
    layout->addWidget(m_minimizeBtn);
    
    m_maximizeBtn = new QPushButton("□", this);
    m_maximizeBtn->setFixedSize(40, 30);
    m_maximizeBtn->setCursor(Qt::PointingHandCursor);
    connect(m_maximizeBtn, &QPushButton::clicked, this, &CustomTitleBar::maximizeClicked);
    layout->addWidget(m_maximizeBtn);
    
    m_closeBtn = new QPushButton("✕", this);
    m_closeBtn->setObjectName("closeBtn");
    m_closeBtn->setFixedSize(40, 30);
    m_closeBtn->setCursor(Qt::PointingHandCursor);
    connect(m_closeBtn, &QPushButton::clicked, this, &CustomTitleBar::closeClicked);
    layout->addWidget(m_closeBtn);
}

void CustomTitleBar::setTitle(const QString &title) {
    m_titleLabel->setText(title);
}

QString CustomTitleBar::title() const {
    return m_titleLabel->text();
}

void CustomTitleBar::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw gradient background with glass effect
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor(40, 40, 50, 200));
    gradient.setColorAt(1, QColor(30, 30, 40, 200));
    
    painter.setBrush(gradient);
    painter.setPen(QPen(QColor(255, 255, 255, 30), 1));
    painter.drawRoundedRect(rect(), 12, 12);
    
    // Draw top highlight for glass effect
    QLinearGradient highlightGradient(0, 0, 0, height() / 2);
    highlightGradient.setColorAt(0, QColor(255, 255, 255, 40));
    highlightGradient.setColorAt(1, QColor(255, 255, 255, 0));
    painter.setBrush(highlightGradient);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -height()/2), 12, 12);
    
    QWidget::paintEvent(event);
}

void CustomTitleBar::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isDragging = true;
        m_dragPosition = event->globalPos() - window()->frameGeometry().topLeft();
        event->accept();
    }
}

void CustomTitleBar::mouseMoveEvent(QMouseEvent *event) {
    if (m_isDragging && (event->buttons() & Qt::LeftButton)) {
        window()->move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void CustomTitleBar::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
        event->accept();
    }
}

void CustomTitleBar::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit maximizeClicked();
        event->accept();
    }
}

void CustomTitleBar::animateButton(QPushButton *button) {
    // Smooth button animation on hover
    if (!m_hoverAnimation) {
        m_hoverAnimation = new QPropertyAnimation(button, "geometry", this);
        m_hoverAnimation->setDuration(150);
        m_hoverAnimation->setEasingCurve(QEasingCurve::OutCubic);
    }
}
