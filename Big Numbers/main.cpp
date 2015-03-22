/*
 * copyright Stefan Niculae gr 131
 * feb14 FMIUB A1S2 OOP
 * Proiect1: nre reale mari
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

# define NMAX 1000
# define ULL_MAX 18446744073709551615 // (2^64 -1)
# define lung unsigned long long

class nrReal {
    struct nod {
        short val;
        nod *ant, *urm;
    };
    
    nod *numUnit, *numDom;
    nod *expUnit, *expDom;
    bool semnNum, semnExp; // 0 este +, 1 este -
    lung lungNum, lungExp;
    
public:
    nrReal() : numUnit(0), numDom(0), expUnit(0), expDom(0),
    semnNum(0), semnExp(0), lungNum(0), lungExp(0) { };
    nrReal(char[], char[]);
    nrReal(const nrReal&);
    ~nrReal();
    nrReal& operator= (const nrReal&);
    
    static void adaug(nod *&p, nod *&u, short x) {                  // static pt ca functia este legata de clasa si nu de obiecte individual
        nod *c = new nod;                                           // construim nodul
        c->val = x;
        c -> urm = NULL;
        
        if(!p) {                                                    // daca lista este vida
            c->ant = NULL;
            p = c;                                                  // nodul va fi primul (si ultimul)
        }
        else {                                                      // altfel il adaugam la final
            c->ant = u;
            u->urm = c;
        }
        u = c;                                                      // nodul adaugat va fi mereu ultimul
    }
    
    lung getExp();
    void setExp(nrReal::nod*);
    void delDomZ();
    void delFinZ();
    nrReal abs();
    friend nrReal maxAbs(nrReal, nrReal);
    friend void swap(nrReal&, nrReal&);
    
    friend bool operator! (const nrReal&);
    friend bool operator== (const nrReal&, const nrReal&);
    friend bool operator!= (const nrReal&, const nrReal&);
    friend bool operator>  (const nrReal&, const nrReal&);
    friend bool operator<  (const nrReal&, const nrReal&);
    friend bool operator<= (const nrReal&, const nrReal&);
    friend bool operator>= (const nrReal&, const nrReal&);
    
    friend nrReal operator+ (const nrReal&, const nrReal&);
    friend nrReal operator- (const nrReal&, const nrReal&);
    friend nrReal operator* (const nrReal&, const nrReal&);
    friend nrReal operator/ (const nrReal&, const nrReal&);
    friend nrReal operator<< (const nrReal&, lung);
    friend nrReal operator+= (nrReal&, const nrReal&);
    friend nrReal operator-= (nrReal&, const nrReal&);
    friend nrReal operator*= (nrReal&, const nrReal&);
    friend nrReal operator/= (nrReal&, const nrReal&);
    friend nrReal operator<<= (nrReal&, lung);
    
    friend std::istream& operator>> (std::istream&, nrReal&);
    friend std::ostream& operator<< (std::ostream&, nrReal&);
} v[10];

lung nrReal::getExp() {
    nod *c;
    long i, k;
    
    for(i = k = 0, c = expUnit; c; c=c->urm, i++)                   // calculam cat este defapt nr repr de lista exp
        k += pow(10,i) * c->val;                                    // exp negativ poate avea maxim valoarea ULL_MAX
    
    return k;
}
void nrReal::setExp(nrReal::nod *p) {
    expUnit = expDom = NULL;
    
    for(nrReal::nod *c = p; c; c=c->urm) {
        nrReal::adaug(expUnit, expDom, c->val);
        lungExp++;
    }
}
void nrReal::delDomZ() {
    nod *c;
    lung i;
    
    for(i=0, c = numDom; !c->val && c->ant; c=c->ant, i++);         // inaintam cat timp gasim zerouri
    
    delete c->urm;                                                  // eliberam memoria
    c->urm = NULL;                                                  // taiem legatura
    
    numDom = c;                                                     // actualizam adresa
    lungNum -= i;                                                   // si nr de cifre
}
void nrReal::delFinZ() {
    nod *c;
    lung i, j;
    
    for(i=0, c = numUnit; !c->val && c->urm; c=c->urm, i++);        // cat timp gasim zerouri finale inaintam
    
    if(i) {
        delete c->ant;                                              // eliberam memoria
        c->ant = NULL;                                              // taiem legatura
        
        numUnit = c;                                                // actualizam adresa
        lungNum -= i;                                               // actualizam nr de cifre al nrului
        
        i = expUnit->val - i;                                       // noua valoare a exp
        expUnit = expDom = NULL;                                    // stergem listele exp-ului
        for(j=0; i; j++, i/=10)                                     // le recream din valoarea calculata anterior
            adaug(expUnit, expDom, i);
        lungExp = j;                                                // actualizam nr de cifre al expului
        semnExp = (j>0);                                            // actualizam si semnul, daca cumva am dat in pozitiv
    }
}
nrReal maxAbs(nrReal a, nrReal b) {
    nrReal x = a, y = b;
    x.semnNum = y.semnNum = 0;                                      // aplicam modul
    return x>y ? a : b;                                             // le comparam
}
nrReal nrReal::abs() {
    nrReal temp = *this;
    temp.semnNum = 0;
    return temp;
}
void swap(nrReal& a, nrReal& b) {
    nrReal temp;
    memcpy(&temp.semnNum, &a.semnNum, 2*sizeof(bool)+2*sizeof(lung));
    memcpy(&a.semnNum,    &b.semnNum, 2*sizeof(bool)+2*sizeof(lung));
    memcpy(&b.semnNum,    &temp.semnNum, 2*sizeof(bool)+2*sizeof(lung));
    
    nrReal::nod *pTemp;
    pTemp = a.numDom; a.numDom = b.numDom; b.numDom = pTemp;
    pTemp = a.numUnit; a.numUnit = b.numUnit; b.numUnit = pTemp;
    pTemp = a.expDom; a.expDom = b.expDom; b.expDom = pTemp;
    pTemp = a.expUnit; a.expUnit = b.expUnit; b.expUnit = pTemp;
}

void checkInput(char s[NMAX]) {
    for(int i=0; i<strlen(s); i++)
        if(s[i] == '-' && i)
            throw 1;
        else if((s[i] < '0' || s[i] > '9') && s[i] != '-')
            throw s[i];
    
}
nrReal::nrReal (char sNum[NMAX], char sExp[NMAX]) {
    checkInput(sNum);
    checkInput(sExp);
    
    numUnit = numDom = expUnit = expDom = NULL;
    
    semnNum = (sNum[0] == '-');
    semnExp = (sExp[0] == '-');
    lungNum = strlen(sNum) - semnNum;
    lungExp = strlen(sExp) - semnExp;
    
    for(lung i=lungNum - !semnNum; i-semnNum +1 != 0; i--)          // creearea listelor de cifre
        nrReal::adaug(numUnit, numDom, (short)sNum[i] - '0');
    for(lung i=lungExp - !semnExp; i-semnExp +1 != 0; i--)          // nu adaugam si '-' in lista de cifre
        nrReal::adaug(expUnit, expDom, (short)sExp[i] - '0');
}
nrReal::nrReal (const nrReal& sursa) {
    *this = *new nrReal;                                            // facem o initializare
    memcpy(&this->semnNum, &sursa.semnNum, 2*sizeof(bool) + 2*sizeof(lung)); // copiem semnul si lungimea numelui si a exponentului
    
    for(nrReal::nod* c = sursa.numUnit; c; c=c->urm)                // copiem cifrele nr-ului si exp-ului nod cu nod
        nrReal::adaug(this->numUnit, this->numDom, c->val);
    for(nrReal::nod* c = sursa.expUnit; c; c=c->urm)
        nrReal::adaug(this->expUnit, this->expDom, c->val);
}
nrReal& nrReal::operator= (const nrReal& sursa) {
    memcpy(&this->semnNum, &sursa.semnNum, 2*sizeof(bool) + 2*sizeof(lung)); // copiem semnul si lungimea numelui si a exponentului
    
    this->numUnit = this->numDom = this->expUnit = this->expDom = NULL; // facem o initializare
    
    for(nrReal::nod* c = sursa.numUnit; c; c=c->urm)                // copiem cifrele nr-ului si exp-ului nod cu nod
        nrReal::adaug(this->numUnit, this->numDom, c->val);
    for(nrReal::nod* c = sursa.expUnit; c; c=c->urm)
        nrReal::adaug(this->expUnit, this->expDom, c->val);
    
    return *this;
}
nrReal::~nrReal() {
    if(numUnit) {                                                   // dc lista nr-ului este nevida
        if(!numUnit->urm)                                           // dc lista are un singur element
            delete numUnit;                                         // il stergem
        else                                                        // altfel stergem nodurile unul cate unul
            for(nrReal::nod *c = numUnit->urm; c; c=c->urm)
                delete c->ant;
    }
    
    if(expUnit) {                                                   // la fel si pt exponent
        if(!expUnit->urm)
            delete expUnit;
        else
            for(nrReal::nod *c = expUnit->urm; c; c=c->urm)
                delete c->ant;
    }
}

std::ostream& operator<< (std::ostream &os, nrReal& x) {
    nrReal::nod *c;
    lung i, j, k = x.getExp();
    
    if(x.semnNum) os << '-';                                        // afisam semnul numarului
    
    if(x.semnExp && k >= x.lungNum) {                               // pt numere ca 0,000123
        os << "0,";
        for(i=1; i <= k-x.lungNum; i++) os << 0;
    }
    
    for(i=1, c = x.numDom; c; c=c->ant, i++) {                      // afisarea propriu zisa
        os << c->val;
        if(x.semnExp && i == x.lungNum-k) os << ',';                // dupa ce am afisat destule cifre, afisam si virgula
    }
    
    if(!x.semnExp)                                                  // daca exp este pozitiv, afisam zerouri
        for(i = 0, c = x.expUnit; c; c=c->urm, i++)                 // pt fiecare cifra din componenta exponentului
            for(j=1; j <= pow(10,i) * c->val; j++)
                os  << 0;                                           // afisam atatea zerouri cand indica valoarea inmultita cu 10^(pozitia)
    
    return os;
}
std::istream& operator>> (std::istream &is, nrReal& x) {
    x = *new nrReal;
    char sNum[NMAX], sExp[NMAX];                                    // un nr mare poate avea maxim NMAX cifre (NMAX <= ULL_MAX)
    is >> sNum >> sExp;                                             // citirea ca string
    
    try {                                                           // incercam sa cream nrul citit
        nrReal temp(sNum, sExp);                                    // initializam un nr temporar cu datele citite
        x = temp;                                                   // acesta va fi nrul citit
    }
    catch (int minus) {
        cout << "EROARE: Minusul se pune doar in fata numarului" << endl;
        exit(1);
    }
    catch (char litera) {
        cout << "EROARE: Printre cifrele numarului am gasit o litera, " << litera << endl;
        exit(1);
    }
    catch(...) {
        cout << "EROARE: neidentificata";
        exit(1);
    }
    
    return is;
}

bool operator! (const nrReal& x) {
    return !x.numDom->val;
}
bool operator== (const nrReal& st, const nrReal& dr) {
    if(memcmp(&st.semnNum, &dr.semnNum, 2*sizeof(bool) + 2*sizeof(lung)))
        return 0;                                                   // daca au semnul sau lungimea nr-ului sau exp-ului diferite returnam zero
    
    nrReal::nod *c, *d;
    for(c = st.numUnit, d = dr.numUnit; c && d; c=c->urm, d=d->urm)
        if(c->val != d->val)                                        // verificam cifra cu cifra daca sunt egale
            return 0;
    
    for(c = st.expUnit, d = dr.expUnit; c && d; c=c->urm, d=d->urm)
        if(c->val != d->val)
            return 0;
    
    return 1;                                                       // daca am ajuns pana aici, inseamna ca sunt egale
}
bool operator!= (const nrReal& st, const nrReal& dr) {
    return !(st==dr);
}
bool operator> (const nrReal& st, const nrReal& dr) {
    nrReal a = st, b = dr;
    lung n = a.lungNum, m = b.lungNum;
    lung x = a.getExp(), y = b.getExp();
    bool schimb = 0;
    
    if(a.semnNum != b.semnNum) return !a.semnNum;                   // dc unul este negativ, il returnam pe celalalt
    
    if(!a.semnExp && !b.semnExp)                                    // dc ambele exp sunt poz
        n += x, m += y;                                             // luam in calcul zerourile la lungime
    
    if(a.semnExp && b.semnExp && x != y) {                          // dc ambele exp sunt neg
        if(n-x > m-y) return 1;                                     // dc (a) are mai multe cifre inaintea virgulei
        else if(n-a.getExp() < m-b.getExp()) return 0; }
    if(a.semnExp && b.semnExp && x == y) {
        if(n != m) return n > m;
    }
    
    if(!a.semnExp && !b.semnExp && n != m)                          // in caz de lungimi diferite il returnam pe cel mai lung
        return n > m;
    
    if(a.semnExp != b.semnExp)                                      // dc un exp este poz si unul neg
    {
        if(a.semnExp) schimb = 1, swap(a,b);                        // (a) va fi mereu cel cu exp poz
        
        n = a.lungNum + a.getExp(), m = b.lungNum > b.getExp() ? b.lungNum - b.getExp() : 0;
        if(n != m) return (n > m)-schimb;
    }
    
    for(nrReal::nod *c=a.numDom, *d=b.numDom; c; c=c->ant, d=d->ant)// comparam cifra cu cifra incepand cu cele dominante
        if(c->val > d->val)
            return 1;
        else if(d->val > c->val)
            return 0;
    
    return 0;                                                       // daca am ajuns pana aici inseamna ca toate cifrele sunt egale
}
bool operator< (const nrReal& st, const nrReal& dr) {
    return !(st>dr) && !(st==dr);
}
bool operator<= (const nrReal& st, const nrReal& dr) {
    return (st<dr) || (st==dr);
}
bool operator>= (const nrReal& st, const nrReal& dr) {
    return (st>dr) || (st==dr);
}

nrReal operator+ (const nrReal& st, const nrReal& dr) {
    nrReal rez, a = st, b = dr;
    nrReal::nod *c, *d;
    bool e10, carry = 0, adaugNum = 1;
    short z;
    lung i = 1, k = 0;
    lung x = a.getExp(), y = b.getExp();
    
    rez.semnExp = (a.semnExp || b.semnExp);
    
    if(a.semnNum == b.semnNum) {                                    // adunare (acelasi semn)
        rez.semnNum = a.semnNum;
        
        if(a.semnExp && b.semnExp) {                                // daca ambele au exp neg, ii dam celui mai mic dif zerouri la final
            if(a.getExp() < b.getExp()) swap(a,b);                  // in (a) va fi mereu cel cu cifre mai multe dupa virgula
            x = a.getExp(), y = b.getExp();
            b <<= x-y;
        }
        
        if(a.semnExp != b.semnExp) {                                // dc unul are exp poz si unul neg
            if(a.semnExp) swap(a,b);                                // in (a) va fi mereu cel cu exp poz
            x = a.getExp(), y = b.getExp();
        }
        
        for(i=1, c = a.numUnit, d = b.numUnit; c || d; i++)         // parcurgem ambele numere
        {
            adaugNum = 1;
            if(a.semnExp != b.semnExp && i <= a.getExp() + b.getExp())
            {                                                       // ne prefacem ca adaugam valexp(a) + valexp(b) zerouri
                if(d) z = d->val, d = d->urm;                       // ori punem cifrele celuilalt numar
                else z = 0;                                         // ori zerouri (la mijlocul numarului)
            }
            else if(!rez.semnExp && i<=x && i<=y)                   // daca ambele au zerouri la sfarsit
            {
                adaugNum = 0;
                if(!rez.expUnit)                                    // incrementam valoare exponentului
                    nrReal::adaug(rez.expUnit, rez.expDom, 1), rez.lungExp++;
                else                                                // avem grija sa nu avem o valoare > 9 in lista exp
                {
                    nrReal::nod *c = rez.expUnit;
                    while(c->val == 9 && c->urm)
                        c->val = 0, c=c->urm;
                    if(rez.expDom->val == 9) rez.expDom->val = 0, nrReal::adaug(rez.expUnit, rez.expDom, 1), rez.lungExp++;
                    else c->val++;
                }
            }
            else if(!rez.semnExp && i<=x) {                         // daca mai avem zerouri doar la un nr
                if(d) z = d->val, d = d->urm;                       // ori punem cifrele celuilalt numar
                else z = 0;                                         // ori zerouri (la mijlocul numarului)
            }
            else if(!rez.semnExp && i<=y) {
                if(c) z = c->val, c = c->urm;
                else z = 0;
            }
            else if(c && d) {                                       // daca nu s-au terminat ambele numere avansam in ambele
                z = c->val + d->val;
                c = c->urm, d = d->urm;
            }
            else if(c) {                                            // daca s-a terminat unul, copiem cifrele celuilalt
                z = c->val;
                c = c->urm;
            }
            else if(d) {
                z = d->val;
                d = d->urm;
            }
            
            if(adaugNum) {
                e10 = z + carry >= 10;                             // verificam daca avem depasire
                nrReal::adaug(rez.numUnit, rez.numDom, (z + carry) - e10 * 10);
                carry = z + carry >= 10;
            }
        }
        
        if(carry)                                                   // dc a ramas transport la cifra dominanta
            nrReal::adaug(rez.numUnit, rez.numDom, 1);
    }
    
    else {                                                          // scadere (semne diferite)
        if(a != maxAbs(a,b)) swap(a,b);                             // in (a) punem max abs
        x = a.getExp(), y = b.getExp();
        
        rez.semnNum = a.semnNum;
        
        if(!a.semnExp && !b.semnExp) {                              // dc ambele exp sunt poz
            i = k = x < y ? x : y;                                  // la exp rez trecem zerourile comune
            while(k) nrReal::adaug(rez.expUnit, rez.expDom, k), k/=10;
            rez.lungExp = (int)log10(k) +1;
            k = i;
            
            if(a.lungNum + x == b.lungNum + y)                      // dc nr-ele au aceeasi lungime
                k += x < y ? x-y : y-x;                             // inseamna ca in b avem zerouri mai multe
        }
        
        if(a.semnExp && b.semnExp) {                                // dc ambele exp sunt neg
            if(x<y) a <<= y-x, e10=0;                               // celui cu mai putine cifre dupa virgula i le adaugam pe cele lipsa
            else a <<= x-y, e10=1;
        }
        
        if(a.semnExp != b.semnExp) {
            rez.semnExp = 1;
            if(a.semnExp) b <<= x+y, e10=1;
            else a <<= x+y, e10=0;
        }
        
        for(i = 1, c = a.numUnit, d = b.numUnit; c || d; i++)
        {
            z = -carry;
            if(c) {
                if(!a.semnExp && !b.semnExp && i <= x-k && k < x) goto skip1;
                z += c->val;                                        // dc ambele exp sunt poz scadem din zero dc nr mai mic nu a ajuns la cifre
                c = c->urm;
            } skip1:
            
            if(d) {
                if(!a.semnExp && !b.semnExp && i <= x-k && k > x) goto skip2;
                z -= d->val;                                        // dc au aceeasi lung scadem zero in schimb
                d = d->urm;
            }skip2:
            
            carry = z < 0;
            z = carry ? 10+z : z;
            nrReal::adaug(rez.numUnit, rez.numDom, z);
        }
        
        rez.delDomZ();
    }
    
    if(rez.semnExp) {
        rez.setExp(a.semnExp ? a.expUnit : b.expUnit);              // avem grija sa punem virgula
        rez.delFinZ();                                              // si sa stergem zerourile de la sfarsit (ca pt 0,5 + 1,5)
    }
    
    rez.lungNum = 0;
    for(c=rez.numUnit; c; c=c->urm) rez.lungNum++;                  // numaram cifrele rezultatului
    
    return rez;
}
nrReal operator- (const nrReal& st, const nrReal& dr) {
    nrReal temp = dr;
    temp.semnNum = !temp.semnNum;
    temp += st;
    if(!temp.numDom->val) temp.semnNum = 0;
    return temp;
}
nrReal operator* (const nrReal& st, const nrReal& dr) {
    nrReal rez, a = st, b = dr;
    nrReal::nod *c, *d, *e;
    lung i, j, k, n = a.lungNum, m = b.lungNum;
    short carry = 0;
    
    rez.lungNum = n + m -1;
    rez.semnNum = st.semnNum != dr.semnNum;
    
    for(i=1; i<=rez.lungNum; i++)                                   // initializam cu zerouri
        nrReal::adaug(rez.numUnit, rez.numDom, 0);
    
    for(i=1, c = a.numUnit; c; c=c->urm, i++)
        for(j=1, d = b.numUnit; d; d=d->urm, j++)
        {
            for(k=1, e=rez.numUnit; k<i+j-1; e=e->urm, k++);        // rez[i+j-1] += a[i] * b[j]
            e->val += c->val * d->val;
        }
    
    for(i=1,c = rez.numUnit; c; c=c->urm,i++)                       // trecem transporturile
    {
        carry = (c->val += carry) /10;
        c->val %= 10;
    }
    if(carry)                                                       // daca avem transport la ultima cifra
        nrReal::adaug(rez.numUnit, rez.numDom, carry), rez.lungNum++;
    
    n = a.getExp(), m = b.getExp();
    if(a.semnExp == b.semnExp) {                                    // dc au acelasi semn
        k = n + m;                                                  // adunam exponentii
        rez.semnExp = a.semnExp;                                    // si punem semnul comun
    }
    else {                                                          // dc exp au semne dif
        k = n > m ? n-m: m-n;                                       // calculam diferenta
        if(k) rez.semnExp = n > m ? a.semnExp : b.semnExp;          // si punem semnul celui mai mare (daca dif nu e zero)
    }
    
    rez.expUnit = rez.expDom = NULL;                                // stergem listele exp-ului
    while(k) nrReal::adaug(rez.expUnit, rez.expDom, k), k/=10;      // le recream din valoarea calculata anterior
    rez.lungExp = (int)log10(k) +1;                                 // actualizam nr de cifre al expului
    
    return rez;
}
nrReal operator/ (const nrReal& st, const nrReal& dr) {
    
    /*nrReal rez, x, y;
     lung k = 0, deAdaugat = dr.lungNum -1;
     
     nrReal::adaug(x.numUnit, x.numDom, 1);
     x.lungNum++;
     cout << "inainte: " << x << dr.lungNum-1;
     x <<= dr.lungNum -1;                                            // ii adaugam suficiente zerouri lui x sa putem scadea din el
     cout << "dupa " << x;
     
     int precizie=0;
     while(!!x)                                                      // cat timp descazutul este nenul
     {
     if(++precizie>4) break;
     
     deAdaugat++;                                                // punem un zero la inceputul catului (la final)
     x <<= 1;                                                    // si un zero la sfarsitul descazutului
     
     while((y + dr) < x) y+=dr, k++;                             // calculam de cate ori intra (dr) in x
     nrReal::adaug(rez.numUnit, rez.numDom, k);                  // punem de cate ori a intrat la sfarsitul catului
     cout << "din " << x << "scadem pe " << y;
     x -= y;                                                     // efectuam scaderea
     cout << "x = " << x;
     }
     
     while(deAdaugat--) {                                            // adaugam zerourile necesare la inceputul catulu
     nrReal::adaug(rez.numUnit, rez.numDom, 0);
     rez.lungNum++;
     }
     
     rez.semnExp = 1;
     k = rez.lungNum -1;
     while(k) nrReal::adaug(rez.expUnit, rez.expDom, k), k/=10;      // punem virgula cu o cifra dupa zero
     rez.lungExp = (int)log10(k) +1;                                 // actualizam nr de cifre al expului
     
     
     cout << rez;
     //rez *= st;*/
    cout << "!!! impartirea este nedefinita !!! intoarcem inmultire in schimb !!!";
    return st*dr;
}
nrReal operator<< (const nrReal& st, lung k) {
    nrReal temp = st;                                               // shiftam spre stanga, adica inmultim cu 10 adica punem zerouri la sfarsit
    while(k--) {
        nrReal::nod *c = new nrReal::nod;
        c->val = 0;
        
        c->urm = temp.numUnit;
        temp.numUnit->ant = c;
        temp.numUnit = c;
        
        temp.lungNum++;
    }
    
    return temp;
}

