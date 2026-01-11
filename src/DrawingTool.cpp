#include "DrawingTool.h"

DrawingTool::DrawingTool(QObject *parent)
    : QObject(parent)
    , m_penColor(Qt::black)
    , m_penWidth(2.0)
    , m_smoothing(true)
{
}

void DrawingTool::setPenColor(const QColor &color) {
    m_penColor = color;
}

void DrawingTool::setPenWidth(qreal width) {
    m_penWidth = qMax(0.5, width);
}

void DrawingTool::setSmoothing(bool enabled) {
    m_smoothing = enabled;
}
