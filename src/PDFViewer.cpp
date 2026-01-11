#include "PDFViewer.h"
#include "PDFDocument.h"
#include "PDFEditor.h"
#include <QPainter>
#include <QWheelEvent>
#include <QTimer>
#include <QOpenGLShaderProgram>

PDFViewer::PDFViewer(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_currentPage(0)
    , m_zoom(1.0)
    , m_scrollOffset(0.0)
    , m_pageSpacing(20.0)
    , m_isPanning(false)
    , m_needsUpdate(false)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    
    // Setup high-performance timer for smooth rendering
    m_renderTimer = new QTimer(this);
    m_renderTimer->setInterval(4); // ~240 FPS
    connect(m_renderTimer, &QTimer::timeout, [this]() {
        if (m_needsUpdate) {
            update();
            m_needsUpdate = false;
        }
    });
    m_renderTimer->start();
}

PDFViewer::~PDFViewer() = default;

bool PDFViewer::loadDocument(const QString &filePath) {
    m_document = std::make_unique<PDFDocument>();
    
    if (m_document->load(filePath)) {
        m_editor = std::make_unique<PDFEditor>(m_document.get(), this);
        m_currentPage = 0;
        m_zoom = 1.0;
        m_scrollOffset = 0.0;
        m_needsUpdate = true;
        
        emit documentLoaded();
        return true;
    }
    
    return false;
}

void PDFViewer::saveDocument(const QString &filePath) {
    if (m_document) {
        m_document->save(filePath);
    }
}

PDFDocument* PDFViewer::document() const {
    return m_document.get();
}

void PDFViewer::setZoom(qreal zoom) {
    m_zoom = qBound(0.1, zoom, 5.0);
    updateTransform();
    m_needsUpdate = true;
}

qreal PDFViewer::zoom() const {
    return m_zoom;
}

void PDFViewer::setCurrentTool(int toolId) {
    if (m_editor) {
        m_editor->setEditMode(static_cast<PDFEditor::EditMode>(toolId));
    }
}

void PDFViewer::initializeGL() {
    initializeOpenGLFunctions();
    
    // Enable multisampling for smooth edges
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Set clear color (dark theme)
    glClearColor(0.12f, 0.12f, 0.14f, 1.0f);
}

void PDFViewer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    updateTransform();
}

void PDFViewer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (!m_document || !m_document->isValid()) {
        return;
    }
    
    renderPage();
}

void PDFViewer::renderPage() {
    if (!m_document) {
        return;
    }
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    
    // Render all pages vertically
    int pageCount = m_document->pageCount();
    qreal yPos = -m_scrollOffset;
    
    for (int i = 0; i < pageCount; ++i) {
        QImage pageImage = m_document->renderPage(i, m_zoom * devicePixelRatio());
        
        if (!pageImage.isNull()) {
            qreal pageWidth = pageImage.width() / devicePixelRatio();
            qreal pageHeight = pageImage.height() / devicePixelRatio();
            
            // Calculate centered position
            int x = (width() - pageWidth) / 2;
            
            // Only render if visible
            if (yPos + pageHeight > 0 && yPos < height()) {
                // Draw shadow
                painter.setPen(Qt::NoPen);
                painter.setBrush(QColor(0, 0, 0, 50));
                painter.drawRoundedRect(x + 5, yPos + 5, pageWidth, pageHeight, 8, 8);
                
                // Draw page
                painter.drawImage(x, yPos, pageImage);
                
                // Draw page number
                painter.setPen(QColor(150, 150, 150));
                painter.setFont(QFont("Arial", 10));
                painter.drawText(x, yPos - 5, QString("Page %1").arg(i + 1));
            }
            
            yPos += pageHeight + m_pageSpacing;
        }
    }
}

void PDFViewer::updateTransform() {
    m_transform.setToIdentity();
    m_transform.scale(m_zoom, m_zoom);
}

void PDFViewer::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        // Zoom with Ctrl + Wheel
        qreal delta = event->angleDelta().y() / 1200.0;
        setZoom(m_zoom + delta);
    } else {
        // Vertical scroll through pages
        qreal scrollDelta = event->angleDelta().y();
        m_scrollOffset -= scrollDelta * 0.5;
        
        // Clamp scroll to valid range
        if (m_scrollOffset < 0) {
            m_scrollOffset = 0;
        }
        
        if (m_document) {
            // Calculate total height
            qreal totalHeight = 0;
            for (int i = 0; i < m_document->pageCount(); ++i) {
                QImage pageImage = m_document->renderPage(i, m_zoom * devicePixelRatio());
                totalHeight += (pageImage.height() / devicePixelRatio()) + m_pageSpacing;
            }
            
            qreal maxScroll = qMax(0.0, totalHeight - height());
            if (m_scrollOffset > maxScroll) {
                m_scrollOffset = maxScroll;
            }
        }
        
        m_needsUpdate = true;
    }
    
    event->accept();
}

void PDFViewer::mousePressEvent(QMouseEvent *event) {
    m_lastMousePos = event->pos();
    
    if (event->button() == Qt::MiddleButton) {
        m_isPanning = true;
        setCursor(Qt::ClosedHandCursor);
    } else if (m_editor) {
        m_editor->beginEdit(event->pos());
    }
    
    event->accept();
}

void PDFViewer::mouseMoveEvent(QMouseEvent *event) {
    if (m_isPanning) {
        // Pan view
        QPoint delta = event->pos() - m_lastMousePos;
        m_lastMousePos = event->pos();
        m_needsUpdate = true;
    } else if (m_editor) {
        m_editor->continueEdit(event->pos());
        m_needsUpdate = true;
    }
    
    event->accept();
}

void PDFViewer::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::MiddleButton) {
        m_isPanning = false;
        setCursor(Qt::ArrowCursor);
    } else if (m_editor) {
        m_editor->endEdit();
    }
    
    event->accept();
}
