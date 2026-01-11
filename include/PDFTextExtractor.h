#ifndef PDFTEXTEXTRACTOR_H
#define PDFTEXTEXTRACTOR_H

#include <QString>
#include <QVector>
#include <QRectF>
#include <QFont>
#include <QColor>

class PDFDocument;

// Структура для хранения текстового блока
struct TextBlock {
    QString text;
    QRectF rect;          // Позиция и размер блока
    QString fontFamily;
    qreal fontSize;
    QColor color;
    int pageIndex;
    
    TextBlock() : fontSize(12.0), color(Qt::black), pageIndex(0) {}
    
    bool contains(const QPointF &point) const {
        return rect.contains(point);
    }
};

class PDFTextExtractor {
public:
    explicit PDFTextExtractor(PDFDocument *document);
    
    // Извлечь все текстовые блоки со страницы
    QVector<TextBlock> extractTextBlocks(int pageIndex);
    
    // Найти текстовый блок по клику
    TextBlock findBlockAt(int pageIndex, const QPointF &pos);
    
private:
    PDFDocument *m_document;
    QVector<QVector<TextBlock>> m_cachedBlocks;  // Кэш по страницам
};

#endif // PDFTEXTEXTRACTOR_H
