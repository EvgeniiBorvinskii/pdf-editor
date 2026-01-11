#ifndef TEXTTOOL_H
#define TEXTTOOL_H

#include <QObject>
#include <QFont>
#include <QColor>

class TextTool : public QObject {
    Q_OBJECT

public:
    explicit TextTool(QObject *parent = nullptr);
    
    void setFont(const QFont &font);
    void setColor(const QColor &color);
    
    QFont font() const { return m_font; }
    QColor color() const { return m_color; }

private:
    QFont m_font;
    QColor m_color;
};

#endif // TEXTTOOL_H
