#include "PDFEditor.h"
#include "PDFDocument.h"
#include <QStack>
#include <QVariant>
#include <QUndoCommand>

// Undo commands for different edit operations
class DrawingCommand : public QUndoCommand {
public:
    DrawingCommand(PDFDocument *doc, int page, const QVector<QPointF> &points, 
                   const QColor &color, qreal width)
        : m_document(doc), m_page(page), m_points(points), m_color(color), m_width(width) {
        setText("Drawing");
    }
    
    void undo() override {
        // Remove the last drawing from document
        m_document->removeLastDrawing();
    }
    
    void redo() override {
        m_document->addDrawing(m_page, m_points, m_color, m_width);
    }
    
private:
    PDFDocument *m_document;
    int m_page;
    QVector<QPointF> m_points;
    QColor m_color;
    qreal m_width;
};

class HighlightCommand : public QUndoCommand {
public:
    HighlightCommand(PDFDocument *doc, int page, const QRectF &rect, const QColor &color)
        : m_document(doc), m_page(page), m_rect(rect), m_color(color) {
        setText("Highlight");
    }
    
    void undo() override {
        m_document->removeLastHighlight();
    }
    
    void redo() override {
        m_document->addHighlight(m_page, m_rect, m_color);
    }
    
private:
    PDFDocument *m_document;
    int m_page;
    QRectF m_rect;
    QColor m_color;
};

class PDFEditor::Private {
public:
    PDFDocument *document = nullptr;
    QUndoStack *undoStack = nullptr;
    EditMode currentMode = None;
    QColor currentColor = Qt::black;
    qreal penWidth = 2.0;
    
    QVector<QPointF> currentStroke;
    QPointF startPoint;
    bool isEditing = false;
    int currentPage = 0;
};

PDFEditor::PDFEditor(PDFDocument *document, QObject *parent)
    : QObject(parent)
    , d(std::make_unique<Private>())
{
    d->document = document;
}

PDFEditor::~PDFEditor() = default;

void PDFEditor::setUndoStack(QUndoStack *stack) {
    d->undoStack = stack;
}

void PDFEditor::setEditMode(EditMode mode) {
    if (d->currentMode != mode) {
        d->currentMode = mode;
        emit editModeChanged(mode);
    }
}

PDFEditor::EditMode PDFEditor::editMode() const {
    return d->currentMode;
}

void PDFEditor::setColor(const QColor &color) {
    d->currentColor = color;
}

void PDFEditor::setPenWidth(qreal width) {
    d->penWidth = width;
}

void PDFEditor::beginEdit(const QPointF &pos) {
    d->isEditing = true;
    d->startPoint = pos;
    d->currentStroke.clear();
    d->currentStroke.append(pos);
}

void PDFEditor::continueEdit(const QPointF &pos) {
    if (!d->isEditing) {
        return;
    }
    
    switch (d->currentMode) {
        case Drawing:
            d->currentStroke.append(pos);
            break;
            
        case Highlight:
            // Update highlight rectangle
            break;
            
        default:
            break;
    }
}

void PDFEditor::endEdit() {
    if (!d->isEditing || !d->document || !d->undoStack) {
        return;
    }
    
    // Apply the edit to the document using undo commands
    switch (d->currentMode) {
        case Drawing:
            if (d->currentStroke.size() > 1) {
                d->undoStack->push(new DrawingCommand(d->document, d->currentPage, 
                                                     d->currentStroke, d->currentColor, d->penWidth));
                emit editApplied();
            }
            break;
            
        case Highlight: {
            QRectF rect(d->startPoint, d->currentStroke.last());
            d->undoStack->push(new HighlightCommand(d->document, d->currentPage, 
                                                    rect.normalized(), d->currentColor));
            emit editApplied();
            break;
        }
            
        case TextEdit:
            // Text editing would open a text input dialog
            break;
            
        case Annotation:
            // Annotation would open a comment dialog
            break;
            
        default:
            break;
    }
    
    d->isEditing = false;
    d->currentStroke.clear();
}

void PDFEditor::undo() {
    if (d->undoStack) {
        d->undoStack->undo();
        emit editApplied();
    }
}

void PDFEditor::redo() {
    if (d->undoStack) {
        d->undoStack->redo();
        emit editApplied();
    }
}

bool PDFEditor::canUndo() const {
    return d->undoStack && d->undoStack->canUndo();
}

bool PDFEditor::canRedo() const {
    return d->undoStack && d->undoStack->canRedo();
}
