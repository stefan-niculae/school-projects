#ifndef criptografie_multime_h
#define criptografie_multime_h

#include <math.h> // pt pow

#include "vector.h"
#include "Zn.h"
#include "complexe.h"
#include "tipulVariabilei.h"

#define NMAX 100

template<class Tip> class Corp;

template <class Tip>
class Multime
{
    int card, comp;
    Tip elem[NMAX];
    
    bool rationala, reala, complexa, Zn;
    bool finita() { return (card != -1); }
    bool vida() { return (card == 0); }
    bool elementar() { return rationala || reala || complexa || Zn; }
    
    std::string tipDeAfisat;
    
public:
    Multime(int nrElem =  -1, int puterea = -1);
    Multime<Tip> construire(int nrElem = -1, int puterea = -1);
    
    Tip operator[] (int i) {
        if(!finita()) {
            std::cout << "Multimea este nenumarabila!";
            return NULL;
        }
        if(i < 0 || i > card-1) {
            std::cout << i << " nu se afla intre indicii 0 si " << card-1 << "!";
            exit(200);
        }
        return elem[i];
    }
    
    template <class Tip2> friend std::istream& operator>> (std::istream&, Multime<Tip2>&);
    template <class Tip2> friend std::ostream& operator<< (std::ostream&, Multime<Tip2> );
    template <class Tip2> friend std::string tipul(const Multime<Tip2>);
    
    int pozitia (Tip x) {
        for(int i=0; i<card; i++)
            if(elem[i] == x)
                return i;
        return -1;
    }
    bool contine (Tip x) {
        return pozitia(x) != -1;
    }
    void mutaPePoz (Tip x, int p) {
        Tip temp = elem[p];
        elem[p] = x;
        elem[pozitia(x)] = temp;
    }
    
    friend class Corp<Tip>;
    template <class Tip2, class Tip3> friend bool izomorfe (Corp<Tip2>, Corp<Tip3>);
    template <class Tip2> friend std::istream& operator>> (std::istream&, Corp<Tip2>&);
};

template <class Tip> Multime<Tip>::Multime (int nrElem, int puterea)
{
    construire(nrElem, puterea);
}

std::string intToString(int x)
{
    std::string s, sRev;
    
    while(x) {
        s += x%10 + '0';
        x /= 10;
    }
    
    unsigned long a = 0; a--;
    for(unsigned long i=s.length() -1; i!=a; i--)
        sRev += s[i];
    
    return sRev;
}

void initVectZn(Vector<class Zn>* elem, int& n, int p)
{
    int st[100], k;         // backtracking pt Aranjamente de n luate cate p (includem si zero)
    int j = 0;
    
    st[k = 1] = -1;
    while(k)
        if(st[k] < n -1) {      // pt ca in Zn avem {0, 1, ..., n-1}
            st[k]++;
            if(k==p) {
                elem[j].set_n(p);
                
                for (int i=1; i<=k; i++) {
                    Zn x(n, st[i]);
                    elem[j].set_elem(i-1, x);       // punem pe poz i-1 pt ca sunt indexate de la 0
                }
                
                j++;
            }
            else
                st[++k] = -1;
        }
        else
            k--;
    
    
    n = pow(n, p);
}

template<class Tip> Multime<Tip> Multime<Tip>::construire(int nrElem, int puterea)
{
    card = nrElem;
    comp = puterea;
    
    tipDeAfisat = tipul(*this);
    if(tipDeAfisat.find("Vector") == std::string::npos && comp != -1) {
        std::cout << "Ai zis ca multimea va avea elemente cu " << comp << " componente, dar nu ai definit ca vector!";
        exit(80);
    }
    
    reala = rationala = complexa = Zn = 0;
    if(tipDeAfisat.find("long double") != std::string::npos) tipDeAfisat = "Real",     reala = 1;
    if(tipDeAfisat.find     ("double") != std::string::npos) tipDeAfisat = "Rational", rationala = 1;
    if(tipDeAfisat.find    ("Complex") != std::string::npos) tipDeAfisat = "Complex",  complexa = 1;
    if(tipDeAfisat.find         ("Zn") != std::string::npos) tipDeAfisat = "Z",        Zn = 1;
    
    if(Zn) {
        if(card == -1)
            tipDeAfisat += "n";
        else {
            tipDeAfisat += intToString(card);
            Zn::nInitSet(card);                                                 // am setat n-ul pt Zn
            for(int i=0; i<card; i++)
                elem[i] = i;                                                    // acum setam valorile, 0, 1, ..., n-1
        }
    }
    
    if(comp == -1 && tipul(*this)[0] == 'V') tipDeAfisat += "^n";
    if(comp != -1) {
        tipDeAfisat += "^";
        tipDeAfisat += intToString(comp);
        
        for(int i=0; i<card; i++)
            elem[i] = comp;                                // setam nr de comp pt fiecare elem(vector)

        if(Zn) initVectZn((Vector<class Zn>*)elem, card, comp);
    }
    
    return *this;
}

template <class Tip2> std::istream& operator>> (std::istream& is, Multime<Tip2>& A)
{
    is >> A.card;
    if(tipul(A).find("Vector") != std::string::npos)
        is >> A.comp;                                          // dc a fost declarat ca vector, citim si puterea/nr de comp
    else
        A.comp = -1;
    A.construire(A.card, A.comp);
    
    if(A.Zn)                                                // dc este un Zn, a fost deja init
        return is;
    
    for(int i=0; i<A.card; i++)
        is >> A.elem[i];
    
    return is;
}
template <class Tip2> std::ostream& operator<< (std::ostream& os, Multime<Tip2> A)
{
    os << A.tipDeAfisat;
    
    if(A.finita()) {
        os << "(" << A.card << " elem)";
        for(int i=0; i<A.card; i++)
            os << "\n\t\t" << A.elem[i];
    }
    else
        os << " infinita";
    
    return os;
}

#endif
