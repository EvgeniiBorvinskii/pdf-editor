#include "MainWindow.h"
#include "CustomTitleBar.h"
#include "PDFViewer.h"
#include "EditToolbar.h"
#include "AnimationEngine.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QLinearGradient>
#include <QApplication>
#include <QTransform>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_titleBar(nullptr)
    , m_pdfViewer(nullptr)
    , m_editToolbar(nullptr)
    , m_undoStack(new QUndoStack(this))
    , m_blurEffect(nullptr)
    , m_isDarkTheme(true)
    , m_contentScale(1.0)
{
    // Remove default window frame
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground, false);
    
    // Set minimum size
    setMinimumSize(1000, 700);
    resize(1400, 900);
    
    setupUI();
    setupAnimations();
    applyLiquidGlassEffect();
    createMenuBar();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI() {
    // Create central widget
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    setCentralWidget(centralWidget);
    
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(1, 1, 1, 1);
    mainLayout->setSpacing(0);
    
    // Custom title bar
    m_titleBar = new CustomTitleBar(this);
    m_titleBar->setTitle("PDF Editor");
    connect(m_titleBar, &CustomTitleBar::minimizeClicked, this, &QWidget::showMinimized);
    connect(m_titleBar, &CustomTitleBar::maximizeClicked, [this]() {
        if (isMaximized()) {
            showNormal();
        } else {
            showMaximized();
        }
    });
    connect(m_titleBar, &CustomTitleBar::closeClicked, this, &QWidget::close);
    mainLayout->addWidget(m_titleBar);
    
    // Content layout
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);
    
    // Edit toolbar (hidden until PDF is loaded)
    m_editToolbar = new EditToolbar(this);
    m_editToolbar->setVisible(false);
    connect(m_editToolbar, &EditToolbar::toolChanged, this, &MainWindow::onToolChanged);
    contentLayout->addWidget(m_editToolbar);
    
    // PDF Viewer
    m_pdfViewer = new PDFViewer(this);
    contentLayout->addWidget(m_pdfViewer, 1);
    
    mainLayout->addLayout(contentLayout, 1);
    
    // Apply stylesheet for Liquid Glass effect
    centralWidget->setStyleSheet(R"(
        #centralWidget {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(20, 20, 25, 240),
                stop:0.5 rgba(25, 25, 30, 240),
                stop:1 rgba(30, 30, 35, 240));
            border: 1px solid rgba(255, 255, 255, 30);
            border-radius: 12px;
        }
    )");
}

void MainWindow::setupAnimations() {
    m_animationEngine = std::make_unique<AnimationEngine>(this);
    m_animationEngine->setTargetFPS(240);
    m_animationEngine->start();
}

void MainWindow::applyLiquidGlassEffect() {
    // Apply blur effect to create glass morphism
    m_blurEffect = new QGraphicsBlurEffect(this);
    m_blurEffect->setBlurRadius(10);
    m_blurEffect->setBlurHints(QGraphicsBlurEffect::PerformanceHint);
}

