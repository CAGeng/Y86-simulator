#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<queue>
#include<qglobal.h>
#include<string.h>
using namespace std;
typedef long long qlonglong;
int id,Mode;//swich prog
//control data
queue<int> jumps,adds;
long long adds_rec;
int wrong_jump,stall_left,ret_left;
int pipeclock,CC[3] = {1,0,0};
int HLT,ADR,INS = 0;
long long PC,PC_end;
// pipeline reg
long long cnd = 1;//cnd in M
long long   D_state,d_state,D_icode,d_icode,D_ifun,d_ifun,D_rA,d_rA,D_rB,d_rB,
            D_valC,d_valC,D_valP,d_valP,n_PC,

            E_state,e_state,E_icode,e_icode,E_ifun,e_ifun,
            E_valA,e_valA,E_valB,e_valB,E_valC,e_valC,
            E_dstE,e_dstE,E_dstM,e_dstM,E_srcA,e_srcA,E_srcB,e_srcB,

            M_state,m_state,M_icode,m_icode,
            M_valE,m_valE,M_valA,m_valA,
            M_dstE,m_dstE,M_dstM,m_dstM,

            W_state,w_state,W_icode,w_icode,
            W_valE,w_valE,W_valM,w_valM,
            W_dstE,w_dstE,W_dstM,w_dstM;
