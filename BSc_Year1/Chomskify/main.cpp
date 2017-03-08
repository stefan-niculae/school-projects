#include <iostream>
#include <sstream>
#include "Gramatica.h"
#include "Vector.h"
using namespace std;

void testLambda()
{
    // Pag 5: http://people.cs.clemson.edu/~goddard/texts/theoryOfComputation/9a.pdf
    
    Gramatica G;
    
    (istringstream)
    "S AxB\
    A &\
    B &\
    " >> G;
    
    cout << "Gramatica initiala: \n" << G << "\n"
    << "Gramatica fara lambda: \n" << G.faraLambda();
    
    
    // Pag 12: http://people.cs.clemson.edu/~goddard/texts/theoryOfComputation/9a.pdf
    
    Gramatica G2;
    (istringstream)
    "S AbA\
    A Aa\
    A &"
    >> G2;
    
    cout << "\n\n\nGramatica initiala: \n" << G2 << "\n"
    << "Gramatica fara lambda: \n" << G2.faraLambda();
    
}

void testDrUna()
{
    // Pag 10: http://people.cs.clemson.edu/~goddard/texts/theoryOfComputation/9a.pdf
    
    Gramatica G;
    
    (istringstream)
    "S aXbX\
    S abX\
    S aXb\
    S ab\
    X aY\
    X bY\
    X a\
    X b\
    Y X\
    Y c"
    >> G;
    
    cout << "Gramatica initiala: \n" << G << "\n"
    << "Gramatica fara productii de doar un terminal in dreapta: \n" << G.faraDrUnTerm();
}

void testDrLung()
{
    // Pag 13: http://people.cs.clemson.edu/~goddard/texts/theoryOfComputation/9a.pdf
    
    Gramatica G;
    
    (istringstream)
    "S AbA\
    S bA\
    S Ab\
    S b\
    A Aa\
    A a"
    >> G;
    
    cout << "Gramatica initiala: \n" << G << "\n"
    << "Gramatica fara productii cu partea dreapta mai lunga de doi: \n" << G.faraDrLungi();
    
    Gramatica G2;
    
    (istringstream)
    "S aXbX\
    S abX\
    S aXb\
    S ab\
    X aY\
    X bY\
    X a\
    X b\
    Y c"
    >> G2;
    
    cout << "\n\n\nGramatica initiala: \n" << G2 << "\n"
    << "Gramatica fara productii cu partea dreapta mai lunga de doi: \n" << G2.faraDrLungi();
}

void testAmestec()
{
    Gramatica G;
    
    (istringstream)
    "S EX\
    S FX\
    S Db\
    S ab\
    X aY\
    X bY\
    X a\
    X b\
    Y aY\
    Y bY\
    Y a\
    Y b\
    Y c\
    D aX\
    E Db\
    F ab\
    A DE\
    B b" >> G;
    
    cout << "Gramatica initiala: \n" << G << "\n"
    << "Gramatica fara amestec de terminale si neterminale in dreapta: \n" << G.faraAmestec();
    
    
    
    // Pag 14: http://people.cs.clemson.edu/~goddard/texts/theoryOfComputation/9a.pdf
    
    Gramatica G2;
    
    (istringstream)
    "S TA\
    S bA\
    S Ab\
    S b\
    A Aa\
    A a\
    T Ab"
    >> G2;
    
    cout << "\n\n\nGramatica initiala: \n" << G2 << "\n"
    << "Gramatica fara amestec de terminale si neterminale in dreapta: \n" << G2.faraAmestec();
}

void testDrS()
{
    Gramatica G;
    
    (istringstream)
    "S AB\
    S SA\
    S BS\
    A AS\
    A a\
    B b"
    >> G;
    
    cout << "Gramatica initiala: \n" << G << "\n"
    << "Gramatica fara simbolul de start in dreapta: \n" << G.faraSInDr();
}

void testTot1()
{
    // Porblema 1: https://word.office.live.com/wv/WordView.aspx?FBsrc=https%3A%2F%2Fwww.facebook.com%2Fdownload%2Ffile_preview.php%3Fid%3D1440939839481074%26time%3D1398081553%26metadata&access_token=100000361082874%3AAVLSlZwZm3SZrXCpxa9FA5g1wYLePrR_gDwWuw6oInoJSQ&title=Teste+-+Tema+3.doc
    
    Gramatica G;
    
    (istringstream)
    "S aXbX\
    X aY\
    X bY\
    X &\
    Y X\
    Y c"
    >> G;
    
    cout << "Gramatica initiala: \n" << G << "\n"
    << "Gramatica Chomskyficata: \n" << G.Chomskyficare();
}

void testTot2()
{
    // Exemplu mijloc pag1: http://www.cs.nyu.edu/courses/fall07/V22.0453-001/cnf.pdf
    
    Gramatica G;
    
    (istringstream)
    "S ASA\
    S aB\
    A B\
    A S\
    B b\
    B &"
    >> G;
    
    cout << "Gramatica initiala: \n" << G << "\n"
    << "Gramatica Chomskyficata: \n" << G.Chomskyficare();
    
}

void testTot3()
{
    Gramatica G;
    
    (istringstream)
    "S AAB\
    S B\
    S a\
    A S\
    A a\
    A &\
    B AB\
    B a\
    B b"
    >> G;
    
    
    cout << "Gramatica initiala: \n" << G << "\n"
    << "Gramatica Chomskyficata: \n" << G.Chomskyficare();
    
}

void testExplicat()
{
    Gramatica G;
    
    (istringstream)
    "S AAB\
    S B\
    S a\
    A S\
    A a\
    A &\
    B AB\
    B a\
    B b"
    >> G;

    cout << "Gramatica initiala: \n" << G << "\n\n";
    G.faraLambda();   cout << "\nDupa PASUL 1 (eliminare lambda):\n" << G;
    G.faraDrUnTerm(); cout << "\nDupa PASUL 2 (eliminare redenumiri):\n" << G;
    G.faraDrLungi();  cout << "\nDupa PASUL 3 (maxim 2 in dreapta):\n" << G;
    /*G.faraAmestec();*/  cout << "\nDupa PASUL 4 (fara terminal + neterminal):\n" << G;
    G.faraSInDr(); cout << "\nDupa PASUL 5 (fara S in dreapta):\n" << G;
    G.ordonare(); cout << "\nDupa PASUL 6 (ordonate frumos): \n" << G;
}

int main()
{
    //testLambda();
    //testDrUna();
    //testDrLung();
    //testAmestec();
    //testDrS();
    
    //testTot1();
    //testTot2();
    //testTot3();
    testExplicat();
    
    return 0;
}