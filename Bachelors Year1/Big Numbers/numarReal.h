#ifndef NumereMari_numarReal_h
#define NumereMari_numarReal_h

#include "numarIntreg.h"

class NrIntreg;

class NrReal
{
    NrIntreg num;
    NrIntreg exp;
    
public:
    NrReal() : num('0'), exp('0') { }
    NrReal (NrIntreg);
    NrReal& operator= (NrIntreg);
    NrReal& operator= (NrReal);
    
    operator NrIntreg();
    
    friend std::istream& operator>> (std::istream&, NrReal&);
    friend std::ostream& operator<< (std::ostream&, NrReal);
    
    friend class NrIntreg;
    friend NrIntreg& NrIntreg::operator= (NrReal);
    
    friend NrReal operator+ (NrReal&, NrReal&);
    friend NrReal operator+ (NrReal&, NrReal&);
};

std::istream& operator>> (std::istream& is, NrReal& A)
{
    return is >> A.num >> A.exp;
}
std::ostream& operator<< (std::ostream& os, NrReal A)
{
    lung n = A.num.getLungime();
    lung e = A.exp;
    
    if(!A.exp.pozitiv() && n > e)                               // daca avem virgula printre cifre
        for(lung i=1; i<=n; i++) {
            os << A.num[i];
            if(i == n - e)
                os << ',';
        }
    
    else {
        if(!A.num.pozitiv()) {                                  // daca numarul este negativ
            os << "-";
            A.num.minus();
        }
        if(!A.exp.pozitiv())                                    // daca exponentul este negativ
            os << "0,";
        os << (NrIntreg)A;                                      // folosim conversia si afisarea deja facute
    }
    
    return os;
}

NrReal& NrReal::operator= (NrReal A)
{                                                               // real -> real
    num = A.num;
    exp = A.exp;
    
    return *this;
}

NrIntreg::operator NrReal()
{                                                               // intreg -> real
    NrReal A;
    A = *this;
    return A;
}
NrReal& NrReal::operator= (NrIntreg A)
{                                                               // intreg -> real (aici se lucreaza)
    lung k = A.stergereZerouriFinale();
    exp(k);                             // in exp punem numarul de zerouri de la finalul Intregului si le si stergem in acelasi timp
    num = A;                                                    // folosim operatorul= de la Intreg
    
    return *this;
}
NrReal::NrReal (NrIntreg A)
{                                                               // copy-constructor Real = Intreg
    NrReal temp;
    temp = A;
    *this = temp;
}

NrIntreg& NrIntreg::operator= (NrReal A)
{                                                               // real -> intreg
    *this = (NrIntreg)A;
    return *this;
}
NrReal::operator NrIntreg()
{                                                               // real -> intreg (aici se lucreaza)
    NrIntreg A = this->num;                                     // initializam cu partea de numar din Real
    
    lung e = exp;                                               // in e retinem valoarea exponentului
    lung n = num.getLungime();                                  // in n retinem lungimea numarului
    
    if(exp.pozitiv())                                           // daca exp este pozitiv
        while(e--)                                              // adaugam zerouri la final
            A.adaug(0, 's');
    
    else {                                                      // daca exponentul este negativ
        if(n <= e)                                              // daca avem mai putine cifre decat valoarea exponentului
            while(e-- - n)
                A.adaug(0, 'i');
    }
        
    return A;
}

NrReal operator+ (NrReal& A, NrReal& B)
{
    NrIntreg t1 = A, t2 = B,
    t3 = t1 + t2;
    
    return (NrReal)t3;
}
NrReal operator- (NrReal& A, NrReal& B)
{
    NrIntreg t1 = A, t2 = B,
    t3 = t1 - t2;
    
    return (NrReal)t3;
}

#endif
