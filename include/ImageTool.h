#ifndef IMAGETOOL_H
#define IMAGETOOL_H

#include <QObject>
#include <QImage>

class ImageTool : public QObject {
    Q_OBJECT

public:
    explicit ImageTool(QObject *parent = nullptr);
    
    void setImage(const QImage &image);
    QImage image() const { return m_image; }
    
    void setScale(qreal scale);
    qreal scale() const { return m_scale; }

private:
    QImage m_image;
    qreal m_scale;
};

#endif // IMAGETOOL_H
