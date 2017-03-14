#ifndef operatiiMatrice_MeniuOperatii_h
#define operatiiMatrice_MeniuOperatii_h
#include "Matrice.h"
using namespace std;

// lui A ii asociem xI - A
auto constrMatrCaract(Matrice<int> A) -> Matrice<Polinom>
{
    Matrice<Polinom> B;
    
    // Setam n-ul; acelasi al lui A si pt B
    int n = A.get_n();
    B.set_n(n);
    
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
        {
            // Setam minus coeficientul din A
            B(i,j).setCoef(0, -A(i,j));
            
            // Daca suntem pe coloana principala, punem si x-ul
            if (i == j)
                B(i,j).setCoef(1, 1);
        }
    
    return B;
}

auto intregInPolin(Matrice<int> A) -> Matrice<Polinom>
{
    Matrice<Polinom> B;
    
    int n = A.get_n();
    B.set_n(n);
    
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            B(i,j).setCoef(0, A(i,j));
            
    return B;
}

void meniu(string input)
{
    // Construim o matrice din input-ul primit si o afisam
    Matrice<int> A;
    (std::istringstream) input >> A;
    cout << "A = \n" << A << "\n";
    
    int rasp;
    cout << "Lucram pe matricea A(1) sau pe xI - A(2)?";
    cin >> rasp;
    
    Matrice<Polinom> B = intregInPolin(A);
    
    if (rasp == 2)
        // Construim matricea caracteristica si o afisam
        B = constrMatrCaract(A);
    
    //cout << "B = \n" << B << "\n";
    
    // Bucla principala
    while(1)
    {
        char s[100];
        cout << "Introdu operatia: ";
        cin.getline(s, 100);
        istringstream iss(s);
        
        string str = "1";
        int x, y;
        char LsauC, op, buff;
        
        // Citim tipul operatiei, daca e pe linii sau pe coloane
        iss >> LsauC;
        
        // Tastam zero ca sa iesim din meniu
        if (LsauC == '0') break;
        
        // Citim indicele primului operand
        iss >> x;
        
        // Citim operatia
        iss >> op;
        
        // Vedem daca se inmulteste cu un polinom sau altceva
        bool atins = 0;
        while (1) {
            
            iss >> buff;
            if (toupper(buff) == 'L' || toupper(buff) == 'C' || iss.eof())
                break;
            
            // Polinomul default cu care se inmulteste este f = 1
            if (!atins)
                str = "", atins = 1;
            
            // Din ( si ) nu se face polinom
            if (buff != '(' && buff != ')')
                str += buff;
                
        }
        
        // Din sirul citit facem un polinom
        Polinom P(str);
        //std::cout << "Stringul = " << str << ", Polinomul = " << P << "\n";
        
        // Citim indicele celui de-al doilea operand
        iss >> y;
        
        // Case insensitive
        LsauC = toupper(LsauC);
        op = tolower(op);
        
        // Executam instructiunea primita
        switch (op) {
            case 'p':
                LsauC == 'L' ? B.PermLin(x, y) : B.PermCol(x, y);
                break;
                
            case '+':
                LsauC == 'L' ? B.adunLin(x, y, P) : B.adunCol(x, y, P);
                break;
            
            case '-':
                LsauC == 'L' ? B.scadLin(x, y, P) : B.scadCol(x, y, P);
                break;
            
            case '*':
                LsauC == 'L' ? B.inmultLin(x, P) : B.inmultCol(x, P);
                break;
            
            case '/':
                LsauC == 'L' ? B.impartLin(x, P) : B.impartCol(x, P);
                break;
                
            case 'g':
                LsauC == 'L' ? B.linGuma(x) : B.colGuma(x);
                break;
                
                
            default:
                break;
        }
        
        // Afisam rezultatul
        cout << "B ~ \n" << B << "\n";
        
    }
}

#endif
