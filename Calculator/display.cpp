#include "display.h"
#include "ui_display.h"
#include <QString>
#include <QApplication>
#include <QMessageBox>
#include <QtMath>
#include <QThread>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QLineEdit>

//including C++ headers
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <math.h>
using namespace std;

//Globel Variables
double FirstNum;
double SecondNum;
double Result;
QString FunitTaker;
QString LunitTaker;
bool userTypingSecondNumber = false;  ///For checking user input to enable or disable accroding to situations

Display::Display(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Display)
{
    ui->setupUi(this);

//    making checkable the operations buttons

//        disconnecting unarya
        disconnect(ui->ButtonP_M,SIGNAL(released()),this,SLOT(unary_operation_pressed()));
        disconnect(ui->ButtonPercentage,SIGNAL(released()),this,SLOT(unary_operation_pressed()));
//         disconnecting root squre and fraction
        disconnect(ui->ButtonBackSpace,SIGNAL(clicked()),this,SLOT(on_ButtonBackSpace_clicked()));
        disconnect(ui->ButtonSqure,SIGNAL(released()),this,SLOT(on_ButtonSqure_released()));
        disconnect(ui->ButtonFraction1,SIGNAL(released()),this,SLOT(on_ButtonFraction1_released()));
        disconnect(ui->ButtonRoot,SIGNAL(released()),this,SLOT(on_ButtonRoot_released()));
        disconnect(ui->ButtonDot,SIGNAL(released()),this,SLOT(on_ButtonDot_released()));
        disconnect(ui->buttonC,SIGNAL(released()),this,SLOT(on_buttonC_released()));
        disconnect(ui->ButtonConvert,SIGNAL(released()),this,SLOT(on_ButtonConvert_released()));

        ///unitbuttons disconnect to unitbuttonpressed
        ///
        disconnect(ui->ButtonMicro,SIGNAL(released()),this,SLOT(on_Unit_pressed()));
        disconnect(ui->ButtonMili,SIGNAL(released()),this,SLOT(on_Unit_pressed()));
        disconnect(ui->ButtonKilo,SIGNAL(released()),this,SLOT(on_Unit_pressed()));
        disconnect(ui->ButtonMega,SIGNAL(released()),this,SLOT(on_Unit_pressed()));

        //set Visible disable of convert combo buttom

//        ui->comboBox_2->setVisible(false);
////        ui->ButtonWatt->setVisible(false);
////        ui->ButtonVolt->setVisible(false);
////        ui->ButtonOhm->setVisible(false);
//        ui->ButtonConvert->setVisible(false);
        ui->radioButton->setVisible(false);



        //oNoff Button
        QPixmap pixmap("C:\\Users\\user\\Desktop\\ON.jpg");
        QIcon ButtonIcon(pixmap);
        ui->Button_ON->setIcon(ButtonIcon);
        ui->Button_ON->setIconSize(QSize(140,70));
        ui->Button_ON->setFixedSize(QSize(140,70));

        QPixmap pixmap1("C:\\Users\\user\\Desktop\\OFF.jpg");
        QIcon ButtonIcon1(pixmap1);
        ui->Button_OFF->setIcon(ButtonIcon1);
        ui->Button_OFF->setIconSize(QSize(140,70));
        ui->Button_OFF->setFixedSize(QSize(140,70));

        ui->Button_OFF->setVisible(false);

}

Display::~Display()
{
    delete ui;
}
///
/// \brief Display::digit_pressed
///If Number button is pressed then display the button text to screen
///
void Display::digit_pressed(){
   ui->comboBox->setVisible(true);
    QPushButton *button =(QPushButton *) sender();
    double labelNumber;
    QString newLabel;
//    if(ui->lcd_screen->text().contains("m")||ui->lcd_screen->text().contains("µ")){
//       ui->ButtonOhm->setVisible(false);
//    }
    if(ui->ButtonConvert->isChecked()){
        ui->lcd_screen->setText("0");
        ui->ButtonConvert->setCheckable(false);
    }

    if((ui->ButtonMin->isChecked()||ui->ButtonDevide->isChecked()||
        ui->ButtonPlus->isChecked()||ui->ButtonMult->isChecked())&& (!userTypingSecondNumber)){
        labelNumber = (button->text()).toDouble();
        userTypingSecondNumber = true;
        newLabel =QString::number(labelNumber,'g',11);
    }
    else{

        if((ui->ButtonEquals->isChecked())){
            ui->lcd_screen->setText("0");
        }

       if(ui->lcd_screen->text().contains('.') && button->text()=="0"){
           newLabel = ui->lcd_screen->text()+button->text();
           ui->ButtonEquals->setCheckable(false);

        }
        else{
               labelNumber = (ui->lcd_screen->text()+button->text()).toDouble();
                newLabel = QString::number(labelNumber,'g',11);
                ui->ButtonEquals->setCheckable(false);


        }


    }
        ui->lcd_screen->setText(newLabel);

}

///
/// \brief Display::on_ButtonDot_released
/// when dot button is pressed then puts dot in screen
///
void Display::on_ButtonDot_released()
{
    //if lcd_screen already contains the point then does nothing
    if(ui->lcd_screen->text().contains('.')){
      }
    else{  //gices the point after the text

    ui->lcd_screen->setText(ui->lcd_screen->text()+".");
    }
}

///
/// \brief Display::unary_operation_pressed
///Unary operation is for % and +/- buttons works as an unique
///
///
void Display:: unary_operation_pressed(){
    double labelNumber;
    QString newLabel;
    QPushButton *button = (QPushButton*) sender();

    if(button->text()== "+/-"){
        labelNumber = ui->lcd_screen->text().toDouble();
        labelNumber = labelNumber *(-1);
        newLabel  = QString::number(labelNumber,'g',11);
        ui->lcd_screen->setText(newLabel);
    }
    if(button->text()== "%"){
        labelNumber = ui->lcd_screen->text().toDouble();
        labelNumber = labelNumber *(0.01);
        newLabel  = QString::number(labelNumber,'g',11);
        ui->lcd_screen->setText(newLabel);
    }
}

