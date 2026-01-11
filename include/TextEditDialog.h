#ifndef TEXTEDITDIALOG_H
#define TEXTEDITDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>

class TextEditDialog : public QDialog {
    Q_OBJECT

public:
    explicit TextEditDialog(const QString& text = QString(), 
                           const QString& fontFamily = QString(), 
                           int fontSize = 12,
                           QWidget *parent = nullptr);
    
    QString getText() const;
    QString getFontFamily() const;
    int getFontSize() const;
    
    void setText(const QString& text);
    void setFontFamily(const QString& family);
    void setFontSize(int size);
    
private:
    void setupUI();
    void applyDarkTheme();
    
    QTextEdit *m_textEdit;
    QComboBox *m_fontCombo;
    QSpinBox *m_fontSizeSpinbox;
    QPushButton *m_okButton;
    QPushButton *m_cancelButton;
};

#endif // TEXTEDITDIALOG_H
