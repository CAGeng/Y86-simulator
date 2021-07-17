#include "widget.h"
#include "ui_widget.h"
#include "pipe.cpp"
#include<QFile>
#include<QLineEdit>
#include<QDateTime>
#include<QFont>
int lef = 100,show_hex = 1;
void run_prepare(){
    switch(id){
        case 1: f.open("./yo/prog1.yo");break;
        case 2: f.open("./yo/prog2.yo");break;
        case 3: f.open("./yo/prog3.yo");break;
        case 4: f.open("./yo/prog4.yo");break;
        case 5: f.open("./yo/prog5.yo");break;
        case 6: f.open("./yo/prog6.yo");break;
        case 7: f.open("./yo/prog7.yo");break;
        case 8: f.open("./yo/prog8.yo");break;
        case 9: f.open("./yo/prog9.yo");break;
        case 10: f.open("./yo/prog10.yo");break;
        case 11: f.open("./yo/pushquestion.yo");break;
        case 12: f.open("./yo/pushtest.yo");break;
        case 13: f.open("./yo/ret-hazard.yo");break;
        case 14: f.open("./yo/asum.yo");break;
        case 15:f.open("./yo/cjr.yo");break;
        case 16:f.open("./yo/j-cc.yo");break;
        case 17: f.open("./yo/abs-asum-cmov.yo");break;
        case 18: f.open("./yo/abs-asum-jmp.yo");break;
    }
    printf("Begin to test prog%d......\n",id);
    init();
    read_all_code();
    PC_end = PC;
    PC = 0;
    lef = 100;
    f.close();
}
void Widget::setlabel(QLabel& l,int x,int y, QString str,int f){
    l.setParent(this);
    l.move(x,y);
    l.setText(str);
    if(f == 1){l.setFont(font); l.setPalette(pe);}
    else if(f == 2){l.setFont(font2);l.setPalette(pe2);}
    else if(f == 3){l.setFont(font3);l.setPalette(pe3);}
    else if(f == 4){l.setFont(font4);l.setPalette(pe4);}
}
void Widget::setbutton(QPushButton& l,int x,int y, QString str,int f){
    l.setParent(this);
    l.move(x,y);
    l.setText(str);
    if(f == 1)l.setFont(font);
    else if(f == 2)l.setFont(font2);
    else if(f == 3){l.setFont(font3);l.setPalette(pe3);}
}


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("Y86_simulater");
    setAutoFillBackground(true);
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background,QBrush(QPixmap("./pictures/2.jpg")));
    setPalette(pal);
    font.setPointSize(12);
    font2.setPointSize(10);
    font3.setPointSize(15);
    font4.setPointSize(20);
    pe.setColor(QPalette::WindowText, Qt::yellow);
    pe2.setColor(QPalette::WindowText, Qt::red);
    pe3.setColor(QPalette::WindowText, Qt::white);
    pe4.setColor(QPalette::WindowText, Qt::gray);
    this->setFixedSize(1200,650);
    edit1.setParent(this);
    edit1.move(1000,10);
    slider.setParent(this);
    slider.move(1110,250);
    choice.setParent(this);
    choice.move(1000,40);
    choice.addItem("prog1");
    choice.addItem("prog2");
    choice.addItem("prog3");
    choice.addItem("prog4");
    choice.addItem("prog5");
    choice.addItem("prog6");
    choice.addItem("prog7");
    choice.addItem("prog8");
    choice.addItem("prog9");
    choice.addItem("prog10");
    choice.addItem("pushquestion");
    choice.addItem("pushtest");
    choice.addItem("ret-hazard");
    choice.addItem("asum");
    choice.addItem("cjr");
    choice.addItem("j-cc");
    choice.addItem("abs-asum-cmov");
    choice.addItem("abs-asum-jmp");
    choice.addItem("....");
    setbutton(go,1110,100,"go",1);
    setbutton(step,1110,150,"step",1);
    setbutton(DEC,1110 ,200,"DEC",1);
    setbutton(OK,1000,70,"OK",1);
    setlabel(clockl,300,10,"               ",2);
    setlabel(PCl,290,30,"                ",2);
    setlabel(l1,230,0,"clock:",3);
    setlabel(l2,240,20,"PC:",3);
    setlabel(l3,300,50,"no_running",1);
    setlabel(l4,960,10,"ID:",1);
    setlabel(l5,230,50,"message:",1);
    //setlabel(OP,900,10,"Console",4);
    setlabel(CCl[0],1000,100,"1",3);
    setlabel(CCl[1],1000,130,"0",3);
    setlabel(CCl[2],1000,160,"0",3);
    setlabel(l6,960,100,"ZF",3);
    setlabel(l7,960,130,"SF",3);
    setlabel(l8,960,160,"OF",3);
    setlabel(n_reg[0],0,480,"rax",1);setlabel(n_reg[1],120,480,"rcx",1);
    setlabel(n_reg[2],240,480,"rdx",1);setlabel(n_reg[3],360,480,"rbx",1);
    setlabel(n_reg[4],480,480,"rsp",1);setlabel(n_reg[5],600,480,"rbp",1);
    setlabel(n_reg[6],720,480,"rsi",1);setlabel(n_reg[7],840,480,"rdi",1);
    setlabel(n_reg[8],0,580,"r8",1);setlabel(n_reg[9],120,580,"r9",1);
    setlabel(n_reg[10],240,580,"r10",1);setlabel(n_reg[11],360,580,"r11",1);
    setlabel(n_reg[12],480,580,"r12",1);setlabel(n_reg[13],600,580,"r13",1);
    setlabel(n_reg[14],720,580,"r14",1);
    setlabel(R,0,440,"Registers",4);
    for(int i = 0; i < 8; i++)
        setlabel(REGl[i],120 * i,500,"                     ",2);
    for(int i = 8; i < 15; i++)
        setlabel(REGl[i],120 * (i-8),600,"                   ",2);
    for(int i = 0; i < 7; i++){
        setlabel(Dl[i],150 + 100*i,400,"                   ",2);
    }
    for(int i = 0; i < 10; i++){
        setlabel(El[i],150 + 100*i,300,"                   ",2);
    }
    for(int i = 0; i < 7; i++){
        setlabel(Ml[i],150 + 100*i,200,"                   ",2);
    }
    for(int i = 0; i < 6; i++){
        setlabel(Wl[i],150 + 100*i,100,"                   ",2);
    }
    setlabel(D,40,400,"Decode",4);
    setlabel(n_Dl[0],150,380,"D_state",1);
    setlabel(n_Dl[1],250,380,"D_icode",1);
    setlabel(n_Dl[2],350,380,"D_ifun",1);
    setlabel(n_Dl[3],450,380,"D_rA",1);
    setlabel(n_Dl[4],550,380,"D_rB",1);
    setlabel(n_Dl[5],650,380,"D_valC",1);
    setlabel(n_Dl[6],750,380,"D_valP",1);
    setlabel(E,40,300,"Execute",4);
    setlabel(n_El[0],150,280,"E_state",1);
    setlabel(n_El[1],250,280,"E_icode",1);
    setlabel(n_El[2],350,280,"E_ifun",1);
    setlabel(n_El[3],450,280,"E_valC",1);
    setlabel(n_El[4],550,280,"E_valA",1);
    setlabel(n_El[5],650,280,"E_valB",1);
    setlabel(n_El[6],750,280,"E_dstE",1);
    setlabel(n_El[7],850,280,"E_dstM",1);
    setlabel(n_El[8],950,280,"E_srcA",1);
    setlabel(n_El[9],1050,280,"E_srcB",1);
    setlabel(M,40,200,"Memory",4);
    setlabel(n_Ml[0],150,180,"M_state",1);
    setlabel(n_Ml[1],250,180,"M_icode",1);
    setlabel(n_Ml[2],350,180,"M_cnd",1);
    setlabel(n_Ml[3],450,180,"M_valE",1);
    setlabel(n_Ml[4],550,180,"M_valA",1);
    setlabel(n_Ml[5],650,180,"M_dstE",1);
    setlabel(n_Ml[6],750,180,"M_dstM",1);
    setlabel(W,40,100,"Write",4);
    setlabel(n_Wl[0],150,80,"W_state",1);
    setlabel(n_Wl[1],250,80,"W_icode",1);
    setlabel(n_Wl[2],350,80,"W_valE",1);
    setlabel(n_Wl[3],450,80,"W_valM",1);
    setlabel(n_Wl[4],550,80,"W_dstE",1);
    setlabel(n_Wl[5],650,80,"W_dstM",1);
    slider.setOrientation(Qt::Horizontal);
    slider.setMinimum(1);
    slider.setMaximum(10);
    slider.setSingleStep(1);
    v = 1;
    connect(&step, &QPushButton::released, this, &Widget::myslot_step);
    connect(&OK, &QPushButton::released, this, &Widget::myslot_ok);
    connect(&DEC, &QPushButton::released, this, &Widget::myslot_DECHEX);
    connect(&slider,&QSlider::valueChanged, this, &Widget::changeV);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Widget::myslot_step);
}