///
/// \brief Display::on_buttonC_released
///Button C to Clear all
void Display::on_buttonC_released()
{
    ui->ButtonPlus->setChecked(false);
    ui->ButtonMin->setChecked(false);
    ui->ButtonMult->setChecked(false);
    ui->ButtonDevide->setChecked(false);
    ui->ButtonOhm->setVisible(true);
    ui->comboBox->setVisible(true);



    FirstNum =0;
   // ui->ButtonEquals->setChecked(false);

    userTypingSecondNumber =false;

    ui->lcd_screen->setText("0");

}
///
/// \brief Display::calculate_operations_pressed
///Contains the operation when  Calculate operation buttons like +,-,/,* is pressed
/// //text.chop is done when unit buttons pressd and is in screen to get the number Value only
/// if unit is like K M a are pressed then value is multiplied by its units comparing to deafult unit as mA
///
void Display::calculate_operations_pressed(){

    QPushButton *button =(QPushButton*) sender();

    if(ui->lcd_screen->text().contains("V")||ui->lcd_screen->text().contains("W")||ui->lcd_screen->text().contains("Ω")){
        ui->ButtonPlus->setCheckable(false);
        ui->ButtonMin->setCheckable(false);
        ui->ButtonMult->setCheckable(false);
       // ui->ButtonDevide->setChecked(false);
        //ui->ButtonEquals->setCheckable(false);
        FunitTaker =ui->lcd_screen->text();
        QString text = ui->lcd_screen->text();
        text.chop(1); //text.chop is don when unit buttons pressd and is in screen to get the number Value only
        if(text.contains("K")){
            text.chop(1);
            FirstNum = (text.toDouble()*1000); //if unit is K then value is multiplied by 1000
        }
        else if(text.contains("M")){
            text.chop(1);
            FirstNum = (text.toDouble()*1000000);

        }
        else if(text.contains("m")){
            text.chop(1);
           FirstNum = text.toDouble()/1000;
        }
        else if(text.contains("µ")){
            text.chop(1);
           FirstNum = text.toDouble()/1000000;
        }
        else{
            FirstNum = text.toDouble();
        }

    }
    ///for normal Calculation operations
    else{
        ui->ButtonPlus->setCheckable(true);
        ui->ButtonMin->setCheckable(true);
        ui->ButtonMult->setCheckable(true);
       FirstNum = ui->lcd_screen->text().toDouble();
    }


  // ui->lcd_screen->text().clear();
    button->setChecked(true);
//    ui->ButtonOhm->setVisible(true);

}

////
/// \brief Display::on_ButtonEquals_released
/// defines the work of equals buttons.Works when button value is realsed after pressing
/// //text.chop is done when unit buttons pressd and is in screen to get the number Value only
/// if unit is like K M a are pressed then value is multiplied by its units comparing to deafult unit as mA
///
void Display::on_ButtonEquals_released()
{
    userTypingSecondNumber= false;
    double labelNumbe;
    double answer;
    QString unit;
    QString newLable;
    LunitTaker = ui->lcd_screen->text();
    if(LunitTaker.contains(" ")){

    }
    if(ui->lcd_screen->text().contains("V")||ui->lcd_screen->text().contains("W")||
        ui->lcd_screen->text().contains("Ω")){
        QString text = ui->lcd_screen->text();
        text.chop(1);
     //   ui->lcd_screen->setText(text);
        if(text.contains("K")){
            text.chop(1);
            SecondNum = (text.toDouble()*1000);

        }
        else if(text.contains("M")){
            text.chop(1);
            SecondNum = (text.toDouble()*1000000);

        }
        else if(text.contains("m")){
            text.chop(1);
           SecondNum = text.toDouble()/1000;
        }
        else if(text.contains("µ")){
            text.chop(1);
           SecondNum = text.toDouble()/1000000;
        }
        else{
            SecondNum = text.toDouble();
        }

///if Current Calculation operation is performed then
        ///
        ///
        ///
        if(ui->ButtonOhm->isChecked()&&ui->ButtonVolt->isChecked()){

             if(FunitTaker.contains("V")){

                 labelNumbe = (FirstNum/SecondNum)*1000;
                // newLable = QString::number(labelNumbe);

             }
             else{
                 labelNumbe = (SecondNum/FirstNum)*1000;
                // newLable = QString::number(labelNumbe);
             }


             ui->ButtonOhm->setCheckable(false);
             ui->ButtonVolt->setCheckable(false);

             //ui->lcd_screen->setText(newLable+unit);
         }
        else if(ui->ButtonOhm->isChecked()&&ui->ButtonWatt->isChecked()){

            if(FunitTaker.contains("W")){

                labelNumbe = sqrt(FirstNum/SecondNum)*1000;
               // newLable = QString::number(labelNumbe);

            }
            else{
                labelNumbe = sqrt(SecondNum/FirstNum)*1000;
                //newLable = QString::number(labelNumbe);
            }
            ui->ButtonOhm->setCheckable(false);
            ui->ButtonVolt->setCheckable(false);
         //   QString unit = " mA";
           // ui->lcd_screen->setText(newLable+unit);

        }
        else if(ui->ButtonVolt->isChecked() && ui->ButtonWatt->isChecked()){

            if(FunitTaker.contains("W")){

                labelNumbe = (FirstNum/SecondNum)*1000;
               // newLable = QString::number(labelNumbe);

            }else{
                labelNumbe = (SecondNum/FirstNum)*1000;
               // newLable = QString::number(labelNumbe);

            }
            ui->ButtonVolt->setCheckable(false);
            ui->ButtonWatt->setCheckable(false);
            //QString unit = " mA";
           // ui->lcd_screen->setText(newLable+unit);

        }
      else{
               newLable = LunitTaker;   ///if  Calculation operation buttons are pressed then set screen as it is in screen
              // ui->lcd_screen->setText(newLable);
           }
        if(labelNumbe<1){
            answer =labelNumbe*1000;
             unit = " µA";
        }
        else if(labelNumbe>=1000 &&labelNumbe<1000000){
            answer = labelNumbe/1000;
            unit = " A";
        }
        else if(labelNumbe<1e+9&&labelNumbe>=1000000){
            answer = labelNumbe/1000000;
            unit = " KA";
        }
        else if(labelNumbe>=1e+9){
                answer = labelNumbe/1e+9;
                unit = " MA";
        }
        else{
            answer = labelNumbe;
            unit = " mA";

        }
        if(newLable!=LunitTaker){
            newLable = QString::number(answer,'g',3);
            ui->lcd_screen->setText(newLable+unit);


        }
        else{
            ui->lcd_screen->setText(newLable);

        }



    }
 ///if normal calculation operations id done
 ///
 ///
    else{
         SecondNum=  ui->lcd_screen->text().toDouble();

         if(ui->ButtonPlus->isChecked()){
            labelNumbe = FirstNum +SecondNum;
            if(ui->lcd_screen->text().contains('.')){
                 newLable =QString::number(labelNumbe,'g',8);
            }else{
            newLable =QString::number(labelNumbe,'g',11);
            }
            ui->lcd_screen->setText(newLable);
           ui->ButtonPlus->setChecked(false);
        }
        else if(ui->ButtonMin->isChecked()){
            labelNumbe = FirstNum - SecondNum;
            if(ui->lcd_screen->text().contains('.')){
                 newLable =QString::number(labelNumbe,'g',8);
            }else{
            newLable =QString::number(labelNumbe,'g',11);
            }
            ui->lcd_screen->setText(newLable);
            ui->ButtonMin->setChecked(false);
        }
        else if(ui->ButtonDevide->isChecked()){
            labelNumbe = FirstNum/SecondNum;
            if(ui->lcd_screen->text().contains('.')){
                 newLable =QString::number(labelNumbe,'g',8);
            }else{
            newLable =QString::number(labelNumbe,'g',11);
            }
            ui->lcd_screen->setText(newLable);
           ui->ButtonDevide->setChecked(false);
        }
        else if(ui->ButtonMult->isChecked()){

            labelNumbe = FirstNum * SecondNum;
            if(ui->lcd_screen->text().contains('.')){
                 newLable =QString::number(labelNumbe,'g',8);
            }else{
                  newLable =QString::number(labelNumbe,'g',11);
            }
            ui->lcd_screen->setText(newLable);
            ui->ButtonMult->setChecked(false);
        }
        else{
             newLable = ui->lcd_screen->text();
             ui->lcd_screen->setText(newLable);
         }
    }

     userTypingSecondNumber = false;
     ui->ButtonEquals->setCheckable(true);
}


