#include "TextEditDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QFontDatabase>

TextEditDialog::TextEditDialog(const QString& text, const QString& fontFamily, int fontSize, QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    applyDarkTheme();
    
    // Устанавливаем значения
    if (!text.isEmpty()) {
        m_textEdit->setPlainText(text);
    }
    
    if (!fontFamily.isEmpty()) {
        int fontIndex = m_fontCombo->findText(fontFamily);
        if (fontIndex >= 0) {
            m_fontCombo->setCurrentIndex(fontIndex);
        }
    }
    
    if (fontSize > 0) {
        m_fontSizeSpinbox->setValue(fontSize);
    }
}

void TextEditDialog::setupUI() {
    setWindowTitle("Edit PDF Text");
    setModal(true);
    setMinimumSize(600, 400);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // Заголовок
    QLabel *titleLabel = new QLabel("📝 Edit Text Content", this);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #FFFFFF;");
    mainLayout->addWidget(titleLabel);
    
    // Текстовое поле
    QLabel *textLabel = new QLabel("Text:", this);
    textLabel->setStyleSheet("color: #CCCCCC; font-size: 12px;");
    mainLayout->addWidget(textLabel);
    
    m_textEdit = new QTextEdit(this);
    m_textEdit->setMinimumHeight(200);
    mainLayout->addWidget(m_textEdit);
    
    // Настройки форматирования
    QFormLayout *formatLayout = new QFormLayout();
    formatLayout->setSpacing(10);
    
    // Шрифт
    m_fontCombo = new QComboBox(this);
    m_fontCombo->addItems(QFontDatabase::families());
    formatLayout->addRow("Font Family:", m_fontCombo);
    
    // Размер шрифта
    m_fontSizeSpinbox = new QSpinBox(this);
    m_fontSizeSpinbox->setRange(6, 72);
    m_fontSizeSpinbox->setValue(12);
    m_fontSizeSpinbox->setSuffix(" pt");
    formatLayout->addRow("Font Size:", m_fontSizeSpinbox);
    
    mainLayout->addLayout(formatLayout);
    
    mainLayout->addStretch();
    
    // Кнопки
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    m_cancelButton = new QPushButton("Cancel", this);
    m_cancelButton->setMinimumWidth(100);
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    buttonLayout->addWidget(m_cancelButton);
    
    m_okButton = new QPushButton("Apply Changes", this);
    m_okButton->setMinimumWidth(120);
    m_okButton->setDefault(true);
    connect(m_okButton, &QPushButton::clicked, this, &QDialog::accept);
    buttonLayout->addWidget(m_okButton);
    
    mainLayout->addLayout(buttonLayout);
}

void TextEditDialog::applyDarkTheme() {
    setStyleSheet(R"(
        QDialog {
            background-color: #1E1E1E;
        }
        
        QLabel {
            color: #CCCCCC;
        }
        
        QTextEdit {
            background-color: #2D2D2D;
            color: #FFFFFF;
            border: 1px solid #3A3A3A;
            border-radius: 4px;
            padding: 8px;
            font-family: 'Segoe UI', Arial;
            font-size: 12px;
        }
        
        QTextEdit:focus {
            border: 1px solid #0078D4;
        }
        
        QComboBox, QSpinBox {
            background-color: #2D2D2D;
            color: #FFFFFF;
            border: 1px solid #3A3A3A;
            border-radius: 4px;
            padding: 5px 10px;
            min-height: 25px;
        }
        
        QComboBox:hover, QSpinBox:hover {
            border: 1px solid #505050;
        }
        
        QComboBox:focus, QSpinBox:focus {
            border: 1px solid #0078D4;
        }
        
        QComboBox::drop-down {
            border: none;
            width: 20px;
        }
        
        QComboBox::down-arrow {
            image: none;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 5px solid #FFFFFF;
            margin-right: 5px;
        }
        
        QComboBox QAbstractItemView {
            background-color: #2D2D2D;
            color: #FFFFFF;
            selection-background-color: #0078D4;
            border: 1px solid #3A3A3A;
        }
        
        QSpinBox::up-button, QSpinBox::down-button {
            background-color: #3A3A3A;
            border: none;
            width: 16px;
        }
        
        QSpinBox::up-button:hover, QSpinBox::down-button:hover {
            background-color: #505050;
        }
        
        QSpinBox::up-arrow {
            image: none;
            border-left: 4px solid transparent;
            border-right: 4px solid transparent;
            border-bottom: 4px solid #FFFFFF;
        }
        
        QSpinBox::down-arrow {
            image: none;
            border-left: 4px solid transparent;
            border-right: 4px solid transparent;
            border-top: 4px solid #FFFFFF;
        }
        
        QPushButton {
            background-color: #0078D4;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 8px 16px;
            font-weight: bold;
        }
        
        QPushButton:hover {
            background-color: #1084D8;
        }
        
        QPushButton:pressed {
            background-color: #006CBE;
        }
        
        QPushButton#cancelButton {
            background-color: #3A3A3A;
        }
        
        QPushButton#cancelButton:hover {
            background-color: #505050;
        }
    )");
    
    m_cancelButton->setObjectName("cancelButton");
}

QString TextEditDialog::getText() const {
    return m_textEdit->toPlainText();
}

QString TextEditDialog::getFontFamily() const {
    return m_fontCombo->currentText();
}

int TextEditDialog::getFontSize() const {
    return m_fontSizeSpinbox->value();
}

void TextEditDialog::setText(const QString& text) {
    m_textEdit->setPlainText(text);
}

void TextEditDialog::setFontFamily(const QString& family) {
    int index = m_fontCombo->findText(family);
    if (index >= 0) {
        m_fontCombo->setCurrentIndex(index);
    }
}

void TextEditDialog::setFontSize(int size) {
    m_fontSizeSpinbox->setValue(size);
}