Widget::~Widget()
{
    delete ui;
}
void Widget::clear_all(){
    for(int i = 0; i < 7; i++){
        Dl[i].clear();dl[i].clear();
    }
    for(int i = 0; i < 10; i++){
        El[i].clear();el[i].clear();
    }
    for(int i = 0; i < 7; i++){
        Ml[i].clear();ml[i].clear();
    }
    for(int i = 0; i < 6; i++){
        Wl[i].clear();wl[i].clear();
    }
}
void setcode(long long i,QLabel& a){
    switch(i){
        case(0):a.setText("halt");break;
        case(1):a.setText("nop");break;
        case(2):a.setText("rrmovq");break;
        case(3):a.setText("irmovq");break;
        case(4):a.setText("rmmovq");break;
        case(5):a.setText("mrmovq");break;
        case(6):a.setText("OPq");break;
        case(7):a.setText("jxx");break;
        case(8):a.setText("cmov**");break;
        case(9):a.setText("call");break;
        case(10):a.setText("pushq");break;
        case(11):a.setText("popq");break;
    }
}
void setstate(long long i, QLabel& a){
    if(i == 0)a.setText("Bubble");
    else if(i == 1)a.setText("AOK");
    else if(i == 2)a.setText("HLT");
    else if(i == 3)a.setText("ADR");
    else if(i == 4)a.setText("INS");
}
QString hex2str(long long a){
    QString str;
    QString c;
    int fu = 0;
    if(a < 0){a = -a;fu = 1;}
    while(a){
        switch (a%16) {
            case 0: str = "0" + str;break;
            case 1: str = "1" + str;break;
            case 2: str = "2" + str;break;
            case 3: str = "3" + str;break;
            case 4: str = "4" + str;break;
            case 5: str = "5" + str;break;
            case 6: str = "6" + str;break;
            case 7: str = "7" + str;break;
            case 8: str = "8" + str;break;
            case 9: str = "9" + str;break;
            case 10: str = "a" + str;break;
            case 11: str = "b" + str;break;
            case 12: str = "c" + str;break;
            case 13: str = "d" + str;break;
            case 14: str = "e" + str;break;
            case 15: str = "f" + str;break;
        }
        a /= 16;
    }
    if(str.isEmpty())str += '0';
    str = "0x" + str;
    if(fu)str = '-' + str;
    return str;
}
void Widget::set_label(){
    PCl.setText(QString::number(PC));
    clockl.setNum(pipeclock);
    CCl[0].setNum(CC[0]);
    CCl[1].setNum(CC[1]);
    CCl[2].setNum(CC[2]);
    setcode(D_icode,Dl[1]);setcode(E_icode,El[1]);setcode(M_icode,Ml[1]);setcode(W_icode,Wl[1]);
    setstate(D_state,Dl[0]);setstate(E_state,El[0]);setstate(M_state,Ml[0]);setstate(W_state,Wl[0]);
    if(!show_hex){
        Dl[2].setText(QString::number(D_ifun));
        Dl[3].setText(QString::number(D_rA));
        Dl[4].setText(QString::number(D_rB));
        Dl[5].setText(QString::number(D_valC));
        Dl[6].setText(QString::number(D_valP));
        El[2].setText(QString::number(E_ifun));
        El[3].setText(QString::number(E_valC));
        El[4].setText(QString::number(E_valA));
        El[5].setText(QString::number(E_valB));
        El[6].setText(QString::number(E_dstE));
        El[7].setText(QString::number(E_dstM));
        El[8].setText(QString::number(E_srcA));
        El[9].setText(QString::number(E_srcB));
        Ml[2].setText(QString::number(cnd));
        Ml[3].setText(QString::number(M_valE));
        Ml[4].setText(QString::number(M_valA));
        Ml[5].setText(QString::number(M_dstE));
        Ml[6].setText(QString::number(M_dstM));
        Wl[2].setText(QString::number(W_valE));
        Wl[3].setText(QString::number(W_valM));
        Wl[4].setText(QString::number(W_dstE));
        Wl[5].setText(QString::number(W_dstM));
    }
    else {
        Dl[2].setText(hex2str(D_ifun));
        Dl[3].setText(hex2str(D_rA));
        Dl[4].setText(hex2str(D_rB));
        Dl[5].setText(hex2str(D_valC));
        Dl[6].setText(hex2str(D_valP));
        El[2].setText(hex2str(E_ifun));
        El[3].setText(hex2str(E_valC));
        El[4].setText(hex2str(E_valA));
        El[5].setText(hex2str(E_valB));
        El[6].setText(hex2str(E_dstE));
        El[7].setText(hex2str(E_dstM));
        El[8].setText(hex2str(E_srcA));
        El[9].setText(hex2str(E_srcB));
        Ml[2].setText(hex2str(cnd));
        Ml[3].setText(hex2str(M_valE));
        Ml[4].setText(hex2str(M_valA));
        Ml[5].setText(hex2str(M_dstE));
        Ml[6].setText(hex2str(M_dstM));
        Wl[2].setText(hex2str(W_valE));
        Wl[3].setText(hex2str(W_valM));
        Wl[4].setText(hex2str(W_dstE));
        Wl[5].setText(hex2str(W_dstM));
    }
    for(int i = 0; i < 15; i++){
        if(!show_hex)REGl[i].setText(QString::number(REG[i]));
        else REGl[i].setText(hex2str(REG[i]));
    }
}

