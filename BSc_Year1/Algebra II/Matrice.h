#ifndef operatiiMatrice_Matrice_h
#define operatiiMatrice_Matrice_h
#include "Polinom.h"

template <class Tip>
class Matrice
{
    static const int NMAX = 15;
    
    int n;
    Tip elem[NMAX][NMAX];
    
public:
    
    // Constructor nu facem, il lasam pe cel implicit
    // (sa initializeze automat si polinoame si intregi
    
    // Supraincarcare operator citire si afisare
    template <class Tip2> friend std::istream& operator>> (std::istream&, Matrice<Tip2>&);
    template <class Tip2> friend std::ostream& operator<< (std::ostream&, Matrice<Tip2>);
    
    inline int get_n()
    {
        return n;
    }
    inline void set_n(int x)
    {
        n = x;
    }
    
    // Nu putem avea A[i][j] asa ca scriem A(i,j)
    inline Tip& operator() (int i, int j)
    {
        return elem[i][j];
    }
    
    void PermCol(int x, int y)
    {
        // Swap chior pe linie sau coloana
        for (int i = 1; i <= n; i++)
            std::swap(elem[i][x], elem[i][y]);
    }
    void PermLin(int x, int y)
    {
        for (int i = 1; i <= n; i++)
            std::swap(elem[x][i], elem[y][i]);
    }
    
    void adunLin(int x, int y, Polinom P)
    {
        // La linia/coloana x adunam y, inmultind fiecare element cu polinomul P
        for (int i = 1; i <= n; i++)
            elem[x][i] += P * elem[y][i];
    }
    void adunCol(int x, int y, Polinom P)
    {
        for (int i = 1; i <= n; i++)
            elem[i][x] += P * elem[i][y];
    }
    
    void scadLin(int x, int y, Polinom P)
    {
        // Exact ca la adunare doar ca - in loc de +
        for (int i = 1; i <= n; i++)
            elem[x][i] -= P * elem[y][i];
    }
    void scadCol(int x, int y, Polinom P)
    {
        for (int i = 1; i <= n; i++)
            elem[i][x] -= P * elem[i][y];
    }
    
    void inmultLin(int x, Polinom P)
    {
        // Inmultim fiecare element al liniei/coloanei x cu polinomul P
        for (int i = 1; i <= n; i++)
            elem[x][i] *= P;
    }
    void inmultCol(int x, Polinom P)
    {
        for (int i = 1; i <= n; i++)
            elem[i][x] *= P;
    }
    
    void impartLin(int x, Polinom P)
    {
        // Impartim fiecare element al liniei/coloanei x la termenul liber al lui P
        for (int i = 1; i <= n; i++)
            elem[x][i] /= P;
    }
    void impartCol(int x, Polinom P)
    {
        for (int i = 1; i <= n; i++)
            elem[i][x] /= P;
    }
    
    void colGuma(int x)
    {
        Polinom nul;
        int indiceElem = 0;
        
        // Mai intai gasim elementul nenul de pe linie/coloana
        for (int i = 1; i <= n; i++)
            if (elem[i][x] != nul) {
                indiceElem = i;
                break;
            }
        
        // Apoi stergem elementele de pe coloana/linie (invers ca inainte!)
        for (int i = 1; i <= n; i++)
            if (i != indiceElem)
                elem[x][i] = nul;

    }
    void linGuma(int x)
    {
        Polinom nul;
        int indiceElem = 0;
        
        for (int i = 1; i <= n; i++)
            if (elem[x][i] != nul) {
                indiceElem = i;
                break;
            }
        
        for (int i = 1; i <= n; i++)
            if (i != indiceElem)
                elem[i][x] = nul;
    }

};

template <class Tip2> std::istream& operator>> (std::istream& is, Matrice<Tip2>& M)
{
    is >> M.n;
    
    // Indexam incepand de la 1 pt ca asa facem operatiile (linia 1 <-> linia 2)
    for (int i = 1; i <= M.n; i++)
        for (int j = 1; j <= M.n; j++)
            is >> M.elem[i][j];
    
    return is;
}

template <class Tip2> std::ostream& operator<< (std::ostream& os, Matrice<Tip2> M)
{
    // Sa fie aliniate frumos, la LATIME spatii pe coloana
    const int LATIME = 15;
    
    for (int i = 1; i <= M.n; i++)
    {
        for (int j = 1; j <= M.n; j++)
        {
            std::ostringstream oss;
            oss << M.elem[i][j];
            
            os << oss.str();
            
            int dif = static_cast<int>(LATIME - oss.str().length());
            while(dif-- > 0)
                os << ' ';
            
        }
        os << "\n";
    }
    
    return os;
}

#endif