///
/// \brief Display::on_ButtonBackSpace_clicked
/// When Button BackSpace is Clicked the removes the lallst letter from the screen
///

void Display::on_ButtonBackSpace_clicked()
{

    QString text = ui->lcd_screen->text();
    text.chop(1);
    if (text.isEmpty()||text.chopped(0) == '-') {
        text = "0";
    }
//    else if (text.chopped(1) == '-'){
//        text = "0";

//    }
    ui->lcd_screen->setText(text);
}
////
/// \brief Display::on_ButtonSqure_released
///does the unique operation and Squres the given Value
///
void Display::on_ButtonSqure_released()
{
    FirstNum = ui->lcd_screen->text().toDouble();

    double labelNumbe;
    QString newLable;
    labelNumbe = std::pow(FirstNum,2);
    newLable =QString::number(labelNumbe,'g',11);
    ui->lcd_screen->setText(newLable);
    ui->ButtonSqure->setChecked(false);
    userTypingSecondNumber= false;
}
/////
/// \brief Display::on_ButtonRoot_released
///Unique Gives the Squre root of Given input
void Display::on_ButtonRoot_released()
{
    FirstNum = ui->lcd_screen->text().toDouble();
    double labelNumbe;
    QString newLable;
    labelNumbe = std::sqrt(FirstNum);
    newLable =QString::number(labelNumbe,'g',11);
    ui->lcd_screen->setText(newLable);
  //  ui->ButtonSqure->setChecked(false);
  //  userTypingSecondNumber= false;
}

///
/// \brief Display::on_ButtonFraction1_released
/// Input value is devident of 1 and gives the value
void Display::on_ButtonFraction1_released()
{
    FirstNum = ui->lcd_screen->text().toDouble();
    double labelNumbe;
    QString newLable;
    labelNumbe = (1/FirstNum);
    newLable =QString::number(labelNumbe);
    ui->lcd_screen->setText(newLable);
    ui->ButtonSqure->setChecked(false);
    userTypingSecondNumber= false;
}

///
/// \brief Display::on_ButtonConvert_released
///-Convet button Converts the unit From ComboBox
///- ComboBox and ComboBox_2 is for the measurement Conversion
///- ComboBox_Current is for Current Conversion ...
///- Where ComboBox_Current convets the Calculated current Units
/// -measurement Conversion is made when value is inputed in screen and setting the
/// input unit and required unit to convert
///
void Display::on_ButtonConvert_released()
{
    QString Convert_unit = ui->comboBox->currentText();   //getting the current text in comboBox
    QString  Required_unit= ui->comboBox_2->currentText(); //getting the current text in comboBox
    QString lcdText = ui->lcd_screen->text();
 //if screen Doesnot contains the Current Calculation Value then measurement conversion can be made
    if(!lcdText.contains(" ")){
        FirstNum = ui->lcd_screen->text().toDouble();
        //if values are same
        if( Convert_unit == Required_unit){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum*1);
            newLable =QString::number(labelNumbe,'g',11);
            ui->lcd_screen->setText(newLable);

        }
        //converting into Kilometer to various
        if( Convert_unit =="K.M" && Required_unit== "M"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum*1000);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);
        }
        if( Convert_unit =="K.M" && Required_unit== "CM"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum*100000);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);
        }
        if( Convert_unit =="K.M" && Required_unit== "MM"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum*1e+6);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);
        }
        if( Convert_unit =="K.M" && Required_unit== "Mile"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/1.609);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);
        }
        if( Convert_unit =="K.M" && Required_unit== "Yard"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum*1093.61);
            newLable =QString::number(labelNumbe,'g',13);
            ui->lcd_screen->setText(newLable);
        }
        if( Convert_unit =="K.M" && Required_unit== "Feet"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum*3280.84);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);
        }
        if( Convert_unit =="K.M" && Required_unit== "Inch"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum*39370.08);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);
        }

///Converting Meter int others
///
///
///

        if( Convert_unit =="M" && Required_unit== "K.M"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/1000);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
        if( Convert_unit =="M" && Required_unit== "CM"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum*100);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
        if( Convert_unit =="M" && Required_unit== "MM"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum*1000);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
        if( Convert_unit =="M" && Required_unit== "Mile"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/1609.34);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
        if( Convert_unit =="M" && Required_unit== "Yard"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum*1.094);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
        if( Convert_unit =="M" && Required_unit== "Feet"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum*3.281);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
        if( Convert_unit =="K.M" && Required_unit== "Inch"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum*39.370);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
/////Converting Centimeter to others
///
///
///
        if( Convert_unit =="CM" && Required_unit== "K.M"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/100000);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
        if( Convert_unit =="CM" && Required_unit== "M"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/100);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
        if( Convert_unit =="CM" && Required_unit== "MM"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/10);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
        if( Convert_unit =="CM" && Required_unit== "Mile"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/160934);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
        if( Convert_unit =="CM" && Required_unit== "Yard"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/91.44);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
        if( Convert_unit =="CM" && Required_unit== "Feet"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/30.48);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
        if( Convert_unit =="CM" && Required_unit== "Inch"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/2.54);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }

