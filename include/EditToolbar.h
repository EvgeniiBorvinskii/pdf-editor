#ifndef EDITTOOLBAR_H
#define EDITTOOLBAR_H

#include <QWidget>
#include <QToolButton>
#include <QButtonGroup>

class EditToolbar : public QWidget {
    Q_OBJECT

public:
    enum Tool {
        Select = 0,
        Text,
        Highlight,
        Drawing,
        Shape,
        Image,
        Annotation,
        Eraser
    };

    explicit EditToolbar(QWidget *parent = nullptr);

signals:
    void toolChanged(int toolId);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void setupUI();
    QToolButton* createToolButton(const QString &icon, const QString &tooltip, Tool tool);
    
    QButtonGroup *m_buttonGroup;
    QList<QToolButton*> m_toolButtons;
};

#endif // EDITTOOLBAR_H
