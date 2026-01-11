#include "TextTool.h"

TextTool::TextTool(QObject *parent)
    : QObject(parent)
    , m_font("Arial", 12)
    , m_color(Qt::black)
{
}

void TextTool::setFont(const QFont &font) {
    m_font = font;
}

void TextTool::setColor(const QColor &color) {
    m_color = color;
}
