#ifndef algoritmUngar_GrafBipartit_h
#define algoritmUngar_GrafBipartit_h
//#include "algoritmUngar.h"

#define NMAX 100

class GrafBipartit
{
    int N;              // Numarul de noduri din stanga si din dreapta (pt alg ungar trebe sa fie egale!)
    int m;              // Numarul de muchii
    int a[NMAX][NMAX];  // Matricea de adiacenta
    
public:
    
    GrafBipartit (int n = 0, int much = 0) : N(n), m(much)
    {
        memset (a, -1, sizeof(a));
        memset (cuplaj, -1, sizeof (cuplaj));
    }
    
    void adaugaMuchie (int, int, int);
    void stergeMuchie (int, int);
    
    friend std::istream& operator>> (std::istream&, GrafBipartit&);
    friend std::ostream& operator<< (std::ostream&, GrafBipartit);
    
    GrafBipartit scadereCostMinim ();
    
    int cuplaj[NMAX];
    bool cuplajBipartit (int, bool[]);
    int cuplajBipartitMaxim ();
    
    bool acoperire[NMAX];
    void acoperireVarfuriMinima ();
    
    void ajustarePonderi ();
    
    void algoritmUngar();
    void afisareCuplaj();
    
};

void GrafBipartit::adaugaMuchie (int x, int y, int z)
{
    // Notam in matricea de adiacenta de la nodul x la nodul y cu ponderea z
    a[x][y] = z;
    
    // Incrementam numarul de muchii
    // m++;
}

void GrafBipartit::stergeMuchie (int x, int y)
{
    // Reinitializam muchia (x,y)
    a[x][y] = a[y][x] = -1;
    
    // Decrementam numarul de muchii
    m--;
}

std::istream& operator>> (std::istream& is, GrafBipartit& G)
{
    // Citim numarul de noduri si de muchii
    is >> G.N >> G.m;
    
    int x, y, z;
    for (int k = 1; k <= G.m; k++) {
        
        // Citim cele doua noduri ale muchiei si ponderea ei
        is >> x >> y >> z;
        
        // Adaugam muchia citita
        G.adaugaMuchie (x, y, z);
        //G.adaugaMuchie (y, x, z);
        
    }
    
    return is;
}

std::ostream& operator<< (std::ostream& os, GrafBipartit G)
{
    //os << G.N << ' ' << G.m << "\n";
    
    // Parcurgem matricea de adiacenta
    for (int i = 0; i <= G.N; i++)
        for (int j = 0; j <= G.N; j++)
            
            // Unde gasim o muchie, afisam cele doua noduri si ponderea acesteia
            if (G.a[i][j] != -1)
                os << '(' << i << ',' << j << ')' << " = " << G.a[i][j] << "\n";
    
    return os;
}

#endif
