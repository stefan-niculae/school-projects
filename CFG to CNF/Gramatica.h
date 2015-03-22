#ifndef CFGtoCNF_Gramatica_h
#define CFGtoCNF_Gramatica_h
#include <string>
#include <sstream>
#include "Vector.h"

#define NMAX 100

class Productie
{
    char st;
    Vector<char> dr;
    
public:
    
    Productie(char stanga = '\0', Vector<char> dreapta = NULL) : st(stanga), dr(dreapta) { }
    
    friend std::istream& operator>> (std::istream&, Productie&);
    friend std::ostream& operator<< (std::ostream&, Productie);
    
    friend bool operator== (Productie, Productie);
    
    char& operator[] (int i)                // cum in stanga avem mereu o singura litera
    {                                       // e evident ca [] si lungime() se refera la dreapta
        return dr[i];
    }
    int lungime()
    {
        return dr.lungime();
    }
    
    inline char getSt() const { return st; }
    inline Vector<char> getDr() const { return dr; }
    
    friend class Gramatica;
    
};

std::istream& operator>> (std::istream& is, Productie& prod)
{
    char s[NMAX];
    
    is >> prod.st;
    is >> s;
    
    prod.dr(s);
    
    return is;
}

std::ostream& operator<< (std::ostream& os, Productie prod)
{
    return os << prod.st << " -> " << prod.dr;
}

bool operator== (Productie prod1, Productie prod2)
{
    return (prod1.st == prod2.st &&
            prod1.dr == prod2.dr);
}

class Gramatica
{
    Vector<char> N;
    Vector<char> T;
    Vector<Productie> P;
    
    bool fol[91];                           // A = 65, Z = 90
    
public:
    
    Gramatica(Vector<char> neterm = NULL, Vector<char> term = NULL, Vector<Productie> prod = NULL) :
    N(neterm), T(term), P(prod) { }
    
    friend std::istream& operator>> (std::istream&, Gramatica&);
    friend std::ostream& operator<< (std::ostream&, Gramatica);
    
    void completare(Productie);
    
    Gramatica Chomskyficare();
    Gramatica faraLambda();
    Gramatica faraDrUnTerm();
    Gramatica faraDrLungi();
    Gramatica faraAmestec();
    Gramatica faraSInDr();
    
    void construireNulabile(bool nulabil[91]);
    Gramatica redenumire(char, char);
    char nefolosit();
    int cateProduce(char);
    Vector<Productie> prodX(char);
    
    Gramatica ordonare();
    
};

std::istream& operator>> (std::istream& is, Gramatica& G)
{
    Productie temp;
    
    while(is >> temp) {
        G.P.adauga(temp);
        G.completare(temp);
    }
    
    return is;
}

#define MAJ(x) (x >= 'A' && x <= 'Z')
void Gramatica::completare(Productie prod)
{
    N.reuniune(prod.getSt());
    
    for(int i=0; i<prod.lungime(); i++) {
        char c = prod.getDr()[i];
        
        if(c == '&') return;                        // pe lambda nu il adaugam nici la N nici la T
        
        if(MAJ(c)) N.reuniune(c);
        else       T.reuniune(c);
        
    }
}

#define CONV(x) ( (int)c - 'A' )
std::ostream& operator<< (std::ostream& os, Gramatica G)
{
    //os << "N: " << G.N << "\n"
    //   << "T: " << G.T << "\n"
    //   << "P: " <<        "\n";
    //for(int i=0; i<G.P.lungime(); i++) os << "\t" << G.P[i] << "\n";
    
    bool afisat[91];                            // A = 65, Z = 90
    memset(&afisat, 0, sizeof(afisat));
    
    for(int i=0; i<G.P.lungime(); i++) {
        char st = G.P[i].getSt();
        
        if(!afisat[st]) {
            os << st << " -> ";
            afisat[st] = 1;
            
            for(int j=0, primul = 1; j<G.P.lungime(); j++)
                if(G.P[j].getSt() == st) {
                    os << (primul ? "" : " | "), primul = 0;
                    
                    if (G.P[j][0] == '&')
                        os << "λ";
                    else
                        os << G.P[j].getDr();
                }
            
            os << "\n";
        }
    }
    
    return os;
}

Gramatica Gramatica::Chomskyficare()
{
    faraLambda();
    faraDrUnTerm();
    faraDrLungi();
    faraAmestec();
    faraSInDr();
    
    ordonare();
    
    return *this;
}

Gramatica Gramatica::faraLambda()
{
    
    bool nulabil[91];                                   // A = 65, Z = 91
    construireNulabile(nulabil);
    std::cout << "NULABILE: "; for(int i=0; i<90; i++) if(nulabil[i]) std::cout << (char)i; std::cout << "\n";
    
    for(int i=0; i<P.lungime(); i++) {                  // parcurgem fiecare productie (inclusiv cele adaugate in bucla asta)
        bool modificat = 0;
        Productie initiala = P[i];
        
        for(int j=0; j<P[i].lungime(); j++)             // ne uitam in dreapta
            if(MAJ(P[i][j]) && nulabil[ P[i][j] ]) {    // daca am gasit un terminal nulabil
                modificat = 1;
                
                Productie faraNulabil = initiala;
                faraNulabil.dr.stergePoz(j);
                
                if(faraNulabil.lungime())
                    P.reuniune(faraNulabil);            // creeam o noua prod fara terminalul acela
            }
    }
    
    for(int i=0; i<P.lungime(); i++)
        if(P[i].dr[0] == '&') {
            P.stergePoz(i);
            i--;
        }
    
    return *this;
}

