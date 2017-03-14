/*
 * Stefan Niculae
 * 23 mai 2014
 * Transformari elementare pe matrice
 */

#include <iostream>
#include <sstream>
#include "MeniuOperatii.h"
using namespace std;

int main()
{
    /* 
     *  Prima linie: n
     *  urmatoarele linii: cele n * n elemente
     */
    
    // Matricea de la seminar 7 ex 1
    /*meniu("4\
          3 -1 0 -1\
          2 0 0 -1\
          1 -1 1 0\
          2 -1 0 0");*/
    
    // Matricea de la ex 221b
    /*meniu("3\
          -3 -1 3\
          22 9 -27\
          5 2 -6");*/
    // 221a
    /*meniu("3\
          4 5 -2\
          -2 -2 1\
          -1 -1 1");*/
    // 221c
    /*meniu("3\
          0 4 2\
          -1 -4 -1\
          0 0 -2");.*/
    // 221d
    /*meniu("4\
          2 -1 1 0\
          0 1 1 0\
          0 -1 2 1\
          1 -1 1 1");*/
    /*meniu("4\
          1 2 3 4\
          0 1 2 3\
          0 0 1 2\
          0 0 0 1");*/
    
    // Matr ex 235
    /*meniu("4\
          3 -1 0 -1\
          2 0 0 -1\
          1 -1 1 0\
          2 -1 0 0");8*/
     
     // Matr 234 (?)
     /*meniu("4\
           0 0 -5 3\
           0 0 -3 1\
           -5 3 0 0\
           -3 1 0 0");*/
    
    // Matr sub examen 2
    /*meniu("3\
          1 2 -1\
          0 2 0\
          1 -2 3");*/
    
    // Matr sub examen 1
    /*meniu("3\
          -2 8 6\
          -4 10 6\
          4 -8 -4");*/
    
    // 238
    /*meniu("5\
          0 0 0 0 1\
          1 0 0 0 -1\
          0 1 0 0 -2\
          0 0 1 0 2\
          0 0 0 1 1");*/
    // examen 3
    /*meniu("4\
          0 0 -5 3\
          0 0 -3 1\
          -5 3 0 0\
          -3 1 0 0");*/
    
    /*meniu ("3\
           1 x x\
           x 1 x\
           x x 1");*/
    
    meniu ("4\
           5 4 2 1\
           0 1 -1 -1\
           -1 -1 3 0\
           1 1 -1 2");
          
    /*
     * Operatii:
     *
     * L1 p L2            (permutare)
     * L1 +/- L2          (nu scrii tu +/-, pui ori + ori -)
     * L1 +/- 3 L2
     * L1 +/- (-x+1) L2
     * L1 * 4
     * L1 * (x^2 + x + 1)
     * L1 / (-2)          (impartire doar la constante!)
     * L1 g               (guma)
     *
     * 0 (zero adica) ca sa iesi
     *
     * (nu se face verificare daca operatia ceruta e legala)
     * (nu conteaza case-ul si nici space-uri intre operanzi/operatori)
     */
    
    return 0;
}
