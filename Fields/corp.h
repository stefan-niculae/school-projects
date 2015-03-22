#ifndef criptografie_corp_h
#define criptografie_corp_h

#include <cmath>                            // pt sqrt si pow
#include <sstream>                          // pt istringstream
#include <cstdlib>
#include "multime.h"
#include "legeCompozitie.h"

#define NMAX 100

#define ORICARE2(x,y)  for(x = M[i=0]; i<M.card; ++i, x = i<M.card? M[i]:x)    \
                         for(y = M[j=i]; j<M.card; ++j, y = j<M.card ? M[j]:y)
#define ORICARE3(x,y,z)for(x = M[i=0]; i<M.card; ++i, x = i<M.card? M[i]:x)    \
                         for(y = M[j=i]; j<M.card; ++j, y = j<M.card ? M[j]:y) \
                           for(z = M[k=j]; k<M.card; ++k, z = k<M.card ? M[k]:z)

class CorpOricare {     // ca sa putem avea pointer catre corp de orice tip
public:                 // ca sa putem citi si afisa corpuri de orice tip dintr-un pointer generic
    virtual void cit (std::istream&) { }
    virtual void afis(std::ostream&) { }
    virtual void setSkip(bool) { }
    //virtual bool finit();
    
    virtual void opIndicii(char, int, int) { }
    virtual void izomorfCu(CorpOricare&) const { }
};

template <class Tip>
class Corp : public CorpOricare
{
    Multime<Tip> M;
    bool abelian;
    
    bool defaultOp() { return Plus.defaultOp() && Ori.defaultOp(); }
    
    Tip opus[NMAX];
    Tip invers[NMAX];
    
public:
    Corp<Tip> (int nrElem = -1, int puterea = -1, std::string opPlus = "neinit", std::string opOri = "neinit");
    Corp<Tip> construire (int nrElem = -1, int puterea = -1, std::string opPlus = "neinit", std::string opOri = "neinit");
    Corp<Tip>& operator= (std::string s) {
        std::istringstream is(s);
        
        std::string P, O;
        int n, p;
        
        is >> n >> p >> P >> O;
        construire(n,p,P,O);
        
        return *this;
    }
    
    template <class Tip2> friend std::istream& operator>> (std::istream&, Corp<Tip2>&);
    template <class Tip2> friend std::ostream& operator<< (std::ostream&, Corp<Tip2> );
    template <class Tip2> friend std::string tipul(const Corp<Tip2>);
    
    Tip operator[] (int i) {
        return M[i];
    }
    Tip   opusulLui (Tip x) {
        if(M.Zn)
            return -x;
        
        if(SKIP_VERIF) {
            if(!M.finita())
                return -x;
            else
                return -M[M.pozitia(x)];
        }
        
        return opus[M.pozitia(x)];
    }
    Tip inversulLui (Tip x) {
        if(M.Zn)
            return 1/x;
        
        if(SKIP_VERIF) {
            if(!M.finita())
               return 1/x;
            else
               return 1/M[M.pozitia(x)];
        }
        
        if(x == M[0]) {
            std::cout << "Zero nu are invers!";
            exit(-1);
        }
        return invers[M.pozitia(x)];
    }
    
    LegeCompozitie<Tip> Plus;
    LegeCompozitie<Tip> Ori;
    Tip Minus (Tip x, Tip y) { return Plus(x,   opusulLui(y)); }
    Tip Impart(Tip x, Tip y) { return Ori (x, inversulLui(y)); }
    
    bool verif ();
    template <class Tip2, class Tip3> friend bool izomorfe (Corp<Tip2>, Corp<Tip3>);
    
    
    bool SKIP_VERIF;                // doar pt teste, de eliminat in ver finala!
    virtual void cit (std::istream& is) {
        is >> *this;
    }
    virtual void afis(std::ostream& os) {
        os << *this;
    }
    virtual void setSkip (bool cat) {
        SKIP_VERIF = cat;
    }
    //virtual bool finit() { return M.finita(); }
    virtual void opIndicii (char c, int x, int y) {
        
        std::cout << "Facem operatia ";
        switch (c) {
            case '+': std::cout << Plus  ; break;
            case '-': std::cout << "[opus de] " << Plus; break;
            case '*': std::cout << Ori   ; break;
            case '/': std::cout << "[invers de] " << Ori; break;
            default: throw 76;
        }
        std::cout << " intre " << M[x]<< " si " << M[y] << ", cu rezultatul ";
        
        switch (c) {
            case '+': std::cout << Plus  (M[x], M[y]); break;
            case '-': std::cout << Minus (M[x], M[y]); break;
            case '*': std::cout << Ori   (M[x], M[y]); break;
            case '/': std::cout << Impart(M[x], M[y]); break;
            default: throw 77;
        }
        std::cout << "\n\n";
    }
    virtual void izomorfCu (Corp<Tip>& K) const {
        std::cout << (izomorfe(*this, K) ? "\nI" : "\nNei") << "zomorfe!";
    }
    
};


