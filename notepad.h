//
// Created by gxzc on 2023/9/18.
//

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Notepad; }
QT_END_NAMESPACE


#ifndef NOTEPAD_NOTEPAD_H
#define NOTEPAD_NOTEPAD_H


class Notepad : public QMainWindow{
    Q_OBJECT

public:
    explicit Notepad(QWidget *parent = nullptr);
    ~Notepad();

private:
    Ui::Notepad *ui;
    QString currentFile;

    void newDocument();

    void open();

    void save();

    void saveAs();

    void print();

    void selectFont();

    void setFontBold(bool bold);

    void setFontItalic(bool italic);

    void setFontUnderline(bool underline);

    void about();

};


#endif //NOTEPAD_NOTEPAD_H
