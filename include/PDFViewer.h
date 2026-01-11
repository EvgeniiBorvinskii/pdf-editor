#ifndef PDFVIEWER_H
#define PDFVIEWER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <memory>

class PDFDocument;
class PDFEditor;

class PDFViewer : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit PDFViewer(QWidget *parent = nullptr);
    ~PDFViewer();

    bool loadDocument(const QString &filePath);
    void saveDocument(const QString &filePath);
    PDFDocument* document() const;
    PDFEditor* editor() const;
    
    void setZoom(qreal zoom);
    qreal zoom() const;
    
    void setCurrentTool(int toolId);

signals:
    void documentLoaded();
    void pageChanged(int page);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void renderPage();
    void updateTransform();
    qreal calculateTotalHeight();
    
    std::unique_ptr<PDFDocument> m_document;
    std::unique_ptr<PDFEditor> m_editor;
    
    int m_currentPage;
    qreal m_zoom;
    QMatrix4x4 m_transform;
    qreal m_scrollOffset;
    qreal m_pageSpacing;
    
    QPoint m_lastMousePos;
    bool m_isPanning;
    
    // Performance optimization
    QTimer *m_renderTimer;
    bool m_needsUpdate;
};

#endif // PDFVIEWER_H