////converting milimeter into others
///
///

        if( Convert_unit =="MM" && Required_unit== "K.M"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/1e+6);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
        if( Convert_unit =="MM" && Required_unit== "M"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/1000);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
        if( Convert_unit =="MM" && Required_unit== "CM"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/10);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
        if( Convert_unit =="MM" && Required_unit== "Mile"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/(1.609e+6));
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
        if( Convert_unit =="MM" && Required_unit== "Yard"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/914);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
        if( Convert_unit =="MM" && Required_unit== "Feet"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/305);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
        if( Convert_unit =="MM" && Required_unit== "Inch"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/25.4);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }

///Converting Mile into others
///
///
        if( Convert_unit =="Mile" && Required_unit== "K.M"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/1.60934);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);
        }
        if( Convert_unit =="Mile" && Required_unit== "M"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/1609.34);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);
        }
        if( Convert_unit =="Mile" && Required_unit== "CM"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/160934);
            newLable =QString::number(labelNumbe,'g',8);
            ui->lcd_screen->setText(newLable);
        }
        if( Convert_unit =="Mile" && Required_unit== "MM"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum*1.609e+6);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);
        }

        if( Convert_unit =="Mile" && Required_unit== "Yard"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum*1760);
            newLable =QString::number(labelNumbe,'g',13);
            ui->lcd_screen->setText(newLable);
        }
        if( Convert_unit =="Mile" && Required_unit== "Feet"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum*5280);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
        if( Convert_unit =="Mile" && Required_unit== "Inch"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum*63360);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
/////Converting
/// Yards to others
///
        if( Convert_unit =="Yard" && Required_unit== "K.M"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/1094);
           if(labelNumbe < 1){
                newLable =QString::number(labelNumbe,'f',4);
            }else{
                newLable =QString::number(labelNumbe,'g',10);
           }

            ui->lcd_screen->setText(newLable);
        }
        if( Convert_unit =="Yard" && Required_unit== "M"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/1.094);
            newLable =QString::number(labelNumbe,'g',8);
            ui->lcd_screen->setText(newLable);
        }
        if( Convert_unit =="Yard" && Required_unit== "CM"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/91.44);
            newLable =QString::number(labelNumbe,'g',8);
            ui->lcd_screen->setText(newLable);
        }
        if( Convert_unit =="Yard" && Required_unit== "MM"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum*914);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);
        }

        if( Convert_unit =="Yard" && Required_unit== "Mile"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/1760);
            newLable =QString::number(labelNumbe,'g',13);
            ui->lcd_screen->setText(newLable);

        }
        if( Convert_unit =="Yard" && Required_unit== "Feet"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum*3);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
        if( Convert_unit =="Yard" && Required_unit== "Inch"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum*36);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
////Converting F
/// Feet to others
///
///
        if( Convert_unit =="Feet" && Required_unit== "K.M"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/3281);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);
        }
        if( Convert_unit =="Feet" && Required_unit== "M"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/3.281);
            newLable =QString::number(labelNumbe,'g',8);
            ui->lcd_screen->setText(newLable);
        }
        if( Convert_unit =="Feet" && Required_unit== "CM"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/30.48);
            newLable =QString::number(labelNumbe,'g',8);
            ui->lcd_screen->setText(newLable);
        }
        if( Convert_unit =="Feet" && Required_unit== "MM"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum*305);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);
        }

        if( Convert_unit =="Feet" && Required_unit== "Mile"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/5280);
            newLable =QString::number(labelNumbe,'g',13);
            ui->lcd_screen->setText(newLable);

        }
        if( Convert_unit =="Feet" && Required_unit== "Yard"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/3);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
        if( Convert_unit =="Feet" && Required_unit== "Inch"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum*12);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
////Converting
/// Inch to various
///
        if( Convert_unit =="Inch" && Required_unit== "K.M"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/39370);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);
        }
        if( Convert_unit =="Inch" && Required_unit== "M"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/39.37);
            newLable =QString::number(labelNumbe,'g',8);
            ui->lcd_screen->setText(newLable);
        }
        if( Convert_unit =="Inch" && Required_unit== "CM"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/2.54);
            newLable =QString::number(labelNumbe,'g',8);
            ui->lcd_screen->setText(newLable);
        }
        if( Convert_unit =="Inch" && Required_unit== "MM"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum*25.4);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);
        }

        if( Convert_unit =="Inch" && Required_unit== "Mile"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/63360);
            newLable =QString::number(labelNumbe,'g',13);
            ui->lcd_screen->setText(newLable);
        }
        if( Convert_unit =="Inch" && Required_unit== "Yard"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/36);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
        if( Convert_unit =="Inch" && Required_unit== "Feet"){
            double labelNumbe;
            QString newLable;
            labelNumbe = (FirstNum/12);
            newLable =QString::number(labelNumbe,'g',10);
            ui->lcd_screen->setText(newLable);

        }
    }

