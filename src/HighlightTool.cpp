#include "HighlightTool.h"

HighlightTool::HighlightTool(QObject *parent)
    : QObject(parent)
    , m_color(Qt::yellow)
    , m_opacity(0.4)
{
}

void HighlightTool::setColor(const QColor &color) {
    m_color = color;
}

void HighlightTool::setOpacity(qreal opacity) {
    m_opacity = qBound(0.0, opacity, 1.0);
}
