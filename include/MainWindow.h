#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsBlurEffect>
#include <QPropertyAnimation>
#include <memory>

class CustomTitleBar;
class PDFViewer;
class EditToolbar;
class AnimationEngine;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void openFile();
    void saveFile();
    void saveFileAs();
    void onToolChanged(int toolId);
    void toggleTheme();
    void updateContentScale();

private:
    void setupUI();
    void setupAnimations();
    void applyLiquidGlassEffect();
    void createMenuBar();
    
    CustomTitleBar *m_titleBar;
    PDFViewer *m_pdfViewer;
    EditToolbar *m_editToolbar;
    std::unique_ptr<AnimationEngine> m_animationEngine;
    
    QString m_currentFile;
    QGraphicsBlurEffect *m_blurEffect;
    bool m_isDarkTheme;
    qreal m_contentScale;
};

#endif // MAINWINDOW_H
