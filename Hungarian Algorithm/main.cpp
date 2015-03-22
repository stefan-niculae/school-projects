#include <iostream>
#include <sstream>
#include "GrafBipartit.h"
#include "algoritmUngar.h"
using namespace std;

void testPas0()
{
    GrafBipartit G;
    
    // http://community.topcoder.com/i/education/alg_hungarianAlgorithm/image012.jpg
    (istringstream)
    "3 9\
    1 1 1\
    1 2 4\
    1 3 5\
    2 1 5\
    2 2 7\
    2 3 6\
    3 1 5\
    3 2 8\
    3 3 8" >> G;
    
    cout << "Graful initial: \n" << G
         << "\nGraful dupa pasul zero: \n" << G.scadereCostMinim();

}

void testPas1()
{
    GrafBipartit G;
    
    // http://d2o58evtke57tz.cloudfront.net/wp-content/uploads/maximum_matching1.png
    /*(istringstream)
     "6 8\
     1 2 0\
     1 3 0\
     3 1 0\
     3 4 0\
     4 3 0\
     5 3 0\
     5 4 0\
     6 6 0" >> G;*/
    
    // http://community.topcoder.com/i/education/alg_hungarianAlgorithm/image016.jpg
    (istringstream)
    "3 5\
    1 1 0\
    2 1 0\
    2 2 0\
    2 3 0\
    3 1 0" >> G;
    
    cout << G
         << "\nExista un maxim de " << G.cuplajBipartitMaxim() << " cuplaje: \n";
    G.afisareCuplaj();

}

void testTot()
{
    GrafBipartit G;
    
    // http://community.topcoder.com/i/education/alg_hungarianAlgorithm/image012.jpg
    (istringstream)
    "3 9\
    1 1 1\
    1 2 4\
    1 3 5\
    2 1 5\
    2 2 7\
    2 3 6\
    3 1 5\
    3 2 8\
    3 3 8" >> G;
    
    cout << "Graful: \n" << G;
    
    G.algoritmUngar();
    
    cout << "\nLista cuplajelor: \n";
    G.afisareCuplaj();
    
}

void testTot2()
{
    GrafBipartit G;
    
    //https://www.youtube.com/watch?v=dQDZNHwuuOY#t=24
    (istringstream)
    "4 16\
    1 1 80\
    1 2 40\
    1 3 50\
    1 4 46\
    2 1 40\
    2 2 70\
    2 3 20\
    2 4 30\
    3 1 35\
    3 2 20\
    3 3 25\
    3 4 30\
    4 1 35\
    4 2 20\
    4 3 25\
    4 4 30" >> G;
    
    cout << "Graful: \n" << G;
    
    G.algoritmUngar();
    
    cout << "\nLista cuplajelor: \n";
    G.afisareCuplaj();
}

int main()
{
    //testPas0();
    //testPas1();
    //testTot();
    testTot2();
    
    return 0;
}