// reg and memory
long long REG[16];
long long  M[10000];
ifstream f;
int gx_reg[16];
char CODE[10000] ;
char line[1005];
int stall_time;
int gx_mem[10000];
int cond(int* CC, int ifun);
void set_gx();
void forwarding_stalling_control();
// initializing  functions
void init(){
    //control data
    jumps.empty(),adds.empty();
    adds_rec = 0;
    wrong_jump = 0,stall_left = 0,ret_left = 0;
    pipeclock = 0,CC[1] = CC[2] = 0;
    CC[0] = 1;
    HLT = 0,ADR = 0,INS = 0;
    PC = 0,PC_end = 0;
    // pipeline reg
    cnd = 1;//cnd in M
    D_state= d_state =D_icode =d_icode =D_ifun =d_ifun =D_rA=d_rA=D_rB=d_rB = 0,
    D_valC=d_valC=D_valP=d_valP=n_PC=0,

    E_state=e_state=E_icode=e_icode=E_ifun=e_ifun=0,
    E_valA=e_valA=E_valB=e_valB=E_valC=e_valC=
    E_dstE=e_dstE=E_dstM=e_dstM=E_srcA=e_srcA=E_srcB=e_srcB=0,

    M_state=m_state=M_icode=m_icode=
    M_valE=m_valE=M_valA=m_valA=
    M_dstE=m_dstE=M_dstM=m_dstM=0,

    W_state=w_state=W_icode=w_icode=
    W_valE=w_valE=W_valM=w_valM=
    W_dstE=w_dstE=W_dstM=w_dstM=0;
    // reg and memory
    memset(REG,0,sizeof (REG));
    memset(M,0,sizeof (M));
    memset(gx_reg,0,sizeof (gx_reg));
    memset(CODE,0,sizeof (CODE));
    stall_time = 0;
    memset(gx_mem,0,sizeof (gx_mem));
    if(id == 13){
        M[64] = 80;
        M[80] = 22;
    }
    if(id == 14){
        M[0x18] = 0x0000000d000d000d;
        M[0x20] = 0x000000c000c000c0;
        M[0x28] = 0x00000b000b000b00;
        M[0x30] = 0x0000a000a000a000;
    }
    if(id == 17 || id == 18){
        M[0x18] = 0x0000000d000d000d;
        M[0x20] = 0xffffff3fff3fff40;
        M[0x28] = 0x00000b000b000b00;
        M[0x30] = 0xffff5fff5fff6000;
    }

}
void getbyte(int n,int i){
    while(n--){
        CODE[2 * PC] = line[i];
        CODE[2 * PC + 1] = line[i + 1];
        PC++;
        i += 2;
    }
}
void read_one_code(){
    getbyte(1, 7);
    int cod = line[7] - '0';

    if(cod > 9 && cod < 0)
        cod += '0' - 'a' + 10;

    switch (cod){
        case 0:
        case 1:
        case 9:
            break;
        case 2:
        case 6:
        case 10:
        case 11:
            getbyte(1,9);break;
        case 7:
        case 8:
            getbyte(8,9);break;
        default: getbyte(9,9);break;
    }
}
void read_all_code(){
    while(!f.eof() && f.good()){
        f.getline(line , 1000, '\n');
        //cout << line << endl;
        if(line[5] != ':' || line[7] == ' ')continue;
        int a = line[3] - '0';
        if(a > 9 && a < 0)
            a += '0' - 'a' + 10;
        int b = line[4] - '0';
        if(b > 9 || b < 0)
            b += '0' - 'a' + 10;
        PC = 16 * a + b;
        read_one_code();
    }
}
int read_half_byte(int i){
    char c;
    int ret;
    c = CODE[2 * n_PC + i];
    if(i)n_PC ++;
    ret = (int)c - '0';
    if(ret < 0 || ret > 9)ret += '0' - 'a' + 10;
    return ret;
}
long long read_8byte(){
    int a,b;
    long long sum = 0,t = 1;
    for(int i = 0; i < 8; i ++){
        a = read_half_byte(0);
        b = read_half_byte(1);
        sum += t * (16 * a + b);
        t = t << 8;
    }
    return sum;
}
// pipeline body
int fetch(){
    n_PC = PC;
    if(PC >= PC_end)return 0;
    d_icode = read_half_byte(0);
    d_ifun = read_half_byte(1);

    if(d_icode == 3 || d_icode == 4 || d_icode == 5
       || d_icode == 6 || d_icode == 2 || d_icode == 10 ||d_icode == 11){
            d_rA = read_half_byte(0);
            d_rB = read_half_byte(1);
    }
    if(d_icode == 3 || d_icode == 4 || d_icode == 5
       || d_icode == 7 || d_icode == 8)
        d_valC = read_8byte();

    if(d_icode == 8){
        d_valP = n_PC;
        n_PC = d_valC;
    }

    if(d_icode == 7){
        if(d_ifun == 0){
            n_PC = d_valC;
        }
    }
    return 1;
}
void decode(){
    HLT = 0;
    e_state = D_state;
    if(!D_state)return;
    e_icode = D_icode;
    e_ifun = D_ifun;
    e_dstE = D_rB;  // ..
    if(D_icode == 5 || D_icode == 11)e_dstE = D_rA;
    if(D_icode == 2 || D_icode == 3 || D_icode == 4 || D_icode == 5
       || D_icode == 6 ){
        e_valA = REG[D_rA];
        e_valB = REG[D_rB];
    }
    else if(D_icode == 10){
        e_valA = REG[D_rA];
        e_valB = REG[4];
    }
    else if(D_icode == 11)
        e_valA = e_valB = REG[4];
    else if(D_icode == 9){
        e_valA = REG[4];
        e_valB = REG[4];
    }
    if( D_icode == 3 || D_icode == 4 || D_icode == 5
       || D_icode == 7 || D_icode == 8 )
       e_valC = D_valC;

    if(D_icode == 8) {
        e_valB = REG[4];
        e_valA = D_valP;
    }

    if(D_icode == 0){
        HLT = 1;
    }
    return;
}
void execute(){
    m_state = E_state;
    if(!E_state)return;
    if(ADR){
        e_state = d_state = 0;
        return;
    }
    if(E_icode == 8 || E_icode == 10){
        if(E_valB == 0){
            ADR = 1;
            cout << "wrong address!!" << endl;
            e_state = d_state = 0;
            return;
        }
    }// ..2

    m_icode = E_icode;
    m_dstE = E_dstE;
    m_valA = E_valA;
    if(E_icode == 2 )m_valE = E_valA;
    else if(E_icode == 3)m_valE = E_valC;
    else if(E_icode == 4)m_valE = E_valB + E_valC;
    else if(E_icode == 5)m_valE = E_valB + E_valC;
    else if(E_icode == 6){
        if(E_ifun == 0){
            m_valE = E_valA + E_valB;
        }
        else if(E_ifun == 1){
            m_valE = E_valB - E_valA;
        }
        else if(E_ifun == 2)m_valE = E_valA & E_valB;
        else if(E_ifun == 3)m_valE = E_valA ^ E_valB;
        //set CC
        int x = ((E_valA ) >> 63) & 1,
            y = ((E_valB ) >> 63) & 1,
            z = ((m_valE ) >> 63) & 1;
        CC[0] = !m_valE;
        CC[1] = z;
        if(E_ifun == 0){
            CC[2] = (x & y & (~z)) | ((~x) & (~y) & z);
        }
        else if(E_ifun == 1){
            CC[2] = (x & z & (~y)) | ((~x) & (~z) & y);
        }
    }
    else if(E_icode == 10)m_valE = E_valB - 8;
    else if(E_icode == 11)m_valE = E_valB + 8;
    else if(E_icode == 9)m_valE = E_valB + 8;
    else if(E_icode == 8){
        m_valE = E_valB - 8;
    }
    else if(E_icode == 7){
        cnd = cond(CC,E_ifun);
        if(cnd != 1)wrong_jump = 1;
        else wrong_jump = 0;
        adds_rec = adds.front();
        adds.pop();
    }
    if(E_icode == 2 && E_ifun){
        cnd = cond(CC,E_ifun);
        if(!cnd)E_state = m_state = 0;
    }
}
void memory(){
    w_state = M_state;
    if(!M_state)return;
    w_icode = M_icode;
    w_dstE = M_dstE;
    if(M_icode == 4)M[M_valE] = M_valA;
    else if(M_icode == 5) w_valM = M[M_valE];
    else if(M_icode == 10) M[M_valE] = M_valA;
    else if(M_icode == 11) w_valM = M[M_valA];
    else if(M_icode == 9) w_valM = M[M_valA];
    else if(M_icode == 8) M[M_valE] = M_valA;
    w_valE = M_valE;
    if(M_icode == 8|| M_icode == 10)
        if(M_valE >> 31){
            ADR = 1;//error
            printf("\n ADR error!!\n");
        }
}
void write(){
    if(!W_state)return;

    if(W_icode == 2 || W_icode == 3 || W_icode == 6)
        REG[W_dstE] = W_valE;
    else if(W_icode == 5)REG[W_dstE] = W_valM;
    else if(W_icode == 10)REG[4] =W_valE;
    else if(W_icode == 11){
        REG[4] = W_valE;
        REG[W_dstE] = W_valM;
    }
    else if(W_icode == 9){
        REG[4] = W_valE;
    }
    else if(W_icode == 8)
        REG[4] = W_valE;
}
void gx_pc(){
    if(W_state && W_icode == 9)PC = W_valM;
    else if(d_state && d_icode == 7){
        PC = D_valC;
        adds.push(n_PC);
    }
    else PC = n_PC;
}
// other functions
int cond(int* CC, int ifun){
    switch(ifun){
        case 0: return 1;
        case 1: return ((CC[1] ^ CC[2]) | CC[0]) & 1;
        case 2: return (CC[2] ^ CC[1]) & 1;
        case 3: return CC[0] & 1;
        case 4: return (~CC[0]) & 1;
        case 5: return (~(CC[1] ^ CC[2])) & 1;
        case 6: return (~(CC[1] ^ CC[2]) & ~CC[0]) & 1;
    }
}
void clock_rise_copy(){
    D_state = d_state, D_icode = d_icode, D_ifun = d_ifun, D_rA = d_rA,
    D_rB = d_rB, D_valC = d_valC , D_valP = d_valP;
    E_state = e_state,E_icode = e_icode, E_ifun = e_ifun,
    E_valA = e_valA, E_valB = e_valB, E_valC = e_valC,
    E_dstE = e_dstE, E_dstM = e_dstM, E_srcA = e_srcA, E_srcB = e_srcB;
    M_state = m_state, M_icode = m_icode,
    M_valE = m_valE, M_valA = m_valA,
    M_dstE = m_dstE, M_dstM = m_dstM;
    W_state = w_state, W_icode = w_icode,
    W_valE = w_valE, W_valM = w_valM,
    W_dstE = w_dstE, W_dstM = w_dstM;
}
void set_gx(){
    if(E_state != 1)return;
    if(E_icode == 3 || E_icode == 2 || E_icode == 6
       ||E_icode == 5 || E_icode == 11)
        gx_reg[E_dstE] = pipeclock + 3;
    if(E_icode == 4)gx_mem[m_valE] = pipeclock + 2;

    if(E_icode == 8 || E_icode == 10){
        gx_reg[4] = pipeclock + 3;
        gx_mem[REG[4] - 8] = pipeclock + 2;
    }

    if(E_icode == 9){
        gx_reg[4] = pipeclock + 3;
    }
}
// show functions
void show(){
    printf("\n*************clock = %d**************\n" , pipeclock);
    printf("\nD_state = %lld\n",D_state);
    printf("D      : icode = %lld  ifun = %lld  rA = %lld  rB = %lld  valC = %lld  valP = %lld\n",
           D_icode,D_ifun,D_rA,D_rB,D_valC,D_valP);
    printf("D input: icode = %lld  ifun = %lld  rA = %lld  rB = %lld  valC = %lld  valP = %lld\n",
           d_icode,d_ifun,d_rA,d_rB,d_valC,d_valP);

    printf("\nE_state = %lld\n",E_state);
    printf("E      : icode = %lld  ifun = %lld  valA = %lld  valB = %lld  valC = %lld  dstE = %lld  dstM = %lld  srcA = %lld  srcB = %lld\n",
           E_icode,E_ifun,E_valA,E_valB,E_valC,E_dstE,E_dstM,E_srcA, E_srcB);
    printf("E input: icode = %lld  ifun = %lld  valA = %lld  valB = %lld  valC = %lld  dstE = %lld  dstM = %lld  srcA = %lld  srcB = %lld\n",
           e_icode,e_ifun,e_valA,e_valB,e_valC,e_dstE,e_dstM,e_srcA, e_srcB);

    printf("\nM_state = %lld\n",M_state);
    printf("M      : icode = %lld  valE = %lld  valA = %lld  dstE = %lld  dstM = %lld\n",M_icode,M_valE,M_valA,M_dstE,M_dstM);
    printf("M      : icode = %lld  valE = %lld  valA = %lld  dstE = %lld  dstM = %lld\n",m_icode,m_valE,m_valA,m_dstE,m_dstM);

    printf("\nW_state = %lld\n",W_state);
    printf("W      : icode = %lld  valE = %lld  valM = %lld  dstE = %lld  dstM = %lld\n",W_icode,W_valE,W_valM,W_dstE,W_dstM);
    printf("W      : icode = %lld  valE = %lld  valM = %lld  dstE = %lld  dstM = %lld\n",w_icode,w_valE,w_valM,w_dstE,w_dstM);
}
void show_reg(){
    printf("rax\trcx\trdx\trbx\trsp\trbp\trsi\trdi\t\n");
    for(int i = 0 ; i < 8; i++)
        printf("%llx\t",(unsigned long long)REG[i]);
    printf("\n");
    printf("r8\tr9\tr10\tr11\tr12\tr13\tr14\t\n");
    for(int i = 8 ; i < 15; i++)
        printf("%llx\t",(unsigned long long)REG[i]);
    printf("\n");
}
void show_mem(){
    printf("memory[0x018] = %lld\n",M[0x18]);
    printf("memory[48] = %llx\n",M[48]);
    printf("memory[128] = %lld\n",M[128]);
}
void show_pipe(){
    printf("\n*************clock = %d**************\n" , pipeclock);
    printf("W:  ");
    if(!W_state)printf("bubble\n");
    else printf("%lld\n",W_icode);
    printf("M:  ");
    if(!M_state)printf("bubble\n");
    else printf("%lld\n",M_icode);
    printf("E:  ");
    if(!E_state)printf("bubble\n");
    else printf("%lld\n",E_icode);
    printf("D:  ");
    if(!D_state)printf("bubble\n");
    else printf("%lld\n",D_icode);
}
//run clock
void run_clock(int n){
    while(n--){
        clock_rise_copy();
        if(Mode)show();
        show_pipe();
        decode();
        memory();
        write();
        execute();//delay execute to forbid gx CC when error occuring
        set_gx();
        forwarding_stalling_control();//to get latest result for decode
        //set reg_gx_time
        gx_pc();
        show_reg();
        show_mem();
        pipeclock++;
    }
}
//control
void stall_1_clock(){
    cout << "stall 1---------------\n";
    e_state = 0;
    clock_rise_copy();
    show_pipe();
    if(Mode)show();
    execute();
    memory();
    write();
    decode();
    forwarding_stalling_control();
    set_gx();
    pipeclock++;
}
void stalling(int n){
    if(n == 1){
        printf("[[[[[stall 1 clock]]]]]\n");
        stall_1_clock();
        printf("continue!\n");
    }
    else if(n == 2){
        printf("[[[[[stall 2 clock]]]]]\n");
        stall_1_clock();
        stall_1_clock();printf("continue!\n");
    }
    else if(n == 3){
        printf("[[[[[stall 3 clock]]]]]\n");
        stall_1_clock();
        stall_1_clock();
        stall_1_clock();printf("continue!\n");
    }
}
void forwarding_stalling_control(){
    int t;
    stall_time = 0;
    if(D_icode == 2 ){
        //forwarding valA
        t = gx_reg[D_rA] - pipeclock;
        if(t <= 0);
        else if(t == 1){
            if(W_state && W_icode == 5)e_valA = W_valM;
            else if(W_state)e_valA = W_valE;
        }
        else if(t == 2){
            if(M_state && M_icode == 5)e_valA = w_valM;
            else if(M_state)e_valA = M_valE;
        }
        else if(t == 3){
            if(E_state && E_icode == 5){
                stall_time = 1;//have to stall
                return;
            }
            else if(E_state)e_valA = m_valE;
        }
    }
    if( D_icode == 6 || D_icode == 4){
        //forwarding valA
        t = gx_reg[D_rA] - pipeclock;
        if(t <= 0);
        else if(t == 1){
            if(W_state && W_icode == 5)e_valA = W_valM;
            else if(W_state)e_valA = W_valE;
        }
        else if(t == 2){
            if(M_state && M_icode == 5)e_valA = w_valM;
            else if(M_state)e_valA = M_valE;
        }
        else if(t == 3){
            if(E_state && E_icode == 5){
                stall_time = 1;//have to stall
                return;
            }
            else if(E_state)e_valA = m_valE;
        }
        //forwarding valB
        t = gx_reg[D_rB] - pipeclock;
        if(t <= 0);
        else if(t == 1){
            if(W_state && W_icode == 5)e_valB = W_valM;
            else if(W_state)e_valB = W_valE;
        }
        else if(t == 2){
            if(M_state && M_icode == 5)e_valB = w_valM;
            else if(M_state)e_valB = M_valE;
        }
        else if(t == 3){
            if(E_state && E_icode == 5){
                stall_time = 1;//have to stall
                return;
            }
            else if(E_state)e_valB = m_valE;
        }
    }
    if(D_icode == 5){
        t = gx_reg[D_rB] - pipeclock;
        if(t <= 0);
        else if(t == 1){
            if(W_state && W_icode == 5)e_valB = W_valM;
            else if(W_state)e_valB = W_valE;
        }
        else if(t == 2){
            if(M_state && M_icode == 5)e_valB = w_valM;
            else if(M_state)e_valB = M_valE;
        }
        else if(t == 3){
            if(E_state && E_icode == 5){
                stall_time = 1;//have to stall
                return;
            }
            else if(E_state)e_valB = m_valE;
        }
    }
    if(D_icode == 9 || D_icode == 11){
        t = gx_reg[4] - pipeclock;
        if(t <= 0);
        else if(t == 1){
            if(W_state && W_icode == 5)e_valB = e_valA = W_valM;
            else if(W_state)e_valB = e_valA = W_valE;
        }
        else if(t == 2){
            if(M_state && M_icode == 5)e_valB = e_valA = w_valM;
            else if(M_state)e_valB = e_valA = M_valE;
        }
        else if(t == 3){
            if(E_state && E_icode == 5){
                stall_time = 1;//have to stall
                return;
            }
            else if(E_state)e_valB = e_valA = m_valE;
        }
    }
    if(D_icode == 10|| D_icode == 8){
        //forwarding valA
        t = gx_reg[D_rA] - pipeclock;
        if(t <= 0);
        else if(t == 1){
            if(W_state && W_icode == 5)e_valA = W_valM;
            else if(W_state)e_valA = W_valE;
        }
        else if(t == 2){
            if(M_state && M_icode == 5)e_valA = w_valM;
            else if(M_state)e_valA = M_valE;
        }
        else if(t == 3){
            if(E_state && E_icode == 5){
                stall_time = 1;//have to stall
                return;
            }
            else if(E_state)e_valA = m_valE;
        }
        //forwarding (rsp)
        t = gx_reg[4] - pipeclock;
        if(t <= 0);
        else if(t == 1){
            if(W_state && W_icode == 5)e_valB = W_valM;
            else if(W_state)e_valB = W_valE;
        }
        else if(t == 2){
            if(M_state && M_icode == 5)e_valB = w_valM;
            else if(M_state)e_valB = M_valE;
        }
        else if(t == 3){
            if(E_state && E_icode == 5){
                stall_time = 1;//have to stall
                return;
            }
            else if(E_state)e_valB = m_valE;
        }
    }
}
int circle(){
    if(stall_left > 0){
        stall_left--;
        stall_1_clock();
    }
    else if(ret_left > 0){
        ret_left--;
        d_state = 0;
        run_clock(1);
    }
    else run_clock(1);
    if(stall_time > 0){
        stall_left = stall_time;
        return 1;
        //stalling(stall_time);
    }

    if(e_state && e_icode == 9){
        ret_left = 3;
        cout << "ret!\n";
        //run_clock(3);
    }
    if(!fetch() && adds.empty() && !ret_left && !stall_left)return 0;//..
    if((HLT || ADR)&& adds.empty())return 0;
    d_state = 1;
    if(wrong_jump){
        e_state = d_state = 0;
        ret_left = 0;
        n_PC = adds_rec;
        wrong_jump = 0;
    }
    return 1;
}
int mm(){
    cin >> id >> Mode;
    switch(id){
        case 1: f.open("prog1.yo");break;
        case 2: f.open("prog2.yo");break;
        case 3: f.open("prog3.yo");break;
        case 4: f.open("prog4.yo");break;
        case 5: f.open("prog5.yo");break;
        case 6: f.open("prog6.yo");break;
        case 7: f.open("prog7.yo");break;
        case 8: f.open("prog8.yo");break;
        case 9: f.open("prog9.yo");break;
        case 10: f.open("prog10.yo");break;
        case 11: f.open("pushquestion.yo");break;
        case 12: f.open("pushtest.yo");break;
        case 13: f.open("ret-hazard.yo");break;
        case 14: f.open("asum.yo");break;
        case 15: f.open("asumi.yo");break;
        case 16: f.open("asumr.yo");break;
        case 17:f.open("cjr.yo");break;
        case 18:f.open("j-cc.yo");break;
        case 19: f.open("abs-asum-cmov.yo");break;
        case 20: f.open("abs-asum-jmp.yo");break;
    }
    printf("Begin to test prog%d......\n",id);
    init();
    read_all_code();
    PC_end = PC;
    PC = 0;
    while(circle());
    d_state = 0;
    run_clock(3);
    printf("\n****************end******************\n");
    show_reg();
    show_mem();
    if(id==10)printf("CC = %d%d%d,right\n",CC[0],CC[1],CC[2]);

    return 0;
}
