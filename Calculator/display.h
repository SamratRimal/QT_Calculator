#ifndef DISPLAY_H
#define DISPLAY_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Display; }
QT_END_NAMESPACE

class Display : public QMainWindow
{
    Q_OBJECT

public:
    Display(QWidget *parent = nullptr);
    ~Display();

private slots:
    void digit_pressed();

    void on_ButtonDot_released();

    void unary_operation_pressed();

    void on_buttonC_released();

    void calculate_operations_pressed();

    void on_ButtonEquals_released();

    void on_ButtonBackSpace_clicked();

    void on_ButtonSqure_released();

    void on_ButtonRoot_released();

    void on_ButtonFraction1_released();

    void on_ButtonConvert_released();

    void on_pushButton_pressed();

    void on_ButtonOhm_released();

    void on_ButtonVolt_released();

    void on_ButtonWatt_released();

    void on_Button_ON_clicked();

    void on_Button_OFF_clicked();

    void on_Unit_pressed();

private:
    Ui::Display *ui;

};
#endif // DISPLAY_H
