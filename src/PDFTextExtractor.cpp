#include "PDFTextExtractor.h"
#include "PDFDocument.h"
#include <QRegularExpression>

PDFTextExtractor::PDFTextExtractor(PDFDocument *document)
    : m_document(document)
{
}

QVector<TextBlock> PDFTextExtractor::extractTextBlocks(int pageIndex) {
    if (!m_document || !m_document->isValid()) {
        return {};
    }
    
    // Проверяем кэш
    if (pageIndex >= 0 && pageIndex < m_cachedBlocks.size()) {
        return m_cachedBlocks[pageIndex];
    }
    
    // Расширяем кэш если нужно
    while (m_cachedBlocks.size() <= pageIndex) {
        m_cachedBlocks.append(QVector<TextBlock>());
    }
    
    QVector<TextBlock> blocks;
    
    // Извлекаем текст из placeholder-контента нашего PDFDocument
    // В реальной реализации здесь был бы парсинг PDF операторов
    QImage pageImage = m_document->renderPage(pageIndex, 1.0);
    if (pageImage.isNull()) {
        return blocks;
    }
    
    // Симулируем извлечение текста
    // В реальности нужна библиотека для парсинга PDF (например, Poppler)
    
    // Заголовок
    TextBlock titleBlock;
    titleBlock.text = QString("PDF Document - Page %1").arg(pageIndex + 1);
    titleBlock.rect = QRectF(50, 100, 495, 40);
    titleBlock.fontFamily = "Arial";
    titleBlock.fontSize = 14;
    titleBlock.color = Qt::black;
    titleBlock.pageIndex = pageIndex;
    blocks.append(titleBlock);
    
    // Основной текст - разбиваем на абзацы
    QStringList lines = {
        "This is a placeholder for PDF content. In a production version,",
        "this would display the actual PDF page rendered using a library",
        "like PDFium, MuPDF, or Poppler.",
        "",
        "The PDF Editor supports:",
        "• Text annotations and editing",
        "• Highlighting important sections",
        "• Drawing and freehand sketches",
        "• Adding shapes and arrows",
        "• Inserting images",
        "• Adding comments and notes",
        "",
        "Use the tools on the left sidebar to edit the document.",
        "File menu provides open/save functionality.",
        "View menu allows zoom and theme switching.",
    };
    
    qreal yPos = 200;
    qreal lineHeight = 20;
    qreal leftMargin = 50;
    qreal rightMargin = 545;
    
    QString currentParagraph;
    qreal paragraphStart = yPos;
    
    for (const QString &line : lines) {
        if (line.isEmpty()) {
            // Сохраняем текущий параграф
            if (!currentParagraph.isEmpty()) {
                TextBlock block;
                block.text = currentParagraph.trimmed();
                block.rect = QRectF(leftMargin, paragraphStart, rightMargin - leftMargin, yPos - paragraphStart);
                block.fontFamily = "Arial";
                block.fontSize = 10;
                block.color = Qt::black;
                block.pageIndex = pageIndex;
                blocks.append(block);
                
                currentParagraph.clear();
            }
            yPos += lineHeight;
            paragraphStart = yPos;
        } else {
            if (!currentParagraph.isEmpty()) {
                currentParagraph += " ";
            }
            currentParagraph += line;
            yPos += lineHeight;
        }
    }
    
    // Последний параграф
    if (!currentParagraph.isEmpty()) {
        TextBlock block;
        block.text = currentParagraph.trimmed();
        block.rect = QRectF(leftMargin, paragraphStart, rightMargin - leftMargin, yPos - paragraphStart);
        block.fontFamily = "Arial";
        block.fontSize = 10;
        block.color = Qt::black;
        block.pageIndex = pageIndex;
        blocks.append(block);
    }
    
    // Сохраняем в кэш
    m_cachedBlocks[pageIndex] = blocks;
    
    return blocks;
}

TextBlock PDFTextExtractor::findBlockAt(int pageIndex, const QPointF &pos) {
    QVector<TextBlock> blocks = extractTextBlocks(pageIndex);
    
    // Ищем блок, содержащий точку
    for (int i = blocks.size() - 1; i >= 0; --i) {
        if (blocks[i].rect.contains(pos)) {
            return blocks[i];
        }
    }
    
    return TextBlock();  // Возвращаем пустой блок
}
