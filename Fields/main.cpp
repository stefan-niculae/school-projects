/*
 *  Stefan Niculae Grupa 131
 *  UB FMI A1S2 POO
 *  Proiect 2: Criptografie
 */

#pragma GCC diagnostic ignored "-Wwrite-strings"  // 'a' nu ii place sa il autotransforme in char*

#include <iostream>         // pt cout, cin
#include <sstream>          // pt istringstream
#include "corp.h"
#include "testeCorp.h"
using namespace std;


void citireCorpOarecare(CorpOricare*& K) {
    string mult; bool vect, skip;
    
    cout << "Tipul de multime (Q, R, C, Zn) = "; cin >> mult;
    cout << "Vector (1/0) = "; cin >> vect;
    
    Corp<Rational> Q;   Corp<Vector<Rational>> Qn;
    Corp  <Real>   R;   Corp<Vector  <Real>>   Rn;
    Corp <Complex> C;   Corp<Vector <Complex>> Cn;
    Corp   <Zn>  Zen;   Corp<Vector   <Zn>>  Zenn;
    
    if(mult[0] == 'Q') { if(vect) K = &Qn; else K = &Q; }
    if(mult[0] == 'R') { if(vect) K = &Rn; else K = &R; }
    if(mult[0] == 'C') { if(vect) K = &Cn; else K = &C; }
    if(mult[0] == 'Z') { if(vect) K = &Zenn; else K = &Zen; }
    
    cout << "Sarim peste verificare? (1/0) "; cin >> skip;    
    K->setSkip(skip);
    
    cout << "Introdu corpul in formatul: \
            \n\t Cardinal: -1 pt inf (n-ul pt Zn) \
            \n\t Puterea : daca ai spus ca e vector \
            \n\t Elemente: pt multime finita (nu Zn) \
            \n\t Operatii: +, * SAU 2x + 3(x-y) \n";

    K->cit(cin);
    K->afis(cout);
}

void meniu()
{
    CorpOricare *K1, *K2;
    citireCorpOarecare(K1);
    
    bool rasp; char op; int x, y;
    
    //if(K1->finit())
    while(1) {
        cout << "\nTestam o operatie (pt multimi finite)? (1/0) "; cin >> rasp;
        if(!rasp) break;
        
        cout << "Ce operatie? (+ - * /) "; cin >> op;
        if(op == '-' || op == '/')
            cout << "WARNING: Daca ai sarit peste verificare, operatiile Minus si Impartire nu vor functiona bine. \
                     Scaderea presupune adunarea cu inversul operatiei Plus dar pentru ca nu am facut verificarea nici nu stim daca exista!\n";

        cout << "Indicele primului element (incepem de la zero) = "; cin >> x;
        cout << "Indicele celui de-al doilea element (incepem de la zero) = "; cin >> y;

        K1 -> opIndicii(op, x, y);
    }
    
    while(1) {
        cout << "\nTestam izomorfismul (mai citim un corp)? (1/0) "; cin >> rasp;
        if(!rasp) break;
        
        citireCorpOarecare(K2);
        K1->izomorfCu(*K2);
    }
    
    cout << "Mai testam odata? (1/0) "; cin >> rasp;
    if(rasp) meniu();
}

int main()
{
    //test1();       // op custom cu reale
    //test2();       // complex op cit din fis
    //test3();       // vect compl elem cit din fis (cam pointless - pt ca nu merg operatiile)
    //test4();       // op cusom pe Zn (cu minus in ea)
    //test5();       // izom intre R=C, R!=Z5
    //test6();       // 2x+5y intre vectori de reale
    //test7();       // x+2y pe vectori de Zn
    //test8();       // minus pe Zn
    //test9();       // operatii pe vectori de complesi
    
    //meniu();
    
    return 0;
}
