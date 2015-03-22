/**
 *  Niculae Stefan, grupa 231
 *  2 dec 2014
 *
 *  Tema seminar Calculabilitate & Complexitate
 *  Exercitiile 17, 18 + bonus
 */

#include <iostream>
using namespace std;

#define minusUnu(X) (X == 0 ? X : X - 1)
typedef unsigned long long natural;

// O(X)
void atrib (natural& Y, natural X) // Y <- X
{
    Y = 0;
    natural Z = 0;
    
    
    if (X != 0) goto L1;
    goto E;
    
L1: X = minusUnu(X);
    Y = Y + 1;
    Z = Z + 1;
    if (X != 0) goto L1;
    
L2: Z = minusUnu(Z);
    X = X + 1;
    if (Z != 0) goto L2;
    goto E;
    
    
E: return;
}

// O(X1+X2)
natural plus (natural X1, natural X2)
{
    natural Y = 0;
    natural Z1 = 0, Z2 = 0;
    
    
    if (X1 != 0) goto L1;
    if (X2 != 0) goto L2;
    goto E;
    
L2: Y = Y + 1;
    X2 = minusUnu(X2);
    Z2 = Z2 + 1;
    if (X2 != 0) goto L2;
    
L3: X2 = X2 + 1;
    Z2 = minusUnu(Z2);
    if (Z2 != 0) goto L3;
    goto E;
    
L1: Y = Y + 1;
    X1 = minusUnu(X1);
    Z1 = Z1 + 1;
    if (X1 != 0) goto L1;
    
L4: X1 = X1 + 1;
    Z1 = minusUnu(Z1);
    if (Z1 != 0) goto L4;

    if (X2 != 0) goto L2;
    goto E;
    
    
E:  return Y;
}

natural minusPunct (natural X1, natural X2)
{
    natural Y = 0;
    natural Z = 0;
    
    atrib (Y, X1);
    atrib (Z, X2);
    
L1: Y = minusUnu(Y);
    X2 = minusUnu(X2);
    if (X2 != 0) goto L1;
    
L2: X2 = X2 + 1;                // refacem X2
    Z = minusUnu(Z);
    if (Z != 0) goto L2;
    
E:  return Y;
}

// O (X1 X2)
natural ori (natural X1, natural X2)
{
    natural Y = 0;
    natural Z1 = 0, Z2 = 0;
    
    
    if (X1 != 0) goto L1;
    goto E;
    
L1: if (X2 != 0) goto L2;
    goto E;
    
L2: X1 = minusUnu(X1);
    Z1 = Z1 + 1;
    
    L3: Y = Y + 1;
        Z2 = Z2 + 1;
        X2 = minusUnu(X2);
        if (X2 != 0) goto L3;
    
    L4: X2 = X2 + 1;
        Z2 = minusUnu(Z2);
        if (Z2 != 0) goto L4;
    
    if (X1 != 0) goto L2;
    
L5: X1 = X1 + 1;
    Z1 = minusUnu(Z1);
    if (Z1 != 0) goto L5;
    goto E;
    
    
E:  return Y;
}

// O(X2 * X1^X2)
natural putere (natural X1, natural X2)
{
    natural Y = 0;
    natural Z1 = 0, Z2 = 0;
    
    
L0: if (X1 != 0) goto L1;
    if (X2 != 0) goto E;
    goto L0;                    // 0^0
    
L1: if (X2 != 0) goto L2;
    Y = Y + 1;
    goto E;
    
L2: Y = Y + 1;
    atrib (Z1, X2);
    
L3: Z1 = minusUnu(Z1);
    Z2 = ori (Y, X1);
    atrib (Y, Z2);
    if (Z1 != 0) goto L3;
    goto E;
    
    
E:  return Y;
}

// O(min{X1, X2})
natural suntEgale (natural X1, natural X2)
{
    natural Y = 0;
    natural Z = 0, Z1 = 0;
    
    
L0: if (X1 != 0) goto L1;
    if (X2 != 0) goto E;
    
    Y = Y + 1;
    goto L3;
    
L1: if (X2 != 0) goto L2;
    goto E;
    
L2: X1 = minusUnu(X1);
    X2 = minusUnu(X2);
    Z1 = Z1 + 1;
    goto L0;
    
L3: if (Z1 != 0) goto L4;
    goto E;
    
L4: X1 = X1 + 1;
    X2 = X2 + 1;
    Z = minusUnu(Z);
    if (Z != 0) goto L4;
    
    
E:  return Y;
}

natural maiMicSauEgal (natural X1, natural X2)
{
    natural Y = 0;
    natural Z = 0;
    
    
L0: if (X1 != 0) goto L1;
    Y = Y + 1;
    goto L3;
    
L1: if (X2 != 0) goto L2;
    goto L3;
    
L2: X1 = minusUnu(X1);
    X2 = minusUnu(X2);
    Z = Z + 1;
    goto L0;
    
L3: if (Z != 0) goto L4;
    goto E;
    
L4: X1 = X1 + 1;
    X2 = X2 + 1;
    Z = minusUnu(Z);
    goto L3;
    
    
E:  return Y;
}