//////Current Converting Part
///
///
    QString text = ui->comboBox_Current->currentText();   //gets the current unit in ComboBox
    if(text == "µA" ||text =="A"||text=="mA"||text=="KA"||text=="MA"){

 ///convetting the when Calculated Value result is in mA
 ///
 ///
        if(text== "mA"){
            double labelNumbe;
            QString newLable;
             if(ui->lcd_screen->text().contains(" mA")){
              }
            else if(ui->lcd_screen->text().contains(" µA")){
                QString text = ui->lcd_screen->text();
                text.chop(3);
                FirstNum = text.toDouble();

                labelNumbe = (FirstNum/1000);
                newLable =QString::number(labelNumbe,'g',10);
                ui->lcd_screen->setText(newLable+" mA");
            }
           else if(ui->lcd_screen->text().contains("A")){
                QString text = ui->lcd_screen->text();
                text.chop(2);
                FirstNum = text.toDouble();

                labelNumbe = (FirstNum*1000);
                newLable =QString::number(labelNumbe,'g',10);
                ui->lcd_screen->setText(newLable+" mA");
            }
            else if(ui->lcd_screen->text().contains(" KA")){
                QString text = ui->lcd_screen->text();
                text.chop(3);
                FirstNum = text.toDouble();

                labelNumbe = (FirstNum*1e+6);
                newLable =QString::number(labelNumbe,'g',10);
                ui->lcd_screen->setText(newLable+" mA");
            }
            else if(ui->lcd_screen->text().contains(" MA")){
                QString text = ui->lcd_screen->text();
                text.chop(3);
                FirstNum = text.toDouble();

                labelNumbe = (FirstNum*1e+9);
                newLable =QString::number(labelNumbe,'g',10);
                ui->lcd_screen->setText(newLable+" mA");
            }

        }

/// converting screen value to A
///
///
        else if(text== "A"){


            double labelNumbe;
            QString newLable;
            if(ui->lcd_screen->text().contains(" µA")){
                QString text = ui->lcd_screen->text();
                text.chop(3);
                FirstNum = text.toDouble();

                labelNumbe = (FirstNum/1000000);
                newLable =QString::number(labelNumbe,'g',10);
                ui->lcd_screen->setText(newLable+" A");
            }

           else if(ui->lcd_screen->text().contains(" mA")){
                QString text = ui->lcd_screen->text();
                text.chop(3);
                FirstNum = text.toDouble();

                labelNumbe = (FirstNum/1000);
                newLable =QString::number(labelNumbe,'g',10);
                ui->lcd_screen->setText(newLable+" A");
            }

            else if(ui->lcd_screen->text().contains(" KA")){
                QString text = ui->lcd_screen->text();
                text.chop(3);
                FirstNum = text.toDouble();

                labelNumbe = (FirstNum*1e+3);
                newLable =QString::number(labelNumbe,'g',10);
                ui->lcd_screen->setText(newLable+" A");
            }
            else if(ui->lcd_screen->text().contains(" MA")){
                QString text = ui->lcd_screen->text();
                text.chop(3);
                FirstNum = text.toDouble();

                labelNumbe = (FirstNum*1e+6);
                newLable =QString::number(labelNumbe,'g',10);
                ui->lcd_screen->setText(newLable+" A");
            }


        }

//// Converting Kilo Amper to others from screen
///
///
        else if(text== "KA"){
            double labelNumbe;
            QString newLable;
            if(ui->lcd_screen->text().contains(" µA")){
                QString text = ui->lcd_screen->text();
                text.chop(3);
                FirstNum = text.toDouble();

                labelNumbe = (FirstNum/1e+9);
                newLable =QString::number(labelNumbe,'g',10);
                ui->lcd_screen->setText(newLable+" KA");
            }

            else if(ui->lcd_screen->text().contains(" mA")){
                QString text = ui->lcd_screen->text();
                text.chop(3);
                FirstNum = text.toDouble();

                labelNumbe = (FirstNum/1e+6);
                newLable =QString::number(labelNumbe,'g',10);
                ui->lcd_screen->setText(newLable+" KA");
            }

           else if(ui->lcd_screen->text().contains(" A")){
                QString text = ui->lcd_screen->text();
                text.chop(2);
                FirstNum = text.toDouble();

                labelNumbe = (FirstNum/1e+3);
                newLable =QString::number(labelNumbe,'g',10);
                ui->lcd_screen->setText(newLable+" KA");
            }
           else if(ui->lcd_screen->text().contains(" MA")){
                QString text = ui->lcd_screen->text();
                text.chop(3);
                FirstNum = text.toDouble();

                labelNumbe = (FirstNum*1e+3);
                newLable =QString::number(labelNumbe,'g',10);
                ui->lcd_screen->setText(newLable+" KA");
            }

        }

///Converting Mega Ampere to others getting from screen
///
///
        else if(text== "MA"){


            double labelNumbe;
            QString newLable;
           if(ui->lcd_screen->text().contains(" µA")){
                QString text = ui->lcd_screen->text();
                text.chop(3);
                FirstNum = text.toDouble();

                labelNumbe = (FirstNum/1e+12);
                newLable =QString::number(labelNumbe,'g',10);
                ui->lcd_screen->setText(newLable+" MA");
            }

            else if(ui->lcd_screen->text().contains(" mA")){
                QString text = ui->lcd_screen->text();
                text.chop(3);
                FirstNum = text.toDouble();

                labelNumbe = (FirstNum/1e+9);
                newLable =QString::number(labelNumbe,'g',10);
                ui->lcd_screen->setText(newLable+" MA");
            }

            else if(ui->lcd_screen->text().contains(" A")){
                QString text = ui->lcd_screen->text();
                text.chop(2);
                FirstNum = text.toDouble();

                labelNumbe = (FirstNum/1e+6);
                newLable =QString::number(labelNumbe,'g',10);
                ui->lcd_screen->setText(newLable+" MA");
            }
            else if(ui->lcd_screen->text().contains(" KA")){
                QString text = ui->lcd_screen->text();
                text.chop(3);
                FirstNum = text.toDouble();

                labelNumbe = (FirstNum/1e+3);
                newLable =QString::number(labelNumbe,'g',10);
                ui->lcd_screen->setText(newLable+" MA");
            }

        }

 ///Converting micro ampere to others getting value form ans
 ///
 ///

        else if(text== "µA"){

            double labelNumbe;
            QString newLable;
            if(ui->lcd_screen->text().contains(" MA")){
                QString text = ui->lcd_screen->text();
                text.chop(3);
                FirstNum = text.toDouble();

                labelNumbe = (FirstNum*1e+12);
                newLable =QString::number(labelNumbe,'g',10);
                ui->lcd_screen->setText(newLable+" µA");
            }

            else if(ui->lcd_screen->text().contains("KA")){
                QString text = ui->lcd_screen->text();
                text.chop(3);
                FirstNum = text.toDouble();

                labelNumbe = (FirstNum*1e+9);
                newLable =QString::number(labelNumbe,'g',10);
                ui->lcd_screen->setText(newLable+" µA");
            }

           else if(ui->lcd_screen->text().contains(" A")){
                QString text = ui->lcd_screen->text();
                text.chop(2);
                FirstNum = text.toDouble();

                labelNumbe = (FirstNum*1e+6);
                newLable =QString::number(labelNumbe,'g',10);
                ui->lcd_screen->setText(newLable+" µA");
            }
           else if(ui->lcd_screen->text().contains(" mA")){
                QString text = ui->lcd_screen->text();
                text.chop(3);
                FirstNum = text.toDouble();

                labelNumbe = (FirstNum*1e+3);
                newLable =QString::number(labelNumbe,'g',10);
                ui->lcd_screen->setText(newLable+" µA");
            }
        }
    }
    userTypingSecondNumber = false;
    ui->ButtonConvert->setCheckable(true);
    ui->ButtonConvert->setChecked(true);
    ui->comboBox->setVisible(true);

}

///
/// \brief Display::on_pushButton_pressed
///onOff button enable the buttons when text is off and disable all when text is off..
///single press is off when as defult is ON

