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
    if (!m_document || m_currentPage >= m_document->pageCount()) {
        return;
    }
    
    // Render PDF page with hardware acceleration
    QImage pageImage = m_document->renderPage(m_currentPage, m_zoom * devicePixelRatio());
    
    if (!pageImage.isNull()) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        
        // Calculate centered position
        int x = (width() - pageImage.width() / devicePixelRatio()) / 2;
        int y = (height() - pageImage.height() / devicePixelRatio()) / 2;
        
        // Draw shadow
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0, 0, 0, 50));
        painter.drawRoundedRect(x + 5, y + 5, 
                                pageImage.width() / devicePixelRatio(),
                                pageImage.height() / devicePixelRatio(), 
                                8, 8);
        
        // Draw page
        painter.drawImage(x, y, pageImage);
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
        // Scroll pages
        if (event->angleDelta().y() > 0 && m_currentPage > 0) {
            m_currentPage--;
            m_needsUpdate = true;
            emit pageChanged(m_currentPage);
        } else if (event->angleDelta().y() < 0 && 
                   m_document && m_currentPage < m_document->pageCount() - 1) {
            m_currentPage++;
            m_needsUpdate = true;
            emit pageChanged(m_currentPage);
        }
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