template <class Tip> Corp<Tip>::Corp (int nrElem, int puterea, std::string opPlus, std::string opOri)
{
    construire(nrElem, puterea, opPlus, opOri);
}

template <class Tip> Corp<Tip> Corp<Tip>::construire (int nrElem, int puterea, std::string opPlus, std::string opOri)
{
    M.construire(nrElem, puterea);
    
    if(opPlus == "neinit") opPlus = "+";
    if(opOri  == "neinit") opOri  = "*";
    
    Plus.construire(opPlus, puterea);
    Ori.construire(opOri, puterea);
    
    verif();
    
    return *this;
}

bool prim (int x)
{
    for(int i=2; i<=sqrt(x); i++)
        if(x % i == 0)
            return 0;
    return 1;
}

template <class Tip> bool Corp<Tip>::verif()
{
    if(SKIP_VERIF) return 1;
    
 try {
    if(M.tipDeAfisat.find("int") != std::string::npos)                      // Z nu e corp, doar +-1 sunt inversabile
         throw 88;
    
    int nDefapt = M.card;
     
    if(M.Zn && M.card == -1)                                                // daca este inca neinitializat
         return 1;
     
    if(M.comp != -1) {//if(M.tipDeAfisat[0] == 'Z') {
        int i = 0;
        nDefapt = 0;
        while(M.tipDeAfisat[i++ +1] != '^') {
            nDefapt *= 10;
            nDefapt += M.tipDeAfisat[i] - '0';
        }
    }
     
    if(M.Zn && !prim(nDefapt))                                              // Zn doar prim
        throw 89;
    
    if((M.elementar() && !M.finita()) || M.Zn)                              // Q, R, C, Zn cu +, * sunt corp
        return 1;
    
    bool eBun = 0;
    for(int i=2; i<=100 && !eBun; i++)
        if(prim(pow(M.card, 1/i)))
            eBun = 1;
    if(!eBun)                                                               // corpurile finite pot avea doar p^n elem
        return 90;
    
    Tip x, y, z;
    int i, j, k;
    bool ENPlus = 0, ENOri = 0;
    
    // (K, +) grup abelian; (K\0, *) grup (abelian); folosim acelasi oricare pt ca nu ne deranjeaza 0 * a, (K,*) e monoid
    abelian = 1;
    ORICARE2(x, y) {
        if(!M.contine (Plus(x,y))) throw 91;                                // K ps in rap cu +
        if(!M.contine (Ori (x,y))) throw 92;                                // K ps in rap cu *
        if(Plus(x,y) != Plus(y,x)) throw 93;                                // + comut peste K
        if(Ori (x,y) != Ori (y,x))  abelian = 0;                            // (* comut peste K)
    }
    
    ORICARE3(x, y, z) {
        if(Plus(x, Plus(y,z)) != Plus(Plus(x,y), z)) throw 94;              // + asoc peste K
        if(Ori (x, Ori (y,z)) != Ori (Ori (x,y), z)) throw 95;              // * asoc peste K
    }
    
    
    // gasim elem neutru(zero) pt +
    for(x = M[i=0]; i<M.card; x = M[i++]) {
        for(y = M[j=i]; j<M.card; y = M[j++])
            if(Plus(x,y) != y)                                              // comut stim ca e deja
                break;
        
        if(j == M.card) {                                                   // daca l-am gasit, il punem pe poz 0
            ENPlus = 1;
            M.mutaPePoz(x, 0);
            break;
        }
    }
    if(!ENPlus) throw 96;
    
    // construim U(K) in rap cu +
    for(int i=0; i<M.card; i++) opus[i] = M[0];
    ORICARE2(x, y) {
        if(i && Plus(x,y) == M[0])
            opus[i] = y, opus[j] = x;
    }
    for(i=1; i<M.card; i++)
        if(opus[i] == M[0])                                                 // cum inversul este unic si zero este inv lui zero
            throw 97;
    
    
    // gasim elem neutru(unu) pt *
    for(x = M[i=1]; i<M.card; x = M[i++]) {
        for(y = M[j=i]; j<M.card; y = M[j++])
            if(Ori(x,y) != y)
                break;
        
        if(j == M.card) {                                                   // daca l-am gasit, il punem pe poz 1
            ENOri = 1;
            M.mutaPePoz(x, 1);
            break;
        }
    }
    if(!ENOri) throw 98;
    
    // construim U(K\0) in rap cu *
    for(int i=0; i<M.card; i++) invers[i] = M[1];
    ORICARE2(x, y) {
        if(i > 1 && Ori(x,y) == M[1])
            invers[i] = y, invers[j] = x;
    }
    for(i=2; i<M.card; i++)
        if(invers[i] == M[1])                                               // cum inversul este unic si unu este inv lui unu
            throw 99;
    
 } catch(int eroare) {
    std::cout << "Multimea " << M << " impreuna cu " << Plus << " si " << Ori << " nu formeaza Corp!\n";
    exit(eroare);
}
    
    return 1;
}