// O(min)
natural maiMic (natural X1, natural X2)
{
    natural Y = 0;
    natural Z = 0;
    
    
L0: if (X2 != 0) goto L1;
    goto L3;
    
L1: if (X1 != 0) goto L2;
    Y = Y + 1;
    goto L3;
    
L2: X1 = minusUnu(X1);
    X2 = minusUnu(X2);
    Z = Z + 1;
    goto L0;
    
L3: if (Z != 0) goto L4;
    goto E;
    
L4: X1 = X1 + 1;
    X2 = X2 + 1;
    Z = minusUnu(Z);
    goto L3;
    
    
E:  return Y;
}

// O(X*X!)
natural factorial (natural X)
{
    natural Y = 0;
    natural Z = 0;
    
    atrib(Z, X);            // Ii facem o copie lui X, pt a nu-l strica
    Y = Y + 1;              // Pt a putea inmulti, initializam Y cu 1
    
    if (X != 0) goto L1;
    goto E;                 // Daca X = 0, returnam 1
    
L1: Y = ori (Y, Z);
    Z = minusUnu(Z);
    if (Z != 0) goto L1;
    goto E;
    
    
E:  return Y;
}

// O(max^2 / min)
void impartire (natural& Y1, natural& Y2, natural X1, natural X2)
{
    /**
     *  Y1 = X1 div X2
     *  Y2 = X1 % X2
     */
    
    Y1 = Y2 = 0;
    natural Z1 = 0, Z2 = 0, Z3 = 0;
    
L0: if (X2 != 0) goto L1;
    goto L0;                    // X1 / 0 nu se poate, cicleaza
    
L1: if (X1 != 0) goto L2;
    goto E;

L2: atrib (Z1, X1);
    
L4: Z2 = maiMic (Z1, X2);
    if (Z2 !=0) goto L3;
    Z3 = minusPunct (Z1, X2);
    atrib (Z1, Z3);
    Y1 = Y1 + 1;
    goto L4;
    
L3: atrib (Y2, Z1);
    goto E;
    
E:  return;
}

natural cat (natural X1, natural X2)
{
    natural Y1 = 0, Y2 = 0;
    impartire (Y1, Y2, X1, X2);
    return Y1;
}

natural rest (natural X1, natural X2)
{
    natural Y1 = 0, Y2 = 0;
    impartire (Y1, Y2, X1, X2);
    return Y2;
}

// O(X1^2 / X2)
natural divizibilCu (natural X1, natural X2)
{
    natural Y = 0;
    natural Z = 0;
    
    
    if (X2 != 0) goto L1;
    goto E;
    
L1: if (X1 != 0) goto L2;
    Y = Y + 1;
    goto E;
    
L2: Z = rest (X1, X2);
    if (Z != 0) goto E;
    Y = Y + 1;
    goto E;
    
    
E:  return Y;
}

// O(X^3)
natural estePrim (natural X)
{
    natural Y = 0;
    natural Z1 = 0, Z2 = 0, Z3 = 0, Z4 = 0;
    
    
    if (X != 0) goto L1;
    goto E;                 // 0 nu e prim
    
L1: X = minusUnu(X);
    if (X != 0) goto L2;
    X = X + 1;              // ca sa nu-l stricam pe X
    goto E;                 // 1 nu e prim
    
L2: X = X + 1;              // refacem X
    Z1 = Z1 + 1;
    Z1 = Z1 + 1;            // Z1 = 2
    Z2 = cat (X, Z1);       // vom testa pana la X / 2
    
L4: Z3 = maiMicSauEgal (Z1, Z2);
    if (Z3 != 0) goto L3;
    Y = Y + 1;
    goto E;
    
L3: Z4 = divizibilCu (X, Z1);
    if (Z4 != 0) goto E;
    Z1 = Z1 + 1;
    goto L4;
    
    
E:  return Y;
}

/**
 *  Ex 17
 *
 *  Pornim un contor de la zero (Y).
 *  Pornim o variabila de la unu (Z1) pana la X - 1.
 *  La fiecare incrementare, verificam daca am dat peste un numar prim
 *  daca da, crestem contorul (L2).
 *
 *  CT = (X-2) * (O(X) + O(X^3))
 *  Pentru ca pt fiecare numar de la 1 la X-1 facem doua operatii costisitoare:
 *      - verificam daca este mai mic decat X
 *      - testam primalitatea
 *  => O(X^4)
 */
natural primeMaiMiciCa (natural X)
{
    natural Y = 0;
    natural Z1 = 0, Z2 = 0;
    
L0: Z1 = Z1 + 1;
    Z2 = maiMic (Z1, X);        // O(min{Z1, X}) => O(X)
    if (Z2 != 0) goto L1;
    goto E;
    
L1: Z2 = estePrim (Z1);         // O(Z1^3) => O(X^3)
    if (Z2 != 0) goto L2;
    goto L0;
    
L2: Y = Y + 1;
    goto L0;
    
    
E:  return Y;
}

