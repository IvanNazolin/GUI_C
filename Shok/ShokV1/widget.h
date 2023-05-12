
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>



QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget

{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_plainTextEdit_blockCountChanged(int newBlockCount);

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_Button5_clicked();

    void on_Button5_clicked(bool checked);

    void on_btn1_clicked();

    void on_btn2_clicked();

    void on_btn2_2_clicked();

    void on_button_fill_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