void Display::on_pushButton_pressed()
{
    if(ui->pushButton->text()=="ON"){
        ui->pushButton->setText("OFF");
             ui->lcd_screen->setText("!!!!!Welcome!!!!");
             QTimer::singleShot(1500,this,[=]{ui->lcd_screen->setText("0");});
             ui->ButtonPlus->setCheckable(true);
             ui->ButtonMin->setCheckable(true);
             ui->ButtonDevide->setCheckable(true);
             ui->ButtonMult->setCheckable(true);
             ui->ButtonEquals->setCheckable(true);
             ui->ButtonDot->setCheckable(true);
             ui->ButtonConvert->setCheckable(true);
             ui->comboBox->setVisible(true);
             ui->comboBox_2->setVisible(true);
             ui->ButtonWatt->setVisible(true);
             ui->ButtonVolt->setVisible(true);
//             ui->ButtonOhm->setVisible(true);



             /// connecting to ui
             ///

             connect(ui->Button0,SIGNAL(released()),this,SLOT(digit_pressed()));
             connect(ui->Button1,SIGNAL(released()),this,SLOT(digit_pressed()));
             connect(ui->Button2,SIGNAL(released()),this,SLOT(digit_pressed()));
             connect(ui->Button3,SIGNAL(released()),this,SLOT(digit_pressed()));
             connect(ui->Button4,SIGNAL(released()),this,SLOT(digit_pressed()));
             connect(ui->Button5,SIGNAL(released()),this,SLOT(digit_pressed()));
             connect(ui->Button6,SIGNAL(released()),this,SLOT(digit_pressed()));
             connect(ui->Button7,SIGNAL(released()),this,SLOT(digit_pressed()));
             connect(ui->Button8,SIGNAL(released()),this,SLOT(digit_pressed()));
             connect(ui->Button9,SIGNAL(released()),this,SLOT(digit_pressed()));

         //    connection to unary_operation_pressed() slot
             connect(ui->ButtonP_M,SIGNAL(released()),this,SLOT(unary_operation_pressed()));
             connect(ui->ButtonPercentage,SIGNAL(released()),this,SLOT(unary_operation_pressed()));

         //    connecting the buttons to calculate operation slot to for calculation operation
             connect(ui->ButtonPlus,SIGNAL(released()),this,SLOT(calculate_operations_pressed()));
             connect(ui->ButtonMin,SIGNAL(released()),this,SLOT(calculate_operations_pressed()));
             connect(ui->ButtonDevide,SIGNAL(released()),this,SLOT(calculate_operations_pressed()));
             connect(ui->ButtonMult,SIGNAL(released()),this,SLOT(calculate_operations_pressed()));
             connect(ui->ButtonEquals,SIGNAL(released()),this,SLOT(calculate_operations_pressed()));
     //        connecting unarya
             connect(ui->ButtonP_M,SIGNAL(released()),this,SLOT(unary_operation_pressed()));
             connect(ui->ButtonPercentage,SIGNAL(released()),this,SLOT(unary_operation_pressed()));
     //         connecting root squre and fraction
             connect(ui->ButtonBackSpace,SIGNAL(clicked()),this,SLOT(on_ButtonBackSpace_clicked()));
             connect(ui->ButtonSqure,SIGNAL(released()),this,SLOT(on_ButtonSqure_released()));
             connect(ui->ButtonFraction1,SIGNAL(released()),this,SLOT(on_ButtonFraction1_released()));
             connect(ui->ButtonRoot,SIGNAL(released()),this,SLOT(on_ButtonRoot_released()));
             connect(ui->ButtonDot,SIGNAL(released()),this,SLOT(on_ButtonDot_released()));
             connect(ui->buttonC,SIGNAL(released()),this,SLOT(on_buttonC_released()));
             connect(ui->ButtonConvert,SIGNAL(released()),this,SLOT(on_ButtonConvert_released()));


             ///unitbuttons disconnect to unitbuttonpressed
             ///
             connect(ui->ButtonMicro,SIGNAL(released()),this,SLOT(on_Unit_pressed()));
             connect(ui->ButtonMili,SIGNAL(released()),this,SLOT(on_Unit_pressed()));
             connect(ui->ButtonKilo,SIGNAL(released()),this,SLOT(on_Unit_pressed()));
             connect(ui->ButtonMega,SIGNAL(released()),this,SLOT(on_Unit_pressed()));
    }
    ////off button
    else{
        ui->pushButton->setText("ON");
        ui->ButtonPlus->setCheckable(false);
        ui->ButtonMin->setCheckable(false);
        ui->ButtonDevide->setCheckable(false);
        ui->ButtonMult->setCheckable(false);
        ui->ButtonEquals->setCheckable(false);
        ui->ButtonDot->setCheckable(false);
        ui->ButtonConvert->setCheckable(false);
//        ui->comboBox_2->setVisible(false);
//        ui->ButtonWatt->setVisible(false);
//        ui->ButtonVolt->setVisible(false);
//        ui->ButtonOhm->setVisible(false);

        /// Disconnectiong to ui
        ///

        disconnect(ui->Button0,SIGNAL(released()),this,SLOT(digit_pressed()));
        disconnect(ui->Button1,SIGNAL(released()),this,SLOT(digit_pressed()));
        disconnect(ui->Button2,SIGNAL(released()),this,SLOT(digit_pressed()));
        disconnect(ui->Button3,SIGNAL(released()),this,SLOT(digit_pressed()));
        disconnect(ui->Button4,SIGNAL(released()),this,SLOT(digit_pressed()));
        disconnect(ui->Button5,SIGNAL(released()),this,SLOT(digit_pressed()));
        disconnect(ui->Button6,SIGNAL(released()),this,SLOT(digit_pressed()));
        disconnect(ui->Button7,SIGNAL(released()),this,SLOT(digit_pressed()));
        disconnect(ui->Button8,SIGNAL(released()),this,SLOT(digit_pressed()));
        disconnect(ui->Button9,SIGNAL(released()),this,SLOT(digit_pressed()));

    //    connection to unary_operation_pressed() slot
        disconnect(ui->ButtonP_M,SIGNAL(released()),this,SLOT(unary_operation_pressed()));
        disconnect(ui->ButtonPercentage,SIGNAL(released()),this,SLOT(unary_operation_pressed()));
        disconnect(ui->ButtonDot,SIGNAL(released()),this,SLOT(on_ButtonDot_released()));



    //    connecting the buttons to calculate operation slot to for calculation operation
        disconnect(ui->ButtonPlus,SIGNAL(released()),this,SLOT(calculate_operations_pressed()));
        disconnect(ui->ButtonMin,SIGNAL(released()),this,SLOT(calculate_operations_pressed()));
        disconnect(ui->ButtonDevide,SIGNAL(released()),this,SLOT(calculate_operations_pressed()));
        disconnect(ui->ButtonMult,SIGNAL(released()),this,SLOT(calculate_operations_pressed()));
        disconnect(ui->ButtonEquals,SIGNAL(released()),this,SLOT(calculate_operations_pressed()));
//        disconnecting unarya
        disconnect(ui->ButtonP_M,SIGNAL(released()),this,SLOT(unary_operation_pressed()));
        disconnect(ui->ButtonPercentage,SIGNAL(released()),this,SLOT(unary_operation_pressed()));
//         disconnecting root squre and fraction
        disconnect(ui->ButtonBackSpace,SIGNAL(clicked()),this,SLOT(on_ButtonBackSpace_clicked()));
        disconnect(ui->ButtonSqure,SIGNAL(released()),this,SLOT(on_ButtonSqure_released()));
        disconnect(ui->ButtonFraction1,SIGNAL(released()),this,SLOT(on_ButtonFraction1_released()));
        disconnect(ui->ButtonRoot,SIGNAL(released()),this,SLOT(on_ButtonRoot_released()));
        disconnect(ui->ButtonDot,SIGNAL(released()),this,SLOT(on_ButtonDot_released()));
        disconnect(ui->buttonC,SIGNAL(released()),this,SLOT(on_buttonC_released()));
        disconnect(ui->ButtonConvert,SIGNAL(released()),this,SLOT(on_ButtonConvert_released()));

        ///unitbuttons disconnect to unitbuttonpressed
        ///
        disconnect(ui->ButtonMicro,SIGNAL(released()),this,SLOT(on_Unit_pressed()));
        disconnect(ui->ButtonMili,SIGNAL(released()),this,SLOT(on_Unit_pressed()));
        disconnect(ui->ButtonKilo,SIGNAL(released()),this,SLOT(on_Unit_pressed()));
        disconnect(ui->ButtonMega,SIGNAL(released()),this,SLOT(on_Unit_pressed()));

        ///Screen View
        ui->lcd_screen->setText("Good Bye!!!!!!!!");
        QTimer::singleShot(1500,this,[=]{ui->lcd_screen->setText("");}); ///clears the screen after 1.5 sec

    }
}