void Gramatica::construireNulabile(bool nulabil[91])
{
    for(int i=0; i<91; i++) nulabil[i] = 0;
    nulabil['&'] = 1;
    
    int n = P.lungime();
    
    for(int k=1; k<=n; k++)                             // facem n pasi
        for(int i=0; i<n; i++)                          // parcurge fiecare productie
            for(int j=0; j<P[i].lungime(); j++) {       // ne uitam la fiecare litera din dreapta
                
                if(!nulabil[P[i][j]])                   // dc una din litere e nenulabila inseamna ca tot neterminalul e nenulabila
                    break;                              // de ex & sau X sau XX sau XY cu X->& si Y->&
                
                if(j == P[i].lungime() -1)              // daca am verificat toate literele si niciuna nu e nulabila
                    nulabil[ P[i].st ] = 1;             // inseamna ca neterminalul este nulabil
            }
    
    for (int i = 0; i < P.lungime(); i++)
        if (P[i].lungime() != 1)
            for (int j = 0; j < P[i].lungime(); j++)
                if (P[i][j] != P[i][0]) {
                    nulabil[ P[i].st ] = 0;
                    break;
                }
}

Gramatica Gramatica::faraDrUnTerm()
{
    int nInit = P.lungime();                            // sa evitam cazul S -> A, A -> B, B -> b deci S -> b
    for(int i=0; i<nInit; i++)                          // ne uitam prin fiecare productie
        if(MAJ( P[i][0] ) && P[i].lungime() == 1)       // daca in dreapta avem o majuscula si numai atat (nu si alte litere/maj)
            for(int j=0; j<nInit; j++)                  // luam din nou toate productiile la rand
                if(P[j].st == P[i][0]) {                // daca am gasit terminalul ala care era singur in dreapta
                    Productie temp (P[i].st, P[j].dr);  // creeam o productie noua cu ala din stanga care mergea intr-un singur terminal in st
                    P.reuniune(temp);                   // si in dr cu terminalul in care mergea ala din stanga
                }
    
    for(int i=0; i<P.lungime(); i++)                    // luam din nou productiile la rand
        if(MAJ( P[i][0] ) && P[i].lungime() == 1) {     // dam iar peste alea care merg intr-un singur neterminal
            P.stergePoz(i);
            i--;
        }
    
    return *this;
}

Gramatica Gramatica::faraDrLungi()
{
    for(int i=0; i<P.lungime(); i++)                    // parcurgem toate productiile (inclusiv cele create in bucla asta)
        if(P[i].lungime() > 2) {                        // gasim o prod mai lunga ca 2
            
            Productie deRedirectionat;
            
            for(int j=0; j<P.lungime(); j++)            // mai intai cautam sa vedem daca nu e deja productia asta cu primele 2 litere
                if(P[j].lungime() == 2 &&
                   P[j][0] == P[i][0] &&
                   P[j][1] == P[i][1] )
                    if(cateProduce(P[j].st) == 1) {     // daca din el nu mai iese si altceva e bun
                        deRedirectionat = P[j];
                        break;
                    }
            
            if(!deRedirectionat.st) {                   // daca n-am gasit-o deja acolo, o cream!
                
                Vector<char> dreapta;                   // in dreapta o sa punem primele doua litere (N sau T) din prod prea lunga
                dreapta.adauga(P[i][0]);
                dreapta.adauga(P[i][1]);
                
                Productie temp( nefolosit(), dreapta ); // in stanga generam un neterminal nefolosit
                P.adauga(temp);
                deRedirectionat = temp;
            }
            
            P[i].dr.stergePoz(0);                       // stergem primul caracter
            P[i][0] = deRedirectionat.st;               // pe al doilea in inlocuim cu neterm de mai sus
            
            i--;                                        // mai testam odata prod asta, poate are mai mult de 3 litere
        }
    
    return *this;
}

char Gramatica::nefolosit()
{
    for(int i=0; i<N.lungime(); i++)                    // notam toate literele de la neterminale ca folosite
        fol[ N[i] ] = 1;
    
    for(char c = 'Z'; c>='A'; c--)
        if(!fol[c]) {                                   // pe primul nefolosit il returnam
            N.reuniune(c);
            fol[c] = 1;
            return c;
        }
    
    throw 106;                                          // daca avem mai mult de 26 de neterminale, nu mai avem ce litere sa folosim!
}

#define TOMAJ(x) (x - 32)                               // A = 65, a = 97