void Widget::myslot_step(){
    if(lef <= 3 && lef > 0){
        d_state = 0;
        run_clock(1);
        lef--;
    }
    else if(!lef){
        l3.setText("END");
        return;
    }
    else if(!circle())lef = 3;
    set_label();
}
void Widget::myslot_ok(){
    myslot_pause();
    go.setText("go");
    l3.setText("running..");
    int i = choice.currentIndex() + 1;
    if(i == 19)id=edit1.text().toInt();
    else id = i;
    if(id > 20 || id < 1)this->close();
    run_prepare();
    set_label();
}
void Widget::myslot_go(){
    timer->start(1000/v);
    go.setText("pause");
    disconnect(&go, &QPushButton::released, this, &Widget::myslot_go);
    connect(&go, &QPushButton::released, this, &Widget::myslot_pause);
}
void Widget::myslot_DECHEX(){
    show_hex = !show_hex;
    set_label();
    if(!show_hex)DEC.setText("HEX");
    else DEC.setText("DEC");
}
void Widget::myslot_pause(){
    timer->stop();
    go.setText("continue");
    disconnect(&go, &QPushButton::released, this, &Widget::myslot_pause);
    connect(&go, &QPushButton::released, this, &Widget::myslot_go);
}
void Widget::changeV(){
    v = slider.value();
    if(timer->isActive())timer->start(1000/v);
}
