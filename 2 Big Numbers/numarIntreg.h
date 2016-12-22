#ifndef NumereMari_numarIntreg_h
#define NumereMari_numarIntreg_h

#define NMAX 100
#define ULL_MAX 18446744073709551615 // (2^64 -1)
#define lung unsigned long long
#define CONV(x) (short)x - '0'

using namespace std;

class NrReal;

class NrIntreg
{
    struct nod {
        unsigned short val;
        nod *ant, *urm;
    };
    void adaug(short x, char unde) {
        nod *c = new nod;                                           // construim nodul
        c->val = x;
        c->ant = c->urm =  NULL;
        
        if (!dom)                                                   // daca lista este vida
            dom = unit = c;                                         // nodul va fi primul (si ultimul)
        else if (unde == 's') {                                     // altfel il adaugam la final
            c->ant = unit;
            unit->urm = c;
            unit = c;
        }
        else if (unde == 'i') {                                     // sau la inceput
            c->urm = dom;
            dom->ant = c;
            dom = c;
        }
        
        lungime++;
    }
    
    nod *unit, *dom;
    bool semn;  // 1 pt negativ, 0 pt pozitiv sau zero
    lung lungime;
    
public:
    NrIntreg() : unit(NULL), dom(NULL), semn(0), lungime(0) { }
    NrIntreg(long long);
    NrIntreg(string);
    NrIntreg(const NrIntreg&);
    ~NrIntreg();
    NrIntreg& operator= (const NrIntreg&);
    NrIntreg& operator() (string); // reinitializare (o folosim la citire)
    NrIntreg& operator() (long long);
    
    friend std::istream& operator>> (std::istream&, NrIntreg&);
    friend std::ostream& operator<< (std::ostream&, NrIntreg);
    
    operator lung();
    unsigned short& operator[] (lung);
    
    bool pozitiv();
    lung getLungime();
    void minus();
    lung stergereZerouriDominante();
    friend NrIntreg maxAbs(NrIntreg, NrIntreg);
    friend void swap(NrIntreg&, NrIntreg&);
    
    friend bool operator!  (NrIntreg&);
    friend bool operator== (NrIntreg&, NrIntreg&);
    friend bool operator!= (NrIntreg&, NrIntreg&);
    friend bool operator<  (NrIntreg&, NrIntreg&);
    friend bool operator>  (NrIntreg&, NrIntreg&);
    friend bool operator<= (NrIntreg&, NrIntreg&);
    friend bool operator>= (NrIntreg&, NrIntreg&);
    
    friend NrIntreg operator+ (NrIntreg&, NrIntreg&);
    friend NrIntreg operator- (NrIntreg&, NrIntreg&);
    friend NrIntreg operator* (NrIntreg&, NrIntreg&);
    friend NrIntreg operator/ (NrIntreg&, NrIntreg&);
    friend NrIntreg operator<< (NrIntreg, lung);
    friend NrIntreg operator>> (NrIntreg, lung);
    friend NrIntreg operator+= (NrIntreg&, NrIntreg&);
    friend NrIntreg operator-= (NrIntreg&, NrIntreg&);
    friend NrIntreg operator*= (NrIntreg&, NrIntreg&);
    friend NrIntreg operator/= (NrIntreg&, NrIntreg&);
    friend NrIntreg operator<<= (NrIntreg&, lung);
    friend NrIntreg operator>>= (NrIntreg&, lung);
    
    friend class NrReal;
    operator NrReal();
    NrIntreg& operator= (NrReal);
    lung stergereZerouriFinale();
};