Gramatica Gramatica::faraAmestec()
{
    for(int i=0; i<T.lungime(); i++) {                  // pt fiecare litera
        
        Vector<char> dreapta;                           // facem o prod noua de forma X -> x
        dreapta.adauga(T[i]);
        Productie temp( TOMAJ(T[i]), dreapta );
        
        if(P.contine(temp) && cateProduce(temp.st) == 1)    // e deja aici, nu mai trebuie adaugat
            continue;
        
        if(N.contine(temp.st))
            redenumire( temp.st, nefolosit() );
        
        P.adauga(temp);                             // il adaugam la productii
        N.adauga(temp.st);                          // si la lista de neterminale
    }
    
    for(int i=0; i<P.lungime(); i++)
        if(P[i].lungime() == 2) {
            if(!MAJ(P[i][0])) P[i][0] = TOMAJ(P[i][0]);
            if(!MAJ(P[i][1])) P[i][1] = TOMAJ(P[i][1]);
        }
    
    return *this;
}

int Gramatica::cateProduce(char x)
{
    if(!MAJ(x))
        return -1;
    
    return prodX(x).lungime();
}

Gramatica Gramatica::redenumire(char x, char y)
{
    if(x == y)
        return *this;
    
    for(int i=0; i<P.lungime(); i++) {                      // parcurgem fiecare productie
        if(P[i].st == x)                                    // daca in stanga avem x il inlocuim cu y
            P[i].st = y;
        
        for(int j=0; j<P[i].lungime(); j++)                 // daca in dreapta una din litere este x
            if(P[i][j] == x)                                // o inlocuim cu y
                P[i][j] = y;
    }
    
    N.stergeElem(x);
    
    fol[x] = 0;
    fol[y] = 1;
    
    return *this;
}

Gramatica Gramatica::faraSInDr()
{
    bool areRost = 0;
    for(int i=0; i<P.lungime(); i++)                    // ne uitam prin toate productiile
        for(int j=0; j<P[i].lungime(); j++)             // numai in dreapta
            if(P[i][j] == 'S') {                        // daca gaisim S in dreapta
                areRost = 1;                            // inseamna ca are rost sa redenumim
                break;
            }
    if(!areRost) return *this;
    
    Vector<Productie> prodS = prodX('S');               // retinem ce produce S
    if(fol['T'])                                        // mai intai verificam sa nu fie deja folosit T
        redenumire('T', nefolosit());                   // daca este, il redenumim in ceva nefolosit
    redenumire('S', 'T');                               // redenuminm ce S cu T
    
    for(int i=0; i<prodS.lungime(); i++)                // in ce am retinut anterior ca produce S
        for(int j=0; j<prodS[i].lungime(); j++)         // schimbam doar in dreapta
            if(prodS[i][j] == 'S')                      // pe S in T
                prodS[i][j] = 'T';
    P += prodS;                                         // acum avem o copie a lui S doar ca ii zice T, deci se respecta regula no S in RHS
    
    return *this;
}

Vector<Productie> Gramatica::prodX(char x)
{
    if(!MAJ(x))
        return NULL;
    
    Vector<Productie> vectX;
    
    for(int i=0; i<P.lungime(); i++)
        if(P[i].st == x)
            vectX.adauga(P[i]);
    
    return vectX;
}

Gramatica Gramatica::ordonare()
{
    Vector<Productie> prodUnitare;
    for(int i=0; i<P.lungime(); i++)
        if(P[i].lungime() == 1 && cateProduce(P[i].st) == 1) {
            prodUnitare.adauga(P[i]);
            P.stergePoz(i); i--;
        }
    
    P += prodUnitare;
    
    Vector<char> Nordonat;
    for(int i=0; i<P.lungime(); i++)
        Nordonat.reuniune(P[i].st);
    N = Nordonat;
    
    int zero = 0;
    for(int k=0; k<N.lungime(); k++) {                  // sortam fiecare neterminal
        int n = zero + cateProduce(N[k]);
        
        for(int i=zero; i<n -1; i++)                    // sortam dupa prima litera (din N sau din T)
            for(int j=i+1; j<n; j++)
                if(P[i][0] > P[j][0])                   // merge pt ca 'A' < 'a'
                    std::swap(P[i], P[j]);
        
        for(int i=zero; i<n -1; i++)                    // sortam dupa a doua litera (ambele din N)
            for(int j=i+1; j<n; j++)
                if(P[i][0] == P[j][0]  &&               // de ex VT inainte de VX, dar nu VT dupa WA
                   P[i].lungime() == 2 &&               // (cine are doua litere)
                   P[j].lungime() == 2 &&
                   P[i][1] > P[j][1])
                    std::swap(P[i], P[j]);
        
        zero = n;
    }
    
    for(int i=0; i<P.lungime() -1; i++)
        for(int j=i+1; j<P.lungime(); j++)
            if(P[j].st == 'S' || P[i].st > P[j].st)     // S vrem mereu pe prima poz
                std::swap(P[i], P[j]);
    
    
    return *this;
}

#endif