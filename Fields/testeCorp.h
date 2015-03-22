#ifndef criptografie_testeCorp_h
#define criptografie_testeCorp_h

#include "corp.h"
using namespace std;

/*
 *  nu prea exista exemple de corpuri finite mici pentru teste
 *  http://en.wikipedia.org/wiki/Finite_field#Some_small_finite_fields
 */


void test1() {
    Corp<Real> R(-1,-1,"x + 2 * y"); R.SKIP_VERIF = 1;
    
    cout << R;
    cout << R.Plus << " intre 1.5 si 2 = " << R.Plus(1.5, 2);
}
void test2() {
    Corp<Complex> C1; C1.SKIP_VERIF = 1;
    
    (istringstream)
    "2 \
    1 1 \
    2 2 \
    2(x+y) \
    *"
    >> C1;
    
    cout << C1;
    C1.opIndicii('+',0,1);
}
void test3() {
    Corp<Vector<Complex>> C; C.SKIP_VERIF = 1;
    
    (istringstream)
    "2 3 \
    1 -1 2 2.5 3 3\
    4 4 5 5 6 6\
    +\
    *"
    >> C;
    
    cout << C;
}
void test4() {
    Corp<Zn> Ztest;
    
    (istringstream)
    "5 \
    2x-y\
    *"
    >> Ztest;
    
    cout << Ztest;
    Ztest.opIndicii('+',1,3);
    
}
void test5() {
    Corp<Real> R;
    Corp<Complex> C;
    Corp<Zn> Z3(3);
    
    cout << R << C << (izomorfe(R,C)  ? "" : "NE" ) << "IZOMORFE!\n\n";
    cout << R << Z3 <<(izomorfe(R,Z3) ? "" : "NE" ) << "IZOMORFE!\n\n";
}
void test6() {
    Corp<Vector<Real>> R; R.setSkip(1);
    
    (istringstream)
    "2\
    3\
    1 2 3\
    4 5 6\
    2x+5y\
    *"
    >> R;
    
    cout << R;
    R.opIndicii('+', 0, 1);
}

void test7()
{
    Corp<Vector<Zn>> Z3(3,2,"x+2y");
    
    cout << Z3;
    Z3.opIndicii('+', 6, 7);
}

void test8()
{
    Corp<Zn> Z5(5);
    
    cout << Z5;
    Z5.opIndicii('-', 1, 3);
}

void test9()
{
    Corp<Vector<Complex>> C; C.setSkip(1);
    (istringstream)
    "2 2\
    1 1 10 0\
    2 2 5 5\
    x+2y\
    *"
    >> C;
    
    cout << C;
    C.opIndicii('+', 0, 1);
    C.opIndicii('*', 0, 1);
}

#endif