///Gives the letter Ω to the screen if there are no other Unit buttons
///
///
void Display::on_ButtonOhm_released()
{
    //if lcd_screen already contains the Ω then does nothing
    if(ui->lcd_screen->text().contains("W")||ui->lcd_screen->text().contains("V")||ui->lcd_screen->text().contains("Ω")){
      }
    else{  //gices the Ω after the text

    ui->lcd_screen->setText(ui->lcd_screen->text()+"Ω");
    }
    ui->ButtonOhm->setCheckable(true);
    ui->ButtonOhm->setChecked(true);
}

///Gives the letter V to the screen if there are no other Unit buttons
///
///
void Display::on_ButtonVolt_released()
{
    //if lcd_screen already contains the V then does nothing
    if(ui->lcd_screen->text().contains("W")||ui->lcd_screen->text().contains("V")||ui->lcd_screen->text().contains("Ω")){
      }
    else{  //gices the Ω after the text

    ui->lcd_screen->setText(ui->lcd_screen->text()+"V");
    }
    ui->ButtonVolt->setCheckable(true);
    ui->ButtonVolt->setChecked(true);
}

///Gives the letter W to the screen if there are no other Unit buttons
///
///
void Display::on_ButtonWatt_released()
{
    //if lcd_screen already contains the W then does nothing
    if(ui->lcd_screen->text().contains("W")||ui->lcd_screen->text().contains("V")||ui->lcd_screen->text().contains("Ω")){
      }
    else{  //gices the Ω after the text

    ui->lcd_screen->setText(ui->lcd_screen->text()+"W");
    }
    ui->ButtonWatt->setCheckable(true);
    ui->ButtonWatt->setChecked(true);


}



////Image button Which trigers the ON Button behind the on off Image  buttons
///
///
void Display::on_Button_ON_clicked()
{
    //ui->pushButton->setText("OFF");
    ui->pushButton->pressed();   ///on off push button is pressed
    ui->Button_OFF->setVisible(true);
    ui->Button_ON->setVisible(false);
  //  ui->ButtonConvert->setVisible(true);
  //  ui->ButtonMicro->setVisible(true);
  //  ui->ButtonMili->setVisible(true);

}

///if off image button is clicked
/// trigers the on button behind the on of button

void Display::on_Button_OFF_clicked()
{
    ui->pushButton->pressed();
    ui->Button_OFF->setVisible(false);
    ui->Button_ON->setVisible(true);
}

//// Unit pressed is used for buttons value of Current "am","AM","KM" etc..
///
///
void Display::on_Unit_pressed(){

    QPushButton *button =(QPushButton *) sender();
    QString newLabel;
    //if lcd_screen already contains the W then does nothing
    if(ui->lcd_screen->text().contains("W")||ui->lcd_screen->text().contains("V")||ui->lcd_screen->text().contains("Ω")){
      }
    else if(ui->lcd_screen->text().contains("µ")||ui->lcd_screen->text().contains("m")||
            ui->lcd_screen->text().contains("K")||ui->lcd_screen->text().contains("M")){

    }
    else{  //gices the Ω after the text

        ui->lcd_screen->setText(ui->lcd_screen->text()+ button->text());

    }

    ui->ButtonWatt->setCheckable(true);
    ui->ButtonWatt->setChecked(true);


}














