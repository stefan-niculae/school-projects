#ifndef algoritmUngar_algoritmUngar_h
#define algoritmUngar_algoritmUngar_h
#include "GrafBipartit.h"

#define INF 999

void GrafBipartit::algoritmUngar()
{
    // Scadem costurile minime pentru fiecare muncitor si slujba
    scadereCostMinim();
    
    std::cout << std::endl << *this;
    
    int k = 1;
    // Cat timp nu avem un cuplaj perfect
    while (cuplajBipartitMaxim() != N) {
        std::cout << "\n\n----------PASUL" << k++ << "----------\n\n";
        
        afisareCuplaj();
        
        // Gasim acoperirea minima a varfurilor
        acoperireVarfuriMinima();
        
        // Ajustam ponderile corespunzator
        ajustarePonderi();
        
        
        
        if(k==10)break;
        std::cout << std::endl << *this;
        
        //break;// de scos!!!!!!!!!!!!!!!!!
        
    }
    
}

GrafBipartit GrafBipartit::scadereCostMinim()
{
    // Mergem pe linia fiecarui muncitor
    for (int i = 1; i <= N; i++) {
        
        int min = INF;
        
        // Gasim salariul minim pe care il vrea pt orice slujba
        for (int j = 1; j <= N; j++)
            if (a[i][j] != -1)
                min = a[i][j] < min ? a[i][j] : min;
        
        std::cout << "minimul pt linia " << i << " este " << min<< "\n";
        
        // Dupa ce l-am gasit, il scadem din fiecare
        for (int j = 1; j <= N; j++)
            a[i][j] -= min;
    }
    
    // Mergem pe linia fiecarei slujbe
    for (int i = 1; i <= N; i++) {
        
        int min = INF;
        
        // Gasim pretul minim cerut de orice muncitor
        for (int j = 1; j <= N; j++)
            if (a[j][i] != -1)
                min = a[j][i] < min ? a[j][i] : min;
                std::cout << "minimul pt coloana " << i << " este " << min<< "\n";
        
        // Dupa ce l-am gasit, il scadem din fiecare
        for (int j = 1; j <= N; j++)
            a[j][i] -= min;
        
    }
    
    return *this;
}

// Returnam 1 <=> este posibil un cuplaj pt nodul u
bool GrafBipartit::cuplajBipartit (int u, bool viz[])
{
    // Incercam fiecare slujba
    for (int v = 0; v <= N; v++)
        
        // Ne uitam numai la slujbele nevizitate si pe a caror muchie avem zero
        if (a[u][v] == 0 && !viz[v]) {
            
            // O marcam ca vizitata
            viz[v] = true;
            
            // Daca slujba v nu are un muncitor SAU
            // muncitorul anterior are alte slujble posibile
            if (cuplaj[v] == -1 ||
                cuplajBipartit (cuplaj[v], viz) ) {
                
                cuplaj[v] = u;
                return true;
            }
        }
    
    return false;
}

// Returnam numarul maxim de cuplaje de la M la N
int GrafBipartit::cuplajBipartitMaxim()
{
    // Resetam vectorul de cuplaje
    memset(cuplaj, -1, sizeof(cuplaj));
    
    // Numarul de slujbe pentru care am gasit muncitor
    int k = 0;
    
    // Parcurgem fiecare muncitor
    for (int u = 0; u <= N; u++) {
        
        // Toate slujbele sunt nevizitate de urmatorul muncitor
        bool viz[NMAX];
        memset (viz, false, sizeof(viz));
        
        // Verificam daca muncitorul u poate avea slujba v
        if (cuplajBipartit (u, viz))
            k++;
        
    }
    std::cout << "avem " << k << " cuplaje!\n";
    return k;
}

void GrafBipartit::afisareCuplaj()
{
    // cuplaj[i] este muncitorul pentru slujba i
    // cuplaj[i] = -1 <=> slujba i nu are muncitor
    
    for (int i = 1; i <= N; i++) {
        
        std::cout << "Slujba " << i;
        
        if(cuplaj[i] == -1)
            std::cout << " nu are muncitor\n";
        else
            std::cout << " are muncitorul " << cuplaj[i] << "\n";
    }
    
}

void GrafBipartit::acoperireVarfuriMinima()
{
    // Initializam acoperirea nula
    memset(acoperire, false, sizeof(acoperire));
    
    // Cream un nou subgraf, in reprezentarea cu matrice de adiacenta
    int subgraf[NMAX][NMAX];
    memset(subgraf, 0, sizeof(subgraf));
    
    // In care retinem numai muchiile nule
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= N; j++)
            if (a[i][j] == 0)
                subgraf[i][N+j] = subgraf[N+j][i] = 1;
    
    // Si cele marcate in cuplajul maxim
    for (int i = 1; i <= N; i++)
        if (cuplaj[i] != -1)
            subgraf[N+i][ cuplaj[i] ] = subgraf[ cuplaj[i] ][N+i] = 2;
    for(int i=1; i<=N; i++){
        for(int j=1; j<=N; j++)
            std::cout << a[i][j] << ' ';
        std::cout<<std::endl;}
    
    for (int i = 1; i <= 2*N; i++){
        for (int j = 1; j <= 2*N; j++)
            std::cout << subgraf[i][j] << ' ';
        std::cout<<std::endl;}
    
    // Mergem pe linia fiecarui nod (muncitor sau slujba)
    for (int i = 1; i <= 2*N; i++) {
        
        // Verificam sa fie nu fie marcat
        bool marcat = 0;
        
        // Si sa aibe cel putin o legatura
        int k = 0;
        
        for (int j = 1; j <= 2*N && !marcat; j++) {
            
            k += subgraf[i][j];
            
            if (subgraf[i][j] == 2)
                marcat = 1;
        }
        
        if (!marcat && k > 0)
            for (int j = 1; j <= 2*N; j++)
                
                // Daca gasim o muchie nemarcanta stim sigur ca nodul la care ajunge va fi marcat
                // pentru ca asta face o acoperire maxima
                if (subgraf[i][j] == 1)
                    acoperire[j] = 1;
    }
    
    std::cout << "acoperirea: ";
    for(int i=1; i<=2*N; i++)
        if(acoperire[i])
        std::cout << i << " ";

}

void GrafBipartit::ajustarePonderi()
{
    int delta = INF;
    
    // Gasim costul minim din toata matricea (inafara de ponderile nule)
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= N; j++)
            if (a[i][j])
                delta = a[i][j] < delta ? a[i][j] : delta;
    
    // Parcurgem matricea, fiecare nod cu fiecare nod
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= N; j++) {
            
            std::cout << "pt muchia " << i << ' ' << j << "\n";
            
            // Daca niciunul din noduri nu se afla in acoperire
            if (!acoperire[i] && !acoperire[N+j]) {
                a[i][j] -= delta;
                std::cout<<"\tscadem " << delta << "(adica devine " << a[i][j] << ")\n";
            }
    
            // Daca ambele noduri se afla in acoperire
            else if(acoperire[i] && acoperire[N+j]) {
                a[i][j] += delta;
                std::cout << "\tadaugam " << delta << "(adica devine " << a[i][j] << ")\n";
            }
        }
    
}

#endif
