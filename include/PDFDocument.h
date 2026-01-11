#ifndef PDFDOCUMENT_H
#define PDFDOCUMENT_H

#include <QString>
#include <QImage>
#include <QVector>
#include <memory>

class PDFDocument {
public:
    PDFDocument();
    ~PDFDocument();

    bool load(const QString &filePath);
    bool save(const QString &filePath);
    
    int pageCount() const;
    QImage renderPage(int pageIndex, qreal scale = 1.0);
    QSizeF pageSize(int pageIndex) const;
    
    bool isValid() const;
    QString filePath() const;
    
    // Editing operations
    void addAnnotation(int page, const QPointF &pos, const QString &text);
    void addDrawing(int page, const QVector<QPointF> &points, const QColor &color, qreal width);
    void addHighlight(int page, const QRectF &rect, const QColor &color);
    void addText(int page, const QPointF &pos, const QString &text, const QFont &font);
    void addImage(int page, const QPointF &pos, const QImage &image);

private:
    class Private;
    std::unique_ptr<Private> d;
};

#endif // PDFDOCUMENT_H