//unused radio button for on offf was commented
/*
void Display::on_radioButton_clicked()
{
    if(ui->radioButton->isChecked()){
         ui->lcd_screen->setText("WELCOME");
         QTimer::singleShot(1000,this,[=]{ui->lcd_screen->setText("0");});
         ui->ButtonPlus->setCheckable(true);
         ui->ButtonMin->setCheckable(true);
         ui->ButtonDevide->setCheckable(true);
         ui->ButtonMult->setCheckable(true);
         ui->ButtonEquals->setCheckable(true);
         ui->ButtonDot->setCheckable(true);
         ui->ButtonConvert->setCheckable(true);


         /// connecting to ui
         ///

         connect(ui->Button0,SIGNAL(released()),this,SLOT(digit_pressed()));
         connect(ui->Button1,SIGNAL(released()),this,SLOT(digit_pressed()));
         connect(ui->Button2,SIGNAL(released()),this,SLOT(digit_pressed()));
         connect(ui->Button3,SIGNAL(released()),this,SLOT(digit_pressed()));
         connect(ui->Button4,SIGNAL(released()),this,SLOT(digit_pressed()));
         connect(ui->Button5,SIGNAL(released()),this,SLOT(digit_pressed()));
         connect(ui->Button6,SIGNAL(released()),this,SLOT(digit_pressed()));
         connect(ui->Button7,SIGNAL(released()),this,SLOT(digit_pressed()));
         connect(ui->Button8,SIGNAL(released()),this,SLOT(digit_pressed()));
         connect(ui->Button9,SIGNAL(released()),this,SLOT(digit_pressed()));

     //    connection to unary_operation_pressed() slot
         connect(ui->ButtonP_M,SIGNAL(released()),this,SLOT(unary_operation_pressed()));
         connect(ui->ButtonPercentage,SIGNAL(released()),this,SLOT(unary_operation_pressed()));

     //    connecting the buttons to calculate operation slot to for calculation operation
         connect(ui->ButtonPlus,SIGNAL(released()),this,SLOT(calculate_operations_pressed()));
         connect(ui->ButtonMin,SIGNAL(released()),this,SLOT(calculate_operations_pressed()));
         connect(ui->ButtonDevide,SIGNAL(released()),this,SLOT(calculate_operations_pressed()));
         connect(ui->ButtonMult,SIGNAL(released()),this,SLOT(calculate_operations_pressed()));
         connect(ui->ButtonEquals,SIGNAL(released()),this,SLOT(calculate_operations_pressed()));
 //        connecting unarya
         connect(ui->ButtonP_M,SIGNAL(released()),this,SLOT(unary_operation_pressed()));
         connect(ui->ButtonPercentage,SIGNAL(released()),this,SLOT(unary_operation_pressed()));
 //         connecting root squre and fraction
         connect(ui->ButtonBackSpace,SIGNAL(clicked()),this,SLOT(on_ButtonBackSpace_clicked()));
         connect(ui->ButtonSqure,SIGNAL(released()),this,SLOT(on_ButtonSqure_released()));
         connect(ui->ButtonFraction1,SIGNAL(released()),this,SLOT(on_ButtonFraction1_released()));
         connect(ui->ButtonRoot,SIGNAL(released()),this,SLOT(on_ButtonRoot_released()));
         connect(ui->ButtonRoot,SIGNAL(released()),this,SLOT(on_ButtonRoot_released()));
         connect(ui->ButtonDot,SIGNAL(released()),this,SLOT(on_ButtonDot_released()));
         connect(ui->buttonC,SIGNAL(released()),this,SLOT(on_buttonC_released()));
         connect(ui->ButtonConvert,SIGNAL(clicked()),this,SLOT(on_ButtonConvert_clicked()));





    }
    else{
        ui->lcd_screen->setText("Good Bye");
        QTimer::singleShot(1000,this,[=]{ui->lcd_screen->setText("");});
        ui->ButtonPlus->setCheckable(false);
        ui->ButtonMin->setCheckable(false);
        ui->ButtonDevide->setCheckable(false);
        ui->ButtonMult->setCheckable(false);
        ui->ButtonEquals->setCheckable(false);
        ui->ButtonDot->setCheckable(false);
        ui->ButtonConvert->setCheckable(false);



        /// connecting to ui
        ///

        disconnect(ui->Button0,SIGNAL(released()),this,SLOT(digit_pressed()));
        disconnect(ui->Button1,SIGNAL(released()),this,SLOT(digit_pressed()));
        disconnect(ui->Button2,SIGNAL(released()),this,SLOT(digit_pressed()));
        disconnect(ui->Button3,SIGNAL(released()),this,SLOT(digit_pressed()));
        disconnect(ui->Button4,SIGNAL(released()),this,SLOT(digit_pressed()));
        disconnect(ui->Button5,SIGNAL(released()),this,SLOT(digit_pressed()));
        disconnect(ui->Button6,SIGNAL(released()),this,SLOT(digit_pressed()));
        disconnect(ui->Button7,SIGNAL(released()),this,SLOT(digit_pressed()));
        disconnect(ui->Button8,SIGNAL(released()),this,SLOT(digit_pressed()));
        disconnect(ui->Button9,SIGNAL(released()),this,SLOT(digit_pressed()));

    //    connection to unary_operation_pressed() slot
        disconnect(ui->ButtonP_M,SIGNAL(released()),this,SLOT(unary_operation_pressed()));
        disconnect(ui->ButtonPercentage,SIGNAL(released()),this,SLOT(unary_operation_pressed()));
        disconnect(ui->ButtonDot,SIGNAL(released()),this,SLOT(on_ButtonDot_released()));



    //    connecting the buttons to calculate operation slot to for calculation operation
        disconnect(ui->ButtonPlus,SIGNAL(released()),this,SLOT(calculate_operations_pressed()));
        disconnect(ui->ButtonMin,SIGNAL(released()),this,SLOT(calculate_operations_pressed()));
        disconnect(ui->ButtonDevide,SIGNAL(released()),this,SLOT(calculate_operations_pressed()));
        disconnect(ui->ButtonMult,SIGNAL(released()),this,SLOT(calculate_operations_pressed()));
        disconnect(ui->ButtonEquals,SIGNAL(released()),this,SLOT(calculate_operations_pressed()));
//        disconnecting unarya
        disconnect(ui->ButtonP_M,SIGNAL(released()),this,SLOT(unary_operation_pressed()));
        disconnect(ui->ButtonPercentage,SIGNAL(released()),this,SLOT(unary_operation_pressed()));
//         disconnecting root squre and fraction
        disconnect(ui->ButtonBackSpace,SIGNAL(clicked()),this,SLOT(on_ButtonBackSpace_clicked()));
        disconnect(ui->ButtonSqure,SIGNAL(released()),this,SLOT(on_ButtonSqure_released()));
        disconnect(ui->ButtonFraction1,SIGNAL(released()),this,SLOT(on_ButtonFraction1_released()));
        disconnect(ui->ButtonRoot,SIGNAL(released()),this,SLOT(on_ButtonRoot_released()));
        disconnect(ui->ButtonDot,SIGNAL(released()),this,SLOT(on_ButtonDot_released()));
        disconnect(ui->buttonC,SIGNAL(released()),this,SLOT(on_buttonC_released()));
        disconnect(ui->ButtonConvert,SIGNAL(clicked()),this,SLOT(on_ButtonConvert_clicked()));






    }

}
*/


