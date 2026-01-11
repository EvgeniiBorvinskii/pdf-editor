#ifndef DRAWINGTOOL_H
#define DRAWINGTOOL_H

#include <QObject>
#include <QColor>
#include <QVector>
#include <QPointF>

class DrawingTool : public QObject {
    Q_OBJECT

public:
    explicit DrawingTool(QObject *parent = nullptr);
    
    void setPenColor(const QColor &color);
    void setPenWidth(qreal width);
    void setSmoothing(bool enabled);
    
    QColor penColor() const { return m_penColor; }
    qreal penWidth() const { return m_penWidth; }
    bool smoothing() const { return m_smoothing; }

private:
    QColor m_penColor;
    qreal m_penWidth;
    bool m_smoothing;
};

#endif // DRAWINGTOOL_H
