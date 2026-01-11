#include "PDFEditor.h"
#include "PDFDocument.h"
#include <QStack>

class PDFEditor::Private {
public:
    PDFDocument *document = nullptr;
    EditMode currentMode = None;
    QColor currentColor = Qt::black;
    qreal penWidth = 2.0;
    
    QVector<QPointF> currentStroke;
    QPointF startPoint;
    bool isEditing = false;
    
    struct EditAction {
        EditMode mode;
        QVariant data;
    };
    
    QStack<EditAction> undoStack;
    QStack<EditAction> redoStack;
};

PDFEditor::PDFEditor(PDFDocument *document, QObject *parent)
    : QObject(parent)
    , d(std::make_unique<Private>())
{
    d->document = document;
}

PDFEditor::~PDFEditor() = default;

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
    if (!d->isEditing || !d->document) {
        return;
    }
    
    // Apply the edit to the document
    switch (d->currentMode) {
        case Drawing:
            if (d->currentStroke.size() > 1) {
                d->document->addDrawing(0, d->currentStroke, d->currentColor, d->penWidth);
                emit editApplied();
            }
            break;
            
        case Highlight: {
            QRectF rect(d->startPoint, d->currentStroke.last());
            d->document->addHighlight(0, rect.normalized(), d->currentColor);
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
    if (!d->undoStack.isEmpty()) {
        auto action = d->undoStack.pop();
        d->redoStack.push(action);
        // Revert the action
        emit editApplied();
    }
}

void PDFEditor::redo() {
    if (!d->redoStack.isEmpty()) {
        auto action = d->redoStack.pop();
        d->undoStack.push(action);
        // Reapply the action
        emit editApplied();
    }
}

bool PDFEditor::canUndo() const {
    return !d->undoStack.isEmpty();
}

bool PDFEditor::canRedo() const {
    return !d->redoStack.isEmpty();
}