nrReal operator+= (nrReal& st, const nrReal& dr) {
    st = (st+dr);
    return st;
}
nrReal operator-= (nrReal& st, const nrReal& dr) {
    st = (st-dr);
    return st;
}
nrReal operator*= (nrReal& st, const nrReal& dr) {
    st = (st*dr);
    return st;
}
nrReal operator/= (nrReal& st, const nrReal& dr) {
    st = (st/dr);
    return st;
}
nrReal operator<<= (nrReal& st, lung k) {
    st = (st<<k);                                                   // st *= 10^k
    return st;
}

void meniu() {
    char s1[NMAX], s2[NMAX], rasp[2];
    nrReal x;
    
    cout << "Pentru primul operand: \n";
    cout << "\tnumarul = "; cin >> s1;
    cout << "\tinmultit cu 10 la puterea = "; cin >> s2;
    nrReal st(s1,s2);
    
    cout << "Pentru al doilea operand: \n";
    cout << "\tnumarul = "; cin >> s1;
    cout << "\tinmultit cu 10 la puterea = "; cin >> s2;
    nrReal dr(s1,s2);
    
    cout << endl;
    cout << st << " + " << dr << " = " << (x = st+dr) << endl;
    cout << st << " * " << dr << " = " << (x = st*dr) << endl;
    cout << "maxAbs = " << (x = maxAbs(st, dr));
    
    cout << "\n\nMai testam odata? (da/nu) "; cin >> rasp;
    if(tolower(rasp[0]) == 'd' && tolower(rasp[1]) == 'a')
        meniu();
}

int main()
{
    /*ifstream f ("date.txt");
     int N; f >> N;
     for(int i=1; i<=N; i++) {
     f >> v[i];
     cout<<v[i]<<endl;
     }
     f.close();*/
    
    meniu();
    
    return 0;
}