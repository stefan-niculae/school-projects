#include <iostream>
#include <fstream>

using namespace std;

#define NMAX 20

class Graf
{
public:
    int n, m;
    int nume[NMAX];
    struct nod
    {
        int val;
        nod* urm;
    } *lista[NMAX];
    
    int grade[NMAX];
    
    void adaug(int, int);
};

void Graf::adaug(int x, int y)
{                                                               // la lista lui x il adaug pe y
    bool gasit = 0;
    nod* c;
    nod* d = new nod;
    d->val = y;
    d->urm = NULL;
    
    for(int i=1; i<=n; i++)
        if(nume[i] == x) {                                              // cautam lista lui x
            for(c = lista[i]; c->urm; c=c->urm);                        // cautam sfarsitul listei lui x
            c->urm = d;                                                 // si adaugam pe y la lista
            gasit = 1;
            break;
        }
    if(!gasit) {                                                        // daca nu l-am gasit
        for(int i=1; i<=n; i++)                                         // cautam prima pozitie libera
            if(!nume[i]) {
                nume[i] = x;                                            // cream lista lui x
                lista[i] = d;                                           // cu un element, adica y
                break;
            }
    }
}
istream& operator>> (istream& is, Graf& G)
{
    int x, y;
    
    is >> G.n >> G.m;
    for(int i=1; i<=G.m; i++) {
        is >> x >> y;
        G.adaug(x, y);
        G.adaug(y, x);
    }
    
    return is;
}
ostream& operator<< (ostream& os, Graf& G)
{
    for(int i=1; G.nume[i]; i++) {
        os << G.nume[i] << ": ";
        for(Graf::nod* c = G.lista[i]; c; c=c->urm)
            os << c->val << ' ';
        os << endl;
    }
    
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
bool izomorfe(Graf G1, Graf G2)
{
    if(G1.n != G2.n || G1.m != G2.m)                                    // daca nu au acelasi nr de noduri si de muchii
        return 0;

    int n = G1.n, st[NMAX], k = 1, as;                                  // backtracking
     
    st[1] = 0;
    
    for(int i=1; i<=G1.n; i++)
    {
        Graf::nod* c;
        for(G1.grade[i] = 0, c = G1.lista[i]; c; c=c->urm, G1.grade[i]++);
        for(G2.grade[i] = 0, c = G2.lista[i]; c; c=c->urm, G2.grade[i]++);
    }
    
    cout << "Lista gradelor lui G1: "; for(int i=1; i<=G1.n; i++) cout << G1.grade[i] << ' '; cout << endl;
    cout << "Lista gradelor lui G1: "; for(int i=1; i<=G2.n; i++) cout << G2.grade[i] << ' '; cout << endl;
    
    while(k) {
        do {
            as = succesor(st,k,G1.n);
            } while (as && !valid(st,k,n));
        if(as)
            if(k==n) {
                bool egale = 1;                                         // il permutam pe G1 dupa permutarea generata de backtracking

                cout << endl << "\nPERMUTAREA: ";
                for(int i=1; i<=n; i++)
                    cout << st[i] << ' ';
                cout << endl;
                for(int i=1; i<=n; i++)
                {
                    cout <<G1.nume[st[i]] << ": ";
                    for(Graf::nod *c = G1.lista[st[i]]; c; c=c->urm)
                        cout << c->val << ' ';
                    cout << endl;
                }
                
                for(int i=1; i<=n && egale; i++)                        // mergem prin fiecare lista
                    if(G1.nume[st[i]] != G2.nume[i]) {                  // trebuie sa aiba numele egal
                        egale = 0;
                        break;
                    }
                
                    else                                                // gradul si fiecare element
                        for(Graf::nod* c = G1.lista[st[i]], *d = G2.lista[i]; c && d; c=c->urm, d=d->urm)
                            if(c->val != d->val || (bool)c->urm != (bool)d->urm) {
                                egale = 0;                               // grafurile nu sunt egale
                                break;
                            }
                
                if(egale)                                               // daca fiecare nod are numele, gradul si lista egala
                    return 1;                                           // o permutare a lui G1 este egala cu G2 => izomorfe
                
            }
            else
                st[++k] = 0;
        else
            k--;
     }

    return !memcmp(&G1.grade, &G2.grade, sizeof(G1.grade));                   // nicio permutare a lui G1 egala cu G2 => neizomorfe
}


int main()
{
    Graf G1, G2;
    ifstream f ("date.txt");
    
    f >> G1 >> G2;
    f.close();
    
    cout << "G1: \n" << G1 << "\nG2: \n" << G2;
    cout << (izomorfe(G1, G2) ? "\n" : "\nNE") << "IZOMORFE\n";
    
    return 0;
}

