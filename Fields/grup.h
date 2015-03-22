#ifndef criptografie_grup_h
#define criptografie_grup_h

#include <math.h>
#include "multime.h"

#define NMAX 100

bool prim(int);

template <class Tip>
class Grup
{
    int finit;
    Tip elem[NMAX];
    
    bool defaultPlus;
    bool defaultOri;
    Tip* customPlus(Tip, Tip);
    Tip* customOri (Tip, Tip);
    
    //char tipOpPlus;
    //Tip customPlus(Tip, Tip);
    
public:
    Grup<Tip> () : finit(0), defaultPlus(1), defaultOri(1) { };    // constructor grup infinit
    Grup<Tip> (int nrElem, char operatie = NULL);                           // constructor grup   finit
    
    template <class Tip2> friend std::istream& operator>> (std::istream&, Grup<Tip2>&);
    template <class Tip2> friend std::ostream& operator<< (std::ostream&, Grup<Tip2> );
    template <class Tip2> friend std::string tipul(const Grup<Tip2>);
    
    Tip operator[] (int i) { return elem[i]; }
};

template <class Tip> Grup<Tip>::Grup (int nrElem, char operatie)
{
    if(nrElem == 0) return;
    
    finit = nrElem;
    
    /*if(tipul(*this)[0] == 'Z')
        for(int i=0; i<finit; i++) {
            Zn a(finit, i);
            elem[i] = a;
        }*/
}

template <class Tip2> std::istream& operator>> (std::istream& is, Grup<Tip2>& G)                    // citire grup
{
    is >> G.finit;
    if(!G.finit)
        return is;
    
    for(int i=1; i<=G.finit; i++)
        is >> G.elem[i];
    
    return is;
}
template <class Tip2> std::ostream& operator<< (std::ostream& os, Grup<Tip2> G)                     // afisare grup
{
    os << tipul(G);
    
    if(!G.finit) os << ", infinit";
    else os << ", cu " << G.finit << " elemente: ";
    
    for(int i=0; i<G.finit; i++)
        os << G[i] << ", ";
    
    if(G.tipOpPlus == '+') os << "\nAditiv";
    else if (G.tipOpPlus == '*') os << "\nMultiplicativ";
    else os << "\nCustom";
    
    return os;
}

#endif
