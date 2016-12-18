#ifndef operatiiMatrice_Polinom_h
#define operatiiMatrice_Polinom_h
#include <sstream> // pt istringstream
#include <cmath> // pt log10

class Polinom
{
    static const int SIZE = 100;
    
    double coef[SIZE];
    
public:
    
    // Constructor chior
    Polinom()
    {
        memset(coef, 0, sizeof(coef));
    }
    
    // Constructor din string (ex: 3x^10 + x^2 - 77)
    Polinom(std::string);
    
    Polinom(int x)
    {
        memset(coef, 0, sizeof(coef));
        coef[0] = x;
    }
    
    // Supraincarcare operator scriere
    friend std::ostream& operator<< (std::ostream&, Polinom);
    
    void setCoef(int puterea, double coeficientul)
    {
        coef[puterea] = coeficientul;
    }
    int getCoef(int gradul)
    {
        return coef[gradul];
    }
    
    Polinom& operator= (const Polinom& P)
    {
        memcpy(coef, P.coef, sizeof(coef));
        return *this;
    }
    
    friend Polinom operator+  (const Polinom&, const Polinom&);
    friend Polinom operator+= (Polinom&, const Polinom&);
    
    friend Polinom operator-  (const Polinom&, const Polinom&);
    friend Polinom operator-= (Polinom&, const Polinom&);
    
    friend Polinom operator*  (const Polinom&, const Polinom&);
    friend Polinom operator*= (Polinom&, const Polinom&);
    
    friend Polinom operator/  (const Polinom&, const Polinom&);
    friend Polinom operator/= (Polinom&, const Polinom&);
    
    friend bool operator== (const Polinom& f, const Polinom& g)
    {
        return !memcmp(&f, &g, sizeof(f));
    }
    friend bool operator!= (const Polinom& f, const Polinom& g)
    {
        return !(f == g);
    }
    
};

// Constructor din string (ex: 3x^10 + x^2 - 77)
Polinom::Polinom(std::string s)
{
    // Initializam toti coeficientii cu zero
    memset(coef, 0, sizeof(coef));
    
    // Cat timp nu am terminat de parsat stringul
    while (s.length() != 0)
    {
        int semn = 1, putereCurenta = 1;
        double coefGradCurent = 1;

        bool monic = (s[0] == 'x') || (s[0] == '-' && s[1] == 'x');
        
        // Stergem spatiile si + sau - dintre monoame
        while ((s[0] > '9' || s[0] < '0') && (!monic || (monic && s[0] == '-')))
        {
            //std::cout << "stergem " << s[0] << "\n";
            // Daca dam peste un -, tinem minte insa
            if (s[0] == '-')
                semn = -1;
            
            s.erase(0,1);
        }
        if (!s.length())
            break;
        
        if (!monic) {
            
            // Citim din string coeficientul
            std::istringstream(s) >> coefGradCurent;
        
            // Stergem coeficientul citit
            s.erase(0, (int)log10(coefGradCurent) + 1);
            
        }
        
        // Stergem si caracterele x, ^ si spatii sau ce mai sunt
        bool grad1 = 1, grad0 = 1;
        
        if(0 == s.length())
            grad1 = 0;
        
        else
            while (s[0] > '9' || s[0] < '0')
            {
                // Daca dam peste un x inseamna ca nu avem grad 0 (adica termen liber)
                if (s[0] == 'x')
                    grad0 = 0;
                
                // Daca dam peste un ^ inseamna ca nu avem nici grad 1 nici grad 0
                if (s[0] == '^')
                    grad1 = grad0 = 0;
                
                if ((monic && s[0] == '-') || 0 == s.length())
                    break;
                
                s.erase(0, 1);
            }
        
        // Avem grija sa inmultim cu -1 daca am citit un minus
        coefGradCurent *= semn;
        //std::cout << "Am citit coeficientul " << coefGradCurent << " si stringul este: " << s << std::endl;
        
        if (grad1) {
            //std::cout << "Suntem in cazul de grad 1\n";
            coef[1] = coefGradCurent;
            continue;
        }
        else if (grad0) {
            //std::cout << "Suntem in cazul de grad 0\n";
            coef[0] = coefGradCurent;
            continue;
        }
        
        // Citim puterea pt care am citit anterior coeficientul
        std::istringstream(s) >> putereCurenta;
        
        // Stergem puterea citita
        s.erase(0, (int)log10(putereCurenta) + 1);
        
        //std::cout << "Am citit puterea " << putereCurenta << " si stringul este: " << s << std::endl;
        
        // Cu informatiile citite completam monomul
        coef[putereCurenta] = coefGradCurent;
        
    }
}

std::ostream& operator<< (std::ostream& os, Polinom P)
{
    int primul = 1;
    
    // Pornim de la gradul cel mai mare
    for (int i = Polinom::SIZE -1; i >= 0; i--)
    {
        // Daca coeficientul este nenul (nu afisam degeaba 0 * x^99 + 0 * x^98...)
        if (P.coef[i]) {
            
            // Afisam plusuri intre monoame (minusurile se afiseaza automat pt ca avem coef negativ
            if (P.coef[i] > 0 && !primul)
                os << '+';
            primul = 0;
            
            // Nu afisam 1x ci doar x, dar afisam 1 cand e termen liber
            if (P.coef[i] == -1 && i != 0)
                os << '-';
            else if (P.coef[i] != 1 || i == 0)
                os << P.coef[i];
        
            // La termenul liber nu mai afisam x
            if (i != 0)
                os << 'x';
            
            // Numai afisam si x^1 si x^0
            if (i > 1)
                os << '^' << i;
            
            os << ' ';
        }
    }
    
    // Daca polinomul este nul afisam 0
    if (primul)
        os << 0;
    
    return os;
}

Polinom operator+ (const Polinom& f, const Polinom& g)
{
    Polinom h;
    
    // Adunare chioara, coeficientii pt grade
    for (int i = 0; i < Polinom::SIZE; i++)
        h.coef[i] = f.coef[i] + g.coef[i];
    
    return h;
}
Polinom operator+= (Polinom& f, const Polinom& g)
{
    f = f + g;
    return f;
}

Polinom operator- (const Polinom& f, const Polinom& g)
{
    Polinom g1;
    
    // Pt f - g inmultim toti coeficientii lui g cu -1 si returnam f + (-g)
    for (int i = 0; i < Polinom::SIZE; i++)
        g1.coef[i] = -g.coef[i];
    
    return f + g1;
}
Polinom operator-= (Polinom& f, const Polinom& g)
{
    f = f - g;
    return f;
}

Polinom operator* (const Polinom& f, const Polinom& g)
{
    Polinom h;
    
    // Formula: monomul de grad (i+j) al rezultatului este
    // suma dupa coef de grad i din f inmultit cu coef de grad j din g
    for (int i = 0; i < Polinom::SIZE; i++)
        for (int j = 0; j < Polinom::SIZE; j++)
            h.coef[i + j] += f.coef[i] * g.coef[j];
    
    return h;
}
Polinom operator*= (Polinom& f, const Polinom& g)
{
    f = f * g;
    return f;
}

Polinom operator/ (const Polinom& f, const Polinom& g)
{
    Polinom h;
    
    // DOAR PT IMPARTIRE LA O CONSTANTA!!!
    // (ca numai dinastea avem voie la operatii elementare pe linii)
    for (int i = 0; i < Polinom::SIZE; i++)
        h.coef[i] = f.coef[i] / g.coef[0];
    
    return h;
}
Polinom operator/= (Polinom& f, const Polinom& g)
{
    f = f / g;
    return f;
}

#endif