void MainWindow::createMenuBar() {
    QMenuBar *menuBar = new QMenuBar(this);
    menuBar->setStyleSheet(R"(
        QMenuBar {
            background: rgba(30, 30, 40, 220);
            color: white;
            border-bottom: 1px solid rgba(255, 255, 255, 30);
            padding: 5px;
        }
        QMenuBar::item {
            background: transparent;
            padding: 8px 15px;
            border-radius: 4px;
        }
        QMenuBar::item:selected {
            background: rgba(42, 130, 218, 150);
        }
        QMenu {
            background: rgba(30, 30, 40, 240);
            color: white;
            border: 1px solid rgba(255, 255, 255, 30);
            border-radius: 8px;
            padding: 5px;
        }
        QMenu::item {
            padding: 8px 30px;
            border-radius: 4px;
        }
        QMenu::item:selected {
            background: rgba(42, 130, 218, 180);
        }
        QMenu::separator {
            height: 1px;
            background: rgba(255, 255, 255, 20);
            margin: 5px 10px;
        }
    )");
    
    // File menu
    QMenu *fileMenu = menuBar->addMenu("📁 File");
    
    QAction *openAction = fileMenu->addAction("📂 Open PDF...");
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    
    QAction *saveAction = fileMenu->addAction("💾 Save");
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    
    QAction *saveAsAction = fileMenu->addAction("💾 Save As...");
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveFileAs);
    
    fileMenu->addSeparator();
    
    QAction *exportAction = fileMenu->addAction("📤 Export as Image...");
    exportAction->setShortcut(QKeySequence("Ctrl+E"));
    connect(exportAction, &QAction::triggered, [this]() {
        QMessageBox::information(this, "Export", "Export functionality - Coming soon!");
    });
    
    fileMenu->addSeparator();
    
    QAction *exitAction = fileMenu->addAction("❌ Exit");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    
    // Edit menu
    QMenu *editMenu = menuBar->addMenu("✏️ Edit");
    
    QAction *undoAction = editMenu->addAction("↶ Undo");
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, &QAction::triggered, m_undoStack, &QUndoStack::undo);
    
    QAction *redoAction = editMenu->addAction("↷ Redo");
    redoAction->setShortcut(QKeySequence::Redo);
    connect(redoAction, &QAction::triggered, m_undoStack, &QUndoStack::redo);
    
    editMenu->addSeparator();
    
    QAction *copyAction = editMenu->addAction("📋 Copy");
    copyAction->setShortcut(QKeySequence::Copy);
    
    QAction *pasteAction = editMenu->addAction("📋 Paste");
    pasteAction->setShortcut(QKeySequence::Paste);
    
    QAction *deleteAction = editMenu->addAction("🗑️ Delete");
    deleteAction->setShortcut(QKeySequence::Delete);
    
    // View menu
    QMenu *viewMenu = menuBar->addMenu("👁️ View");
    
    QAction *zoomInAction = viewMenu->addAction("🔍+ Zoom In");
    zoomInAction->setShortcut(QKeySequence::ZoomIn);
    connect(zoomInAction, &QAction::triggered, [this]() {
        if (m_pdfViewer) {
            m_pdfViewer->setZoom(m_pdfViewer->zoom() + 0.2);
        }
    });
    
    QAction *zoomOutAction = viewMenu->addAction("🔍- Zoom Out");
    zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    connect(zoomOutAction, &QAction::triggered, [this]() {
        if (m_pdfViewer) {
            m_pdfViewer->setZoom(m_pdfViewer->zoom() - 0.2);
        }
    });
    
    QAction *fitWidthAction = viewMenu->addAction("↔️ Fit Width");
    fitWidthAction->setShortcut(QKeySequence("Ctrl+1"));
    
    QAction *fitPageAction = viewMenu->addAction("📄 Fit Page");
    fitPageAction->setShortcut(QKeySequence("Ctrl+2"));
    
    viewMenu->addSeparator();
    
    QAction *fullscreenAction = viewMenu->addAction("⛶ Fullscreen");
    fullscreenAction->setShortcut(QKeySequence("F11"));
    connect(fullscreenAction, &QAction::triggered, [this]() {
        if (isFullScreen()) {
            showNormal();
        } else {
            showFullScreen();
        }
    });
    
    viewMenu->addSeparator();
    
    // Theme toggle
    QAction *themeAction = viewMenu->addAction("🎨 Toggle Theme (Dark/Light)");
    themeAction->setShortcut(QKeySequence("Ctrl+T"));
    connect(themeAction, &QAction::triggered, this, &MainWindow::toggleTheme);
    
    // Tools menu
    QMenu *toolsMenu = menuBar->addMenu("🛠️ Tools");
    
    QAction *textToolAction = toolsMenu->addAction("📝 Text Annotation");
    connect(textToolAction, &QAction::triggered, [this]() { m_pdfViewer->setCurrentTool(1); });
    
    QAction *highlightToolAction = toolsMenu->addAction("🖍️ Highlight");
    connect(highlightToolAction, &QAction::triggered, [this]() { m_pdfViewer->setCurrentTool(2); });
    
    QAction *drawToolAction = toolsMenu->addAction("✏️ Drawing");
    connect(drawToolAction, &QAction::triggered, [this]() { m_pdfViewer->setCurrentTool(3); });
    
    QAction *shapeToolAction = toolsMenu->addAction("⬜ Shapes");
    connect(shapeToolAction, &QAction::triggered, [this]() { m_pdfViewer->setCurrentTool(4); });
    
    QAction *imageToolAction = toolsMenu->addAction("🖼️ Insert Image");
    connect(imageToolAction, &QAction::triggered, [this]() { m_pdfViewer->setCurrentTool(5); });
    
    QAction *eraserToolAction = toolsMenu->addAction("🧹 Eraser");
    connect(eraserToolAction, &QAction::triggered, [this]() { m_pdfViewer->setCurrentTool(7); });
    
    // Help menu
    QMenu *helpMenu = menuBar->addMenu("❓ Help");
    
    QAction *aboutAction = helpMenu->addAction("ℹ️ About");
    connect(aboutAction, &QAction::triggered, [this]() {
        QMessageBox::about(this, "About PDF Editor",
            "<h2>PDF Editor v1.0</h2>"
            "<p>Modern PDF Editor with Liquid Glass design</p>"
            "<p>Features:</p>"
            "<ul>"
            "<li>🎨 Beautiful dark/light themes</li>"
            "<li>⚡ 240 FPS smooth animations</li>"
            "<li>✏️ Complete editing toolset</li>"
            "<li>🚀 Hardware accelerated rendering</li>"
            "</ul>"
            "<p><b>Author:</b> EvgeniiBorvinskii</p>"
            "<p><b>GitHub:</b> <a href='https://github.com/EvgeniiBorvinskii/pdf-editor'>pdf-editor</a></p>");
    });
    
    QAction *shortcutsAction = helpMenu->addAction("⌨️ Keyboard Shortcuts");
    connect(shortcutsAction, &QAction::triggered, [this]() {
        QMessageBox::information(this, "Keyboard Shortcuts",
            "<h3>Keyboard Shortcuts</h3>"
            "<table>"
            "<tr><td><b>Ctrl+O</b></td><td>Open PDF</td></tr>"
            "<tr><td><b>Ctrl+S</b></td><td>Save</td></tr>"
            "<tr><td><b>Ctrl+Z</b></td><td>Undo</td></tr>"
            "<tr><td><b>Ctrl+Y</b></td><td>Redo</td></tr>"
            "<tr><td><b>Ctrl++</b></td><td>Zoom In</td></tr>"
            "<tr><td><b>Ctrl+-</b></td><td>Zoom Out</td></tr>"
            "<tr><td><b>Ctrl+T</b></td><td>Toggle Theme</td></tr>"
            "<tr><td><b>F11</b></td><td>Fullscreen</td></tr>"
            "<tr><td><b>Ctrl+Wheel</b></td><td>Zoom</td></tr>"
            "<tr><td><b>Middle Click</b></td><td>Pan</td></tr>"
            "</table>");
    });
    
    // Add menu bar to layout
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(centralWidget()->layout());
    if (mainLayout) {
        mainLayout->insertWidget(1, menuBar);
    }
}

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this,
        "Open PDF File", QString(), "PDF Files (*.pdf)");
    
    if (!fileName.isEmpty()) {
        if (m_pdfViewer->loadDocument(fileName)) {
            m_currentFile = fileName;
            m_titleBar->setTitle("PDF Editor - " + QFileInfo(fileName).fileName());
            
            // Show edit toolbar when PDF is loaded
            if (m_editToolbar) {
                m_editToolbar->setVisible(true);
            }
        } else {
            QMessageBox::critical(this, "Error", "Failed to load PDF file.");
        }
    }
}