NrIntreg::NrIntreg(long long x)
{
    dom = unit = NULL;
    lungime = 0;
    semn = x < 0;
    
    for(; x ; x /= 10)
        adaug(x%10, 'i');
}
NrIntreg::NrIntreg(string s)
{
    dom = unit = NULL;
    lungime = 0;
    semn = s[0] == '-';
    
    for(int i=semn; s[i]; i++)
        adaug(CONV(s[i]), 's');
}
NrIntreg::NrIntreg(const NrIntreg& sursa)
{
    *this = *new NrIntreg;
    this->dom = this->unit = NULL;
    
    this->lungime = 0;
    this->semn = sursa.semn;
    
    for(nod *c = sursa.dom; c; c=c->urm)
        adaug(c->val, 's');
}
NrIntreg& NrIntreg::operator= (const NrIntreg& sursa)
{
    this->semn = sursa.semn;
    this->lungime = 0;
    this->dom = this->unit = NULL;
    
    for(nod *c = sursa.dom; c; c=c->urm)
        adaug(c->val, 's');
    
    return *this;
}
NrIntreg& NrIntreg::operator() (string s)
{
    NrIntreg temp(s);
    *this = temp;
    
    return *this;
}
NrIntreg& NrIntreg::operator() (long long k)
{
    NrIntreg temp(k);
    *this = temp;
    
    return *this;
}
NrIntreg::~NrIntreg()
{
    if(!dom) return;
    
    nod *c;
    for(c = dom->urm; c; c=c->urm)                              // mergem de la a doua spre sfarsit
        delete c->ant;                                          // si mereu o stergem pe cea anterioara
    delete c;                                                   // stergem apoi si ultima cifra (sau singura daca nu are >= 2 cifre)
}

std::istream& operator>> (std::istream& is, NrIntreg& A)
{
    char s[NMAX];
    is >> s;
    
    A(s);
    
    return is;
}
std::ostream& operator<< (std::ostream& os, NrIntreg A)
{
    if(A.semn)
        os << '-';
    
    for(NrIntreg::nod *c = A.dom; c; c=c->urm)
        os << c->val;
    
    return os;
}

NrIntreg::operator lung()
{
    try {
        if(lungime > 19)
            throw lungime;
    } catch(int eroare) {
        std::cout << "EROARE(" << eroare << "): " << " numarul nu mai incape in ULL!\n";
        exit(eroare);
    }
 
    lung temp = 0;
    
    for(nod* c = dom; c; c=c->urm)
        temp *= 10, temp += c->val;
        
    return temp;
}
unsigned short& NrIntreg::operator[] (lung k)
{
    try {
        if(k > lungime)
            throw 1;
        else if(k == 0)
            throw 0;
    } catch(int eroare) {
        if(eroare == 1)
            std::cout << "EROARE(" << eroare << "): indicele mai mare decat lungimea!\n";
        else if(eroare == 0)
            std::cout << "EROARE(" << eroare << "): incepem indexarea de pe pozitia 1!\n";
        exit(eroare);
    }
    
    nod* c;
    for(c = dom; k-1; --k, c=c->urm);
    
    return c->val;
}

bool NrIntreg::pozitiv()
{
    return !semn;
}
lung NrIntreg::getLungime()
{
    return lungime;
}
void NrIntreg::minus()
{
    semn = !semn;
}
lung NrIntreg::stergereZerouriDominante()
{
    nod* c;
    int k = 0;

    if(dom->urm && !dom->val) {                                     // daca nu are mai mult de o cifra, nu poate avea zerouri dominante
        k = 1;
        
        for(c = dom->urm; c && !c->val; c=c->urm, k++) {
            delete c->ant;
            lungime--;
        }
        
        c->ant = NULL;
        dom = c;
    }
    
    return k;                                                       // nrul de zerouri sterse
}
void swap(NrIntreg& A, NrIntreg& B)
{
    NrIntreg temp = A;
    A = B;
    B = temp;
}

