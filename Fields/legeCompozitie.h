#ifndef criptografie_legeCompozitie_h
#define criptografie_legeCompozitie_h

#include <string>
#include "expressionTree.h"

template<class Tip> class Corp;
void completareCuStelute(std::string&);

template <class Tip>
class LegeCompozitie
{
    bool defaultPlus;
    bool defaultOri;
    bool defaultOp() { return defaultPlus || defaultOri; }
    
    int nVect;
    
    std::string formula;
    
public:
    
    LegeCompozitie (std::string s = "neinit") {
        construire(s);
    }
    LegeCompozitie(char c) {
        std::string s;
        s += c;
        construire(s);
    }
    LegeCompozitie<Tip> construire (std::string s = "neinit", int nV = -1) {
        
        try {
            if(s != "neinit")
            for(int i=0; i<s.length(); i++)
                if(s[i] != 'x' &&
                   s[i] != 'y' &&
                   s[i] != '+' &&
                   s[i] != '-' &&
                   s[i] != '/' &&
                   s[i] != '*' &&
                   s[i] != ' ' &&
                   (s[i] < '0'  || s[i] > '9') )
                    throw i;
        } catch(int i) {
            std::cout << "Am detectat un caracter ilegal in formula, anume " << s[i] << "!";
            exit(55);
        }

        
        nVect = nV;
        
        completareCuStelute(s);
        
        defaultPlus = (s[0] == '+');
        defaultOri  = (s[0] == '*');
        
        if(defaultOp() && s[1]) {
            std::cout << "Nu e data bine formula pt legea de compunere, nu poate incepe cu + sau *!";
            exit(17);
        }
        
        if(!defaultOp())
            formula = s;
        else {
            formula = "x ";
            formula += defaultPlus ? '+' : '*';
            formula += " y";
        }
        
        return *this;
    }
    
    template <class Tip2> friend std::istream& operator>> (std::istream&, LegeCompozitie<Tip2>&);
    template <class Tip2> friend std::ostream& operator<< (std::ostream&, LegeCompozitie<Tip2> );
    
    Tip operator() (Tip, Tip);                                  // vedem ce semn aplicam(ori + sau *, ori custom)
    
    friend class Corp<Tip>;
    template <class Tip2> friend std::istream& operator>> (std::istream&, Corp<Tip2>&);
};

inline bool eOperator (char c) { return (c == '+' || c == '-' || c == '*' || c == '/'); }
inline bool eOperand  (char c) { return (c == 'x' || c == 'y'); }
inline bool eParanteza(char c) { return (c == '(' || c == ')'); }
inline bool eSpatiu   (char c) { return (c == ' '); }
inline bool eCoef     (char c) { return (c >= '0' && c <= '9'); }
void completareCuStelute(std::string& s)
{
    if(s == "neinit")
        return;
    
    for(int i=0; i<s.length(); i++) {
        if (eSpatiu(s[i]))      // scoatem spatiile
            s.erase(i,1);
            
        if( i && (eOperand(s[ i ]) || '(' == s[i] )  &&
            (eOperand(s[i-1]) || eCoef(s[i-1]))  ) {
        
            s.insert(i, "*");
            i++;
        }
    }
}

template <class Tip2> std::istream& operator>> (std::istream& is, LegeCompozitie<Tip2>& L)
{
    /*                              // mai greu de citit testele cu getline
    char str[1000];
    is.getline(str, 1000);
    std::string s(str);
     */
    
    std::string s;
    is >> s;
    L.construire(s);
    
    return is;
}
template <class Tip2> std::ostream& operator<< (std::ostream& os, LegeCompozitie<Tip2> L)
{
    os << L.formula;
    
    if(L.nVect != -1)
        os << " pe vectori de " << L.nVect << " elemente";
    
    return os;
}

template <class Tip> Tip LegeCompozitie<Tip>::operator() (Tip x, Tip y)
{
    if(defaultPlus) return x + y;
    if(defaultOri)  return x * y;
    
    ArbBinExpr<Tip> ABE(formula, x, y, nVect);
    return ABE.rezultat();
}


#endif