void MainWindow::saveFile() {
    if (m_currentFile.isEmpty()) {
        saveFileAs();
    } else {
        m_pdfViewer->saveDocument(m_currentFile);
    }
}

void MainWindow::saveFileAs() {
    QString fileName = QFileDialog::getSaveFileName(this,
        "Save PDF File", QString(), "PDF Files (*.pdf)");
    
    if (!fileName.isEmpty()) {
        m_pdfViewer->saveDocument(fileName);
        m_currentFile = fileName;
        m_titleBar->setTitle("PDF Editor - " + QFileInfo(fileName).fileName());
    }
}

void MainWindow::onToolChanged(int toolId) {
    m_pdfViewer->setCurrentTool(toolId);
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw shadow
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 80));
    painter.drawRoundedRect(rect().adjusted(5, 5, -5, -5), 12, 12);
    
    QMainWindow::paintEvent(event);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    updateContentScale();
}

void MainWindow::toggleTheme() {
    m_isDarkTheme = !m_isDarkTheme;
    
    QPalette palette;
    if (m_isDarkTheme) {
        // Dark theme
        palette.setColor(QPalette::Window, QColor(20, 20, 25));
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Base, QColor(25, 25, 30));
        palette.setColor(QPalette::AlternateBase, QColor(30, 30, 35));
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::Button, QColor(35, 35, 40));
        palette.setColor(QPalette::ButtonText, Qt::white);
        palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        palette.setColor(QPalette::HighlightedText, Qt::black);
        
        centralWidget()->setStyleSheet(R"(
            #centralWidget {
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                    stop:0 rgba(20, 20, 25, 240),
                    stop:0.5 rgba(25, 25, 30, 240),
                    stop:1 rgba(30, 30, 35, 240));
                border: 1px solid rgba(255, 255, 255, 30);
                border-radius: 12px;
            }
        )");
    } else {
        // Light theme
        palette.setColor(QPalette::Window, QColor(240, 240, 245));
        palette.setColor(QPalette::WindowText, Qt::black);
        palette.setColor(QPalette::Base, QColor(255, 255, 255));
        palette.setColor(QPalette::AlternateBase, QColor(245, 245, 250));
        palette.setColor(QPalette::Text, Qt::black);
        palette.setColor(QPalette::Button, QColor(230, 230, 235));
        palette.setColor(QPalette::ButtonText, Qt::black);
        palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        palette.setColor(QPalette::HighlightedText, Qt::white);
        
        centralWidget()->setStyleSheet(R"(
            #centralWidget {
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                    stop:0 rgba(240, 240, 245, 240),
                    stop:0.5 rgba(245, 245, 250, 240),
                    stop:1 rgba(250, 250, 255, 240));
                border: 1px solid rgba(0, 0, 0, 30);
                border-radius: 12px;
            }
        )");
    }
    
    qApp->setPalette(palette);
    update();
}

void MainWindow::updateContentScale() {
    // Smooth content scaling based on window size
    qreal baseWidth = 1400.0;
    qreal baseHeight = 900.0;
    
    qreal widthScale = width() / baseWidth;
    qreal heightScale = height() / baseHeight;
    
    m_contentScale = qMin(widthScale, heightScale);
    m_contentScale = qBound(0.5, m_contentScale, 2.0);
    
    // Apply scale to viewer
    if (m_pdfViewer) {
        QTransform transform;
        transform.scale(m_contentScale, m_contentScale);
        // Smooth scaling animation
        m_animationEngine->addAnimation(200, [this, transform](qreal progress) {
            // Smooth interpolation would go here
        }, AnimationEngine::Animation::EaseInOut);
    }
}
