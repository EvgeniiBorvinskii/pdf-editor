#ifndef ANNOTATIONTOOL_H
#define ANNOTATIONTOOL_H

#include <QObject>
#include <QString>
#include <QColor>

class AnnotationTool : public QObject {
    Q_OBJECT

public:
    explicit AnnotationTool(QObject *parent = nullptr);
    
    void setBackgroundColor(const QColor &color);
    void setTextColor(const QColor &color);
    
    QColor backgroundColor() const { return m_bgColor; }
    QColor textColor() const { return m_textColor; }

private:
    QColor m_bgColor;
    QColor m_textColor;
};

#endif // ANNOTATIONTOOL_H
