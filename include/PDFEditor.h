#ifndef PDFEDITOR_H
#define PDFEDITOR_H

#include <QObject>
#include <QVector>
#include <QPointF>
#include <QColor>
#include <memory>

class PDFDocument;

class PDFEditor : public QObject {
    Q_OBJECT

public:
    enum EditMode {
        None,
        TextEdit,
        Highlight,
        Drawing,
        Shape,
        Image,
        Annotation,
        Eraser
    };

    explicit PDFEditor(PDFDocument *document, QObject *parent = nullptr);
    ~PDFEditor();

    void setEditMode(EditMode mode);
    EditMode editMode() const;
    
    void setColor(const QColor &color);
    void setPenWidth(qreal width);
    
    void beginEdit(const QPointF &pos);
    void continueEdit(const QPointF &pos);
    void endEdit();
    
    void undo();
    void redo();
    bool canUndo() const;
    bool canRedo() const;

signals:
    void editModeChanged(EditMode mode);
    void editApplied();

private:
    class Private;
    std::unique_ptr<Private> d;
};

#endif // PDFEDITOR_H
