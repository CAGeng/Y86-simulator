#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QPushButton>
#include<QLineEdit>
#include<QLabel>
#include<QTextEdit>
#include<QString>
#include<QFont>
#include<QLineEdit>
#include<QPalette>
#include<QTimer>
#include<QTime>
#include<QSlider>
#include<QComboBox>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    Ui::Widget *ui;
    QFont font,font2,font3,font4;
    QPalette pe,pe2,pe3,pe4;
    QPushButton step;
    QPushButton go,OK,DEC;
    QLabel l1,l2,l3,l4,l5,l6,l7,l8,CCl[3];
    QLabel Dl[7],dl[7],El[10],el[10],Ml[7],ml[7],Wl[6],wl[6];
    QLabel n_Dl[7],n_dl[7],n_El[10],n_el[10],n_Ml[7],n_ml[7],n_Wl[6],n_wl[6];
    QLabel clockl,PCl,REGl[16],n_reg[16];
    QLabel D,E,M,W,OP,R;
    QLineEdit edit1;
    QTimer *timer;
    QSlider slider;
    QComboBox choice;
    int v;
    void set_label();
    void myslot_step();
    void myslot_ok();
    void myslot_go();
    void myslot_pause();
    void myslot_DECHEX();
    void changeV();
    void setlabel(QLabel&,int,int,QString,int);
    void setbutton(QPushButton&,int,int, QString,int);
    void clear_all();
};
#endif // WIDGET_H
