#include <iostream>
#include <fstream>

using namespace std;

#define NMAX 100
#define nimic 999
#define conv(x) (int)x - 'a' +1
#define MIN(x,y) x < y ? x : y
#define MAX(x,y) x > y ? x : y

class Automat
{
public:
    int nrStari;
    char alf[NMAX]; int nrSimb = 0;
    int stareInit;
    int nrStariFin, stariFin[NMAX];
    int nrTranz;
    int tranz[NMAX][NMAX];
    
    int stareFin(int);
    bool evaluare(char[NMAX]);
    
    void minimizare();
    void stergereStariInaccesibile();
    void creareTabelClase();
    void stergereStare(int);
} M;

istream& operator>> (istream &is, Automat& M) {
    is >> M.nrStari;
    
    is >> M.alf;
    while(M.alf[++M.nrSimb]);
    
    is >> M.stareInit;
    
    is >> M.nrStariFin;
    for(int i=1; i<=M.nrStariFin; i++)
        is >> M.stariFin[i];
    
    is >> M.nrTranz;
    for(int i=0; i<=M.nrStari; i++) for(int j=1; j<=M.nrStari; j++) M.tranz[i][j] = nimic;
    for(int i=1; i<=M.nrTranz; i++)
    {
        int deUnde, panaUnde; char cuCe;
        
        is >> deUnde >> panaUnde >> cuCe;
        M.tranz[deUnde][conv(cuCe)] = panaUnde;
    }
    
    return is;
}
ostream& operator<< (ostream &os, Automat &M){
    os << "Nr stari: " << M.nrStari << endl;
    os << "Alfabetul (" << M.nrSimb << "): " << M.alf << endl;
    os << "Starea initiala: " << M.stareInit << endl;
    os << "Stari finale (" << M.nrStariFin << "): ";
    for(int i=1; i<=M.nrStariFin; i++) os << M.stariFin[i] << ' ';
    
    os << "\nMatricea tranzitiilor: " << endl;
    
    os << "   ";
    for(int i=0; i<M.nrSimb; i++)
        os << M.alf[i] << ' ';
    os << endl;
    
    for(int i=0; i<M.nrStari; i++) {
        os << i << "| ";
        for(int j=1; j<=M.nrSimb; j++)
            if(M.tranz[i][j] == nimic) cout << "  ";
            else os << M.tranz[i][j] << ' ';
        os << endl;
    }
    
    return os;
}

int Automat::stareFin(int x) {
    for(int i=1; i<=nrStariFin; i++)
        if(stariFin[i] == x)
            return i;
    return 0;
}
bool Automat::evaluare(char cuv[NMAX]) {
    int stare = stareInit;                                                          // pornim din starea initiala
    
    for(int i=0; cuv[i]; i++) {                                                     // parcurgem cuvantul dat
        stare = tranz[stare][conv(cuv[i])];                                         // calculam noua stare
        if(stare == nimic) return 0;                                                // daca am ajuns intr-o stare gunoi, cuv nu e acceptat
    }
    
    return stareFin(stare);                                                         // dc am ramas intr-o stare finala, cuv e acceptat
}

