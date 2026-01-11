#include "AnnotationTool.h"

AnnotationTool::AnnotationTool(QObject *parent)
    : QObject(parent)
    , m_bgColor(Qt::yellow)
    , m_textColor(Qt::black)
{
}

void AnnotationTool::setBackgroundColor(const QColor &color) {
    m_bgColor = color;
}

void AnnotationTool::setTextColor(const QColor &color) {
    m_textColor = color;
}
