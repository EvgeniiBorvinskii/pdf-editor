#ifndef HIGHLIGHTTOOL_H
#define HIGHLIGHTTOOL_H

#include <QObject>
#include <QColor>

class HighlightTool : public QObject {
    Q_OBJECT

public:
    explicit HighlightTool(QObject *parent = nullptr);
    
    void setColor(const QColor &color);
    void setOpacity(qreal opacity);
    
    QColor color() const { return m_color; }
    qreal opacity() const { return m_opacity; }

private:
    QColor m_color;
    qreal m_opacity;
};

#endif // HIGHLIGHTTOOL_H