/**
 *  Ex 18
 *
 *  Pornim un contor de la zero (Z2) pana la X
 *  Intram intr-o bucla in care la fiecare pas:
 *      - incrementam valoarea lui Y
 *      - testam daca Y este prim, daca da crestem contorul
 *  Cand contorul a ajuns la X, inseamna ca in Y vom gasi al X-lea nr prim
 *
 *  CT = nedet??
 *  Am sti sa o calculam daca am sti cat de mult tine bucla, adica pana unde
 *  sa mearga Y-ul. Dar asta ar insemna sa stim distanta dintre oricare doua
 *  numere prime, ceea ce nu stim. Cu toate ca daca ne luam dupa o aproximare
 *  pentru numere mici, distributia pare sa fie liniara.
 *  ( https://www.youtube.com/watch?v=Ih6fGV4QIVo )
 *  Deci complexitatea ar putea fi X * (X^3 + X) => O(X^4)
 *  (ne costa X^3 verificarea primalitatii lui Y si X verificarea contor < X)
 */
natural alKleaNumarPrim (natural X)
{
    natural Y = 0;
    natural Z1 = 0, Z2 = 0;
    
L0: if (X != 0) goto L1;        // nu exista al zero-lea numar prim
    goto L0;
    
L1: Y = Y + 1;
    Z1 = estePrim (Y);          // O(X^3)
    if (Z1 != 0) goto L2;
    goto L1;
    
L2: Z2 = Z2 + 1;
    Z1 = maiMic (Z2, X);        // O(min{Z2, X}) => O(X)
    if (Z1 != 0) goto L1;
    goto E;
    
E:  return Y;
}

/**
 *  Exercitiu propus
 *
 *  Numarul lui Godel, sau [ ]
 *  [ ] : N^n -> N cu n din N*
 *  [x_1, x_2, ... , x_n] = (p_1)^(x_1) * ... * (p_n) ^ (x_n)
 *  Adica inmultim al k-lea numar prim ridicat la puterea argumentului
 *  de pe pozitia k.
 *
 *
 *  Folosim doua variabile auxiliare: in una retinem la al catelea nr prim suntem (Z1)
 *  in cea de-a doua (Z2), crestem valoarea de la 0 la P_n (al n-lea nr prim, unde n = nr argumente)
 *
 *  Pornim in bucla, iterand Z2 la fiecare pas si verificand daca este prim.
 *  Daca da, incrementam contorul, Z1, si totodata, inmultim rezultatul (Y) cu numarul prim gasit
 *  ridicat la puterea argumentului corespunzator.
 *
 *
 *  CT = O(P_n) * (O(P_n^3) + O(min{Z2, n} + O(P_n^max{Xi}) * O(Xi * (P_i^Xi))
 *  => O(max{Xi} * P_n^max{Xi}) ??
 *  La fiecare iteratie a buclei:
 *      - verificam daca Z2 este prim O(X^3)
 *      - comparam Z1 cu n (in implementarea noastra cu 1, 2, 3)
 *      - in caz de primalitate, inmultim (O(X1 * X2)) pe Y cu Z2 la puterea (X1 * X1^X2) Xi
 *  Bucla merge de la 0 pana la P_n.
 *
 *  !Observatie: folosim maxim 3 argumente pt ca numerele lui Godel cresc foarte repede, iesind
 *               din limitele tipurilor de date primitive ale C++ (max uLL = 2^64)
 */
natural Godel (natural X1, natural X2 = 0, natural X3 = 0)
{
    natural Y = 0;
    natural Z1 = 0, Z2 = 0, Z3 = 0;
    
    
    Y = Y + 1;
    
L1: Z2 = Z2 + 1;
    Z3 = estePrim (Z2);
    if (Z3 != 0) goto L2;
    goto L1;
    
L2: Z1 = Z1 + 1;

    Z3 = suntEgale(Z1, 1);
    if (Z3 != 0) goto L3;
    
    Z3 = suntEgale(Z1, 2);
    if (Z3 != 0) goto L4;
    
    Z3 = suntEgale(Z1, 3);
    if (Z3 != 0) goto L5;
    
    Z3 = suntEgale(Z1, 4);
    if (Z3 != 0) goto E;
    
    goto L1;
    
L3: Z3 = putere (Z2, X1);
    Y = ori (Y, Z3);
    goto L1;
    
L4: Z3 = putere (Z2, X2);
    Y = ori (Y, Z3);
    goto L1;
    
L5: Z3 = putere (Z2, X3);
    Y = ori (Y, Z3);
    goto L1;

    
E:  return Y;
}

void testEx17 ()
{
    for (int i = 0; i <= 10; i++)
        cout << "prime mai mici ca " << i << " = " << primeMaiMiciCa(i) << endl;
}

void testEx18 ()
{
    for (int i = 1; i <= 10; i++)
        cout << "al " << i << "-lea numar prim = " << alKleaNumarPrim(i) << endl;
}

void testGodel ()
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++)
                printf ("[%d, %d, %d] = %llu\n", k, j, i, Godel (k, j, i));
}


int main()
{
    
    testEx17();
    //testEx18();
    //testGodel();
    
    return 0;
}