template <class Tip2> std::istream& operator>> (std::istream& is, Corp<Tip2>& K)
{
    is >> K.M;
    
    is >> K.Plus;
    is >> K.Ori;
    
    K.Plus.nVect = K.Ori.nVect = K.M.comp;
    
    K.verif();
    
    return is;
}
template <class Tip2> std::ostream& operator<< (std::ostream& os, Corp<Tip2> K)
{
    os << "\nMultime: " << K.M << "\n";
 
    os << "Operatia 1: " << K.Plus << "\n";
    os << "Operatia 2: " << K.Ori << "\n";
    
    return os;
}

int succesor(int *st, int k, int n) {
    if(st[k]<n) {
        st[k]++;
        return 1;
    }
    return 0;
}
int valid(int* st, int k, int n) {
    for(int i=1; i<k; i++)
        if(st[i] == st[k])
            return 0;
    return 1;
}

#define f(x) K2.M[st[K1.M.pozitia(x)]]                                  // f: K1 -> K2
template <class Tip2, class Tip3> bool izomorfe (Corp<Tip2> K1, Corp<Tip3> K2)
{
    if( (K1.M.comp != K2.M.comp) ||                                     // pt vectori cu nr dif de comp
        (K1.M.card != K2.M.card) )                                      // dc au nr dif de elem (sau fin/inf)
        return 0;

    if(!K1.M.finita() && K1.M.elementar()) {                            // pt mult infinite
        if(K1.M.reala || K1.M.complexa)                                 // cand una dintre multimi e reala sau complexa
            return (K2.M.reala || K2.M.complexa);                       // cealalata nu poate fi decat reala/complexa
        return (K1.M.rationala && K2.M.rationala) ||                    // ori ambele sunt rationale
               (K1.M.Zn && K2.M.Zn);                                    // ori ambele Zn (am verif acelasi n mai sus)
    }
    
    if(K1.M.finita() && K2.M.finita())                                  // orice corp finit Fn
        return (K1.M.Zn || K2.M.Zn);                                    // este izom cu Zn
    
    int n = K1.M.card, st[NMAX], k, as;                                 // backtracking
    st[k = 1] = 0;
    
    while(k) {                                                          // il permutam in toate halurile
        do {                                                            // f[x] simbolizand f(x)
            as = succesor(st,k,n);                                      // am generat toate functiile posibile
        } while (as && !valid(st,k,n));                                 // daca niciuna nu e izomorfism, nu sunt izomorfe
        if(as)
            if(k==n) {
                /*
                std::cout << std::endl << "\nPERMUTAREA: ";
                for(int i=1; i<=n; i++)
                    std::cout << st[i] << ' ';
                std::cout << std::endl;
                 */
                
                int i, j;
                Tip2 x, y;                                              // f(x +_1 y) = f(x) +_2 f(y)
                for(x = K1.M[i=0]; i<K1.M.card; ++i, x = i<K1.M.card? K1.M[i]:x)    \
                    for(y = K1.M[j=i]; j<K1.M.card; ++j, y = j<K1.M.card ? K1.M[j]:y)
                        if( f( K1.Plus(x, y) ) != K2.Plus( f(x), f(y) )  ||
                            f( K1.Ori (x, y) ) != K2.Ori ( f(x), f(y) )   )
                            return 0;
            }
            else
                st[++k] = 0;
        else
            k--;
    }
    
    return 1;
}

#endif