bool operator! (NrIntreg& A)
{
    if(!A.dom->val && A.dom->urm->val)
        A.stergereZerouriDominante();
  
    return !A.dom->val;
}
bool operator== (NrIntreg& A, NrIntreg& B)
{
    if(A.semn != B.semn || A.lungime != B.lungime)
        return 0;
    
    for(lung i=1; i<=A.lungime; i++)
        if(A[i] != B[i])
            return 0;
    
    return 1;
}
bool operator!= (NrIntreg& A, NrIntreg& B)
{
    return !(A == B);
}
bool operator<  (NrIntreg& A, NrIntreg& B)
{
    A.stergereZerouriDominante();
    B.stergereZerouriDominante();
    
    if(A.semn && !B.semn) return 1;                                 // daca A este negativ (si B pozitiv), evident este mai mic
    if(!A.semn && B.semn) return 0;                                 // pt invers, este invers
    
    if(A.lungime < B.lungime) return 1;                             // daca A are mai putine cifre, evident este mai mic
    if(B.lungime < A.lungime) return 0;                             // daca nu, nu
    
    for(lung i=1; i<=A.lungime; i++)
        if(A[i] > B[i])
            return 0;
        else if(B[i] > A[i])
            return 1;
    
    return 0;
}
bool operator>  (NrIntreg& A, NrIntreg& B)
{
    return !(A == B) && !(A < B);
}
bool operator<= (NrIntreg& A, NrIntreg& B)
{
    return (A == B) || (A < B);
}
bool operator>= (NrIntreg& A, NrIntreg& B)
{
    return (A == B) || (A > B);
}
NrIntreg maxAbs(NrIntreg A, NrIntreg B)
{
    NrIntreg tempA = A, tempB = B;
    tempA.semn = tempB.semn = 0;
    return tempA > tempB ? A : B;
}

NrIntreg operator+ (NrIntreg& A, NrIntreg& B)
{
    if(A.semn != B.semn) {
        NrIntreg temp = B;
        temp.semn = A.semn;
        return A - temp;
    }
    
    NrIntreg M = A.lungime > B.lungime ? A : B;
    NrIntreg m = A.lungime > B.lungime ? B : A;
    
    unsigned short transp = 0;
    lung i, j;
    for(i = m.lungime, j = M.lungime; i>=1; i--, j--) {
        M[j] += m[i] + transp;
        transp = M[j]/10;
        M[j] %= 10;
    }
    
    if(transp) {
        if(M.lungime == m.lungime)
            M.adaug(transp, 'i');
        else
            M[j] += transp;
    }
    
    return M;
}
NrIntreg operator- (NrIntreg& A, NrIntreg& B)
{
    if(A.semn != B.semn) {
        NrIntreg temp = B;
        temp.semn = A.semn;
        return A + temp;
    }
    
    NrIntreg deCeNuMerge;
    NrIntreg M = A, m = B;
    if(M != (deCeNuMerge = maxAbs(A, B))) {
        swap(M,m);
        M.semn = 1;
    }
    
    bool transp = 0;
    short temp;
    lung i, j;
    
    for(i = m.lungime, j=M.lungime; i>=1; i--, j--) {
        temp = M[j] - (m[i] + transp);
        if(temp < 0) M[j] = temp + 10, transp = 1;
        else         M[j] = temp,      transp = 0;
    }
    
    M.stergereZerouriDominante();
    return M;
}
NrIntreg operator* (NrIntreg& A, NrIntreg& B)
{
    NrIntreg rez;
    lung k = A.lungime;while(k--) rez.adaug(0, 's');
    k = B.lungime - 1; while(k--) rez.adaug(0, 's');
    
    for(lung i = A.lungime; i>=1; i--)
        for(lung j = B.lungime; j>=1; j--)
            rez[i+j-1] += A[i] * B[j];
    
    unsigned short transp = 0;
    for(lung i=rez.lungime; i>=1; i--) {
        rez[i] += transp;
        transp = rez[i]/10;
        rez[i] %= 10;
    }
    if(transp)
        rez.adaug(transp, 'i');

    return rez;
}
NrIntreg operator<< (NrIntreg A, lung k)
{
    while(k--)
        if(A.unit != A.dom) {
            NrIntreg::nod *c = A.unit->ant;
            
            A.unit = c;
            A.unit->urm = NULL;
            
            delete c;
            
            A.lungime--;
        }
    return A;
}
NrIntreg operator<<= (NrIntreg& A, lung k)
{
    A = A << k;
    return A;
}

lung NrIntreg::stergereZerouriFinale()
{
    lung k = 0;
    
    return k;
}

#endif
