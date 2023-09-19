//
// Created by gxzc on 2023/9/18.
//

#include <QFileDialog>
#include <QMessageBox>
#include <QFontDialog>
#include "notepad.h"
#include "ui_notepad.h"

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printer)
#if QT_CONFIG(printdialog)
#include <QPrintDialog>
#endif // QT_CONFIG(printdialog)
#include <QPrinter>
#endif // QT_CONFIG(printer)
#endif // QT_PRINTSUPPORT_LIB

Notepad::Notepad(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::Notepad) {
    ui->setupUi(this);
    connect(ui->actionNew, &QAction::triggered, this, &Notepad::newDocument);
    connect(ui->actionOpen, &QAction::triggered, this, &Notepad::open);
    connect(ui->actionSave, &QAction::triggered, this, &Notepad::save);
    connect(ui->actionSave_as, &QAction::triggered, this, &Notepad::saveAs);
    connect(ui->actionPrint, &QAction::triggered, this, &Notepad::print);
#if  QT_CONFIG(clipboard)
    connect(ui->textEdit, &QTextEdit::copyAvailable, ui->actionCopy, &QAction::setEnabled);
    connect(ui->actionCopy, &QAction::triggered, ui->textEdit, &QTextEdit::copy);
    connect(ui->actionCut, &QAction::triggered, ui->textEdit, &QTextEdit::cut);
    connect(ui->actionPaste, &QAction::triggered, ui->textEdit, &QTextEdit::paste);
#endif
    connect(ui->textEdit, &QTextEdit::undoAvailable, ui->actionUndo, &QAction::setEnabled);
    connect(ui->actionUndo, &QAction::triggered, ui->textEdit, &QTextEdit::undo);
    connect(ui->textEdit, &QTextEdit::redoAvailable, ui->actionRedo, &QAction::setEnabled);
    connect(ui->actionRedo, &QAction::triggered, ui->textEdit, &QTextEdit::redo);
    connect(ui->actionBold, &QAction::triggered, this, &Notepad::setFontBold);
    connect(ui->actionItalic, &QAction::triggered, this, &Notepad::setFontItalic);
    connect(ui->actionUnderline, &QAction::triggered, this, &Notepad::setFontUnderline);
    connect(ui->actionFont, &QAction::triggered, this, &Notepad::selectFont);
    connect(ui->actionAbout, &QAction::triggered, this, &Notepad::about);
    connect(ui->actionExit, &QAction::triggered, this, &Notepad::close);


}

Notepad::~Notepad() {
    delete ui;
}

void Notepad::newDocument() {
    currentFile.clear();
    ui->textEdit->setText(QString());
}

void Notepad::open() {
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件");

    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "警告", "无法打开文件：" + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}

void Notepad::save() {
    QString fileName;

    if (currentFile.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this, "保存");
        if (fileName.isEmpty())
            return;
        currentFile = fileName;
    } else {
        fileName = currentFile;
    }
    QFile file(currentFile);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "警告", "无法保存文件:" + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}

void Notepad::saveAs() {
    QString fileName = QFileDialog::getSaveFileName(this, "另存为");
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "警告", "无法保存文件:" + file.errorString());
        return;
    }
    currentFile = fileName;
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}

void Notepad::print() {
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printer)
    QPrinter printDev;
#if QT_CONFIG(printdialog)
    QPrintDialog dialog(&printDev, this);
    if (dialog.exec() == QDialog::Rejected)
        return;
#endif // QT_CONFIG(printdialog)
    ui->textEdit->print(&printDev);
#endif // QT_CONFIG(printer)
}

void Notepad::selectFont() {
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, this);
    if (fontSelected)
        ui->textEdit->setFont(font);
}

void Notepad::setFontBold(bool bold) {
    QFont font = ui->textEdit->font();
    font.setBold(bold);
    ui->textEdit->setFont(font);
}

void Notepad::setFontItalic(bool italic) {
    QFont font = ui->textEdit->font();
    font.setItalic(italic);
    ui->textEdit->setFont(font);
}

void Notepad::setFontUnderline(bool underline) {
    QFont font = ui->textEdit->font();
    font.setUnderline(underline);
    ui->textEdit->setFont(font);
}

void Notepad::about() {
    QMessageBox::about(this, tr("关于Notepad"), tr("使用Clion QtWidgets编写的记事本"));
}
