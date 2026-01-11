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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_titleBar(nullptr)
    , m_pdfViewer(nullptr)
    , m_editToolbar(nullptr)
    , m_blurEffect(nullptr)
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
    
    // Edit toolbar
    m_editToolbar = new EditToolbar(this);
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
    // File menu
    QMenu *fileMenu = new QMenu("File", this);
    
    QAction *openAction = fileMenu->addAction("Open PDF");
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    
    QAction *saveAction = fileMenu->addAction("Save");
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    
    QAction *saveAsAction = fileMenu->addAction("Save As...");
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveFileAs);
    
    fileMenu->addSeparator();
    
    QAction *exitAction = fileMenu->addAction("Exit");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
}

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this,
        "Open PDF File", QString(), "PDF Files (*.pdf)");
    
    if (!fileName.isEmpty()) {
        if (m_pdfViewer->loadDocument(fileName)) {
            m_currentFile = fileName;
            m_titleBar->setTitle("PDF Editor - " + QFileInfo(fileName).fileName());
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
}