void afisClase(int n, int v[NMAX][NMAX]) {
    cout << endl << "Tabelul claselor de echivalenta: " << endl;
    for(int i=1; i<=n -1; i++) {
        cout << i << "| ";
        for(int j=0; j<=n -2; j++)
            if(v[i][j] != nimic)
                cout << v[i][j] << ' ';
        cout << endl;
    }
    cout << "\n   ";
    for(int j=0; j<=n-2; j++)
        cout << j << ' ';
    cout << endl;
}
void Automat::stergereStare(int x){
    
    if(stareFin(x)) {                                                               // daca e stare finala
        stariFin[stareFin(x)] = stariFin[nrStariFin];                               // aducem ultima stare finala din lista pe poz lui
        stariFin[nrStariFin] = 0;                                                   // stergem ultima stare finala (e copiata anterior)
        nrStariFin--;                                                               // decrementam nrul de stari finale
    }
    
    for(int i=x; i<nrStari -1; i++)                                                 // pornim de la starea de sters
        for(int j=1; j<=nrSimb; j++)                                                // copiem peste ea randul de dedesubt
            tranz[i][j] = tranz[i+1][j];                                            // litera cu litera
    for(int i=1; i<=nrSimb; i++)                                                    // am suprascris starea de sters
        tranz[nrStari][i] = nimic;                                                  // stergem dublura de pe ultimul rand
    nrStari--;
    
    for(int i=0; i<nrStari; i++)                                                    // parcurgem matricea tranzitiilor
        for(int j=1; j<=nrSimb; j++)
            if(tranz[i][j] == x)                                                    // orice legatura spre nodul sters este acum nimic
                tranz[i][j] = nimic;
            else if(tranz[i][j] > x && tranz[i][j] != nimic)                        // orice legatura spre o stare mai mare decat cea stearsa
                tranz[i][j]--;                                                      // va fi decrementata pt ca tocmai s-a suit o pozitie
}
void Automat::stergereStariInaccesibile() {
    bool accesibil[NMAX], schimbare = 1;
    memset(&accesibil, 0, sizeof(accesibil));
    
    accesibil[stareInit] = 1;
    
    while(schimbare) {
        schimbare = 0;
        
        for(int i=0; i<nrStari; i++)
            for(int j=0; j<nrSimb; j++)
                if(accesibil[i] && !accesibil[tranz[i][conv(alf[j])]]) {
                    accesibil[tranz[i][conv(alf[j])]] = 1;
                    schimbare = 1;
                }
    }
    
    for(int i=1, k=0; i<nrStari; i++)                                               // stergem starile inaccesibile
        if(!accesibil[i]) {
            stergereStare(i-k);                                                     // avem grija ca functia de stergere modifica randurile
            k++;
        }
}
void Automat::minimizare() {
    this->stergereStariInaccesibile();
    cout << "\n\tDupa stergerea starilor inaccesibile: \n" << M;
    
    int diferite[NMAX][NMAX];
    memset(&diferite, nimic, sizeof(diferite));
    
    for(int i=0; i<=nrStari -1; i++)                                                // initializam cu nimic peste tot
        for(int j=0; j<=nrStari -2; j++)                                            // inafara de starile unde avem finala/nefinala
            diferite[i][j] = (i && i > j) ? (bool)stareFin(i) != (bool)stareFin(j) : nimic;
    
    bool schimbare = 1;
    while(schimbare) {
        schimbare = 0;
        
        for(int i=1; i<=nrStari -1; i++)                                            // parcurgem tabelul facut
            for(int j=0; j<=nrStari -2; j++)
                if(!diferite[MAX(i,j)][MIN(i,j)])                                   // unde gasim stari potential egale
                    for(int l=0; l<nrSimb; l++)                                     // verificam daca delta(i) si delta(j) marcate pt orice lit
                        if(diferite[MAX(tranz[i][conv(alf[l])],tranz[j][conv(alf[l])])][MIN(tranz[i][conv(alf[l])],tranz[j][conv(alf[l])])]== 1) {
                            diferite[MAX(i,j)][MIN(i,j)] = 1;
                            schimbare = 1;
                            break;
                        }
        
    
    }
    afisClase(nrStari, diferite);
    
    int clasa[NMAX];                                                                // clasa[x] = y adica x e in clasa lui y
    for(int i=0; i<nrStari; i++)                                                    // initial fiecare stare e in clasa ei
        clasa[i] = i;
    
    int k = nrStari;
    for(int i=1; i<=nrStari -1; i++)
        for(int j=0; j<=nrStari -2; j++)
            if(!diferite[i][j]) {                                                   // pt fiecare pereche egala, unim clasele
                clasa[i] = clasa[j] = MIN(clasa[i],clasa[j]);
                k--;
            }
    
    
    for(int i=0; i<k; i++) {                                                        // redenumim clasele, in loc de 0 1 4 7 sa avem 0 1 2 3
        int gasit = 0;
        for(int j=0; j<nrStari; j++)
            if(i==clasa[j]) {
                gasit = 1;
                break;
            }
        
        if(!gasit) {
            int urm = 0;
            for(int j=0; j<nrStari; j++)
                if(!urm && clasa[j] > i) {
                    urm = clasa[j];
                    clasa[j] = i;
                }
                else if(urm && clasa[j] == urm)
                    clasa[j] = i;
        }
    }
    
    int nouaMatrice[NMAX][NMAX];                                                    // construim de la zero o noua matrice de tranz
    for(int i=0; i<k; i++)                                                          // pt fiecare clasa
        for(int l=0; l<nrStari; l++)                                                // gasim un reprezentant
            if(clasa[l] == i) {
                for(int j=0; j<nrSimb; j++)                                         // litera cu litera
                    nouaMatrice[i][conv(alf[j])] = clasa[tranz[l][conv(alf[j])]];   // construim unde merge
                break;
            }
    
    nrStari = k;                                                                    // matricea nou construita este chiar DFA minimal
    memcpy(&tranz, &nouaMatrice, sizeof(nouaMatrice));
    
    int noileFinale[NMAX];
    k = 0;
    for(int i=1; i<=nrStariFin; i++) {
        int gasit = 0;                                                              // cautam fiecare stare finala clasa
        
        for(int j=1; j<=k; j++)                                                     // in vectorul proaspat construit
            if(clasa[stariFin[i]] == noileFinale[j]) {
                gasit = 1;
                break;
        }
        if(!gasit)                                                                  // daca nu l-am gasit
            noileFinale[++k] = clasa[stariFin[i]];                                  // il adaugam
    }
    nrStariFin = k;
    memcpy(&stariFin, &noileFinale, sizeof(stariFin));                              // copiem lista de stari finale creata
        
}

int main()
{
    ifstream f ("date.txt");
    char cuv[NMAX];
    
    f >> M >> cuv;
    f.close();
    
    cout << "\tAutomatul initial: \n" << M;
    //cout << endl << "Cuvantul " << cuv << " este " << (!M.evaluare(cuv) ? "NE" : "") << "ACCEPTAT\n";

    M.minimizare();
    cout << "\n\tDupa minimizare: \n" << M;
    
    return 0;
}

